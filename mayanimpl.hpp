// MayanSort - many sort algorithms implementation in C++ 20.
// 
// MIT License:
// Copyright (c) 2023 The pysoft group.
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this softwareand associated documentation files(the
//    "Software"), to deal in the Software without restriction, including
//    without limitation the rights to use, copy, modify, merge, publish,
//    distribute, sublicense, and /or sell copies of the Software, and to
//    permit persons to whom the Software is furnished to do so, subject to
//    the following conditions :
//
// The above copyright noticeand this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// mayanimpl.hpp: Some sorting algorithm implementations.

#include <algorithm>
#include <iterator>
#include <vector>
#include "mayandef.hpp"


namespace MayanSort {
	namespace _impl {
        	template<typename T, typename Size>
	        struct tmp_buffer{
	            	T* memory;
	            	Size size;
	            	explicit tmp_buffer(Size size){
			        memory = new T[size];
		        }
	            	~tmp_buffer(){
	                	delete [] memory;
			}
		}

		// Dual-Pivot Quicksort
		namespace _dualsort {
			template<typename T, typename Size, typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			Size _dual_partition(RandomAccessIterator arr, Size low, Size high, Size* lp, Compare comp) {
				if (arr[low] > arr[high]) std::swap(arr[low], arr[high]);

				// p is the left pivot, and q is the right pivot.
				Size j = low + 1, g = high - 1, k = low + 1;
				T p = arr[low], q = arr[high];
				while (k <= g) {

					// if elements are less than the left pivot
					if (comp(arr[k], p)) {
						std::swap(arr[k], arr[j]);
						j++;
					}

					// if elements are greater than or equal
					// to the right pivot
					else if (comp(q, arr[k])) {
						while (arr[g] > q && k < g) g--;
						std::swap(arr[k], arr[g]);
						g--;
						if (comp(arr[k], p)) {
							std::swap(arr[k], arr[j]);
							j++;
						}
					}
					k++;
				}
				j--;
				g++;

				// bring pivots to their appropriate positions.
				std::swap(arr[low], arr[j]);
				std::swap(arr[high], arr[g]);

				// returning the indices of the pivots.
				*lp = j; // because we cannot return two elements
				// from a function.

				return g;
			}

			template<typename T, typename Size, typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void _dual_sort(RandomAccessIterator arr, Size low, Size high, Compare comp)
			{
				if (low < high) {
					// lp means left pivot, and rp means right pivot.
					Size lp, rp;
					rp = _dual_partition<T>(arr, low, high, &lp, comp);
					_dual_sort<T>(arr, low, lp - 1, comp);
					_dual_sort<T>(arr, lp + 1, rp - 1, comp);
					_dual_sort<T>(arr, rp + 1, high, comp);
				}
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void dqsort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				_dual_sort<MayanSort::ItValue<RandomAccessIterator>>
					(first, (MayanSort::ItSize<RandomAccessIterator>)0, std::distance(first, last), comp);
			}
		}

		// Comb Sort
		namespace _combsort {
			// To find gap between elements
			template<typename Size>
			Size _get_next_gap(Size gap)
			{
				// Shrink gap by Shrink factor
				gap = (gap * 10) / 13;

				if (gap < 1)
					return 1;
				return gap;
			}

			// Function to sort a[0..n-1] using Comb Sort
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _inner_comb_sort(RandomAccessIterator a, Size n, Comp comp) {
				// Initialize gap
				Size gap = n;

				// Initialize swapped as true to make sure that
				// loop runs
				bool swapped = true;

				// Keep running while gap is more than 1 and last
				// iteration caused a swap
				while (gap != 1 || swapped) {
					// Find next gap
					gap = _get_next_gap(gap);

					// Initialize swapped as false so that we can
					// check if swap happened or not
					swapped = false;

					// Compare all elements with current gap
					for (int i = 0; i < n - gap; i++) {
						if (comp(a[i + gap], a[i])) {
							std::swap(a[i], a[i + gap]);
							swapped = true;
						}
					}
				}
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void combsort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				_inner_comb_sort(
					first, std::distance(first, last), comp);
			}
		}

		// Bitonic Sort
		namespace _bitonicsort {
			/*The parameter dir indicates the sorting direction, ASCENDING
			or DESCENDING; if (a[i] > a[j]) agrees with the direction,
			then a[i] and a[j] are interchanged.*/
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _bitonic_cmp(RandomAccessIterator a, Size i, Size j, bool dir, Comp comp)
			{
				if (!dir == comp(a[j], a[i])) std::swap(a[i], a[j]);
			}

			/*It recursively sorts a bitonic sequence in ascending order,
			  if dir = 1, and in descending order otherwise (means dir=0).
			  The sequence to be sorted starts at index position low,
			  the parameter cnt is the number of elements to be sorted.*/
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _bitonic_merge(RandomAccessIterator a, Size low, Size cnt, bool dir, Comp comp)
			{
				if (cnt > 1)
				{
					Size k = cnt / 2;
					for (Size i = low; i < low + k; i++) _bitonic_cmp(a, i, i + k, dir, comp);
					_bitonic_merge(a, low, k, dir, comp);
					_bitonic_merge(a, low + k, k, dir, comp);
				}
			}

			/* This function first produces a bitonic sequence by recursively
				sorting its two halves in opposite sorting orders, and then
				calls bitonicMerge to make them in the same order */
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _inner_bitonic_sort(RandomAccessIterator a, Size low, Size cnt, bool dir, Comp comp)
			{
				if (cnt > 1)
				{
					Size k = cnt / 2;

					// sort in ascending order since dir here is 1
					_bitonic_sort(a, low, k, 1, comp);

					// sort in descending order since dir here is 0
					_bitonic_sort(a, low + k, k, 0, comp);

					// Will merge whole sequence in ascending order
					// since dir=1.
					_bitonic_merge(a, low, cnt, dir, comp);
				}
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void bitonic_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				_inner_bitonic_sort(
					first, 
					(MayanSort::ItSize<RandomAccessIterator>)0, std::distance(first, last),
					false, comp);
			}
		}

		// Ternary Heap Sort
		namespace _ternarysort {
			// Implement the ternary heap property of arr, with the maximum value placed in arr[i].
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _ternary_heapify(RandomAccessIterator arr, Size n, Size i, Comp comp) {
				Size largest = i;
				Size l = 3 * i + 1;
				Size m = 3 * i + 2;
				Size r = 3 * i + 3;

				if (l < n && comp(arr[largest], arr[l])) largest = l;
				if (m < n && comp(arr[largest], arr[m])) largest = m;
				if (r < n && comp(arr[largest], arr[r])) largest = r;

				if (largest != i) {
					std::swap(arr[i], arr[largest]);
					_ternary_heapify(arr, n, largest, comp);
				}
			}

			// Use ternary heap to sorting arr.
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _ternary_heapsort(RandomAccessIterator arr, Size n, Comp comp) {
				for (Size i = n / 3 - 1; i >= 0; i--)
					_ternary_heapify(arr, n, i, comp);

				for (Size i = n - 1; i >= 0; i--) {
					std::swap(arr[0], arr[i]);
					_ternary_heapify(arr, i, (Size)0, comp);
				}
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void ternary_heap_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				_ternary_heapsort(first, std::distance(first, last), comp);
			}
		}

		// Smooth Sort
		namespace _smoothsort {
			static const unsigned int leonardo[] =
			{
			   1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177, 287, 465, 753, 1219, 1973,
			   3193, 5167, 8361, 13529, 21891, 35421, 57313, 92735, 150049, 242785,
			   392835, 635621, 1028457, 1664079, 2692537, 4356617, 7049155, 11405773,
			   18454929, 29860703, 48315633, 78176337, 126491971, 204668309, 331160281,
			   535828591, 866988873, 1402817465, 2269806339u, 3672623805u,
			};

			template<typename RandomAccessIterator, class Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void smooth_sort_fix(RandomAccessIterator arr, size_t current_heap, int level_index, int levels[], Comp compare)
			{
				while (level_index > 0)
				{
					size_t prev_heap = current_heap - leonardo[levels[level_index]];
					if (compare(arr[current_heap], arr[prev_heap]))
					{
						if (levels[level_index] > 1)
						{
							size_t child_heap2 = current_heap - 1;
							size_t child_heap1 = child_heap2 - leonardo[levels[level_index] - 2];
							if (compare(arr[prev_heap], arr[child_heap1])
								|| compare(arr[prev_heap], arr[child_heap2]))
								break;
						}
						std::swap(arr[current_heap], arr[prev_heap]);
						current_heap = prev_heap;
						level_index -= 1;
					}
					else
						break;
				}

				int current_level = levels[level_index];
				while (current_level > 1)
				{
					size_t max_child = current_heap;
					size_t child_heap2 = current_heap - 1;
					size_t child_heap1 = child_heap2 - leonardo[current_level - 2];

					if (compare(arr[max_child], arr[child_heap1])) max_child = child_heap1;
					if (compare(arr[max_child], arr[child_heap2])) max_child = child_heap2;
					if (max_child == child_heap1)
					{
						std::swap(arr[current_heap], arr[child_heap1]);
						current_heap = child_heap1;
						current_level -= 1;
					}
					else if (max_child == child_heap2)
					{
						std::swap(arr[current_heap], arr[child_heap2]);
						current_heap = child_heap2;
						current_level -= 2;
					}
					else break;
				}
			}

			template<typename RandomAccessIterator, class Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void smooth_sort(RandomAccessIterator arr, size_t size, Comp compare)
			{
				int levels[64] = { 1 };
				int toplevel = 0;

				for (size_t i = 1; i < size; ++i)
				{
					if (toplevel > 0 && levels[toplevel - 1] - levels[toplevel] == 1)
					{
						toplevel -= 1;
						levels[toplevel] += 1;
					}
					else if (levels[toplevel] != 1)
					{
						toplevel += 1;
						levels[toplevel] = 1;
					}
					else
					{
						toplevel += 1;
						levels[toplevel] = 0;
					}
					smooth_sort_fix(arr, i, toplevel, levels, compare);
				}

				for (size_t i = size - 2; i > 0; --i)
				{
					if (levels[toplevel] <= 1)
					{
						toplevel -= 1;
					}
					else
					{
						levels[toplevel] -= 1;
						levels[toplevel + 1] = levels[toplevel] - 1;
						toplevel += 1;

						smooth_sort_fix(arr, i - leonardo[levels[toplevel]], toplevel - 1, levels, compare);
						smooth_sort_fix(arr, i, toplevel, levels, compare);
					}
				}
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void smooth_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				smooth_sort(first, (size_t)std::distance(first, last), comp);
			}
		}

		// Weak Heap Sort
		namespace _weaksort {
			unsigned char weakheap_getflag(unsigned char* r, size_t index)
			{
				return (r[index >> 3] >> (index & 7)) & 1;
			}

			template<typename RandomAccessIterator, class Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void weakheap_merge(
				unsigned char flags[], 
				RandomAccessIterator beg, RandomAccessIterator i, RandomAccessIterator j, 
				Comp compare
			)
			{
				if (compare(*i, *j))
				{
					flags[(j - beg) >> 3] ^= 1 << ((j - beg) & 7);
					std::swap(*i, *j);
				}
			}

			template<typename RandomAccessIterator, class Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void weakheap_sort(RandomAccessIterator beg, RandomAccessIterator end, Comp compare)
			{
				if (end - beg > 1)
				{
					size_t n = (size_t)(end - beg);
					unsigned char* flags = new unsigned char[(n + 7) / 8];
					for (size_t i = 0; i < n / 8; ++i)
						flags[i] = 0;
					for (size_t i = n - 1; i > 0; --i)
					{
						size_t j = i;
						while ((j & 1) == weakheap_getflag(flags, j >> 1))
							j >>= 1;
						weakheap_merge(flags, beg, beg + (j >> 1), beg + i, compare);
					}
					for (size_t i = n - 1; i > 1; --i)
					{
						std::swap(*beg, *(beg + i));
						size_t j = 1, k;
						while ((k = 2 * j + weakheap_getflag(flags, j)) < i)
							j = k;
						while (j > 0)
						{
							weakheap_merge(flags, beg, beg, beg + j, compare);
							j >>= 1;
						}
					}
					std::swap(*beg, *(beg + 1));
					delete[] flags;
				}
			}
		}


		// Slowest sorts
		namespace slowest {
			// See https://www.geeksforgeeks.org/the-slowest-sorting-algorithms/
			
			// Stooge Sort
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _inner_stoogesort(RandomAccessIterator arr, Size l, Size h, Comp comp)
			{
				// Base Case
				if (l >= h) return;

				// If first element is smaller than
				// last element, swap them
				if (comp(arr[h], arr[l])) std::swap(arr[l], arr[h]);

				// If there are more than 2 elements
				// in the array
				if (h - l + 1 > 2) {
					Size t = (h - l + 1) / 3;

					// Recursively sort the first
					// 2/3 elements
					_inner_stoogesort(arr, l, h - t, comp);

					// Recursively sort the last
					// 2/3 elements
					_inner_stoogesort(arr, l + t, h, comp);

					// Recursively sort the first
					// 2/3 elements again
					_inner_stoogesort(arr, l, h - t, comp);
				}
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void stooge_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				_inner_stoogesort(first, (MayanSort::ItSize<It>)0, std::distance(first, last) - 1, comp);
			}

			// Slow Sort
			template<typename RandomAccessIterator, typename Size, typename Comp>
			requires std::sortable<RandomAccessIterator, Comp>
			void _inner_slow_sort(RandomAccessIterator A, Size i, Size j, Comp comp)
			{
				// Base Case
				if (i >= j)
					return;

				// Middle value
				Size m = (i + j) / 2;

				// Recursively call with left half
				_inner_slow_sort(A, i, m, comp);

				// Recursively call with right half
				_inner_slow_sort(A, m + 1, j, comp);

				// Swap if first element
				// is lower than second
				if (comp(A[j], A[m])) {
					std::swap(A[j], A[m]);
				}

				// Recursively call with whole
				// array except maximum element
				_inner_slow_sort(A, i, j - 1, comp);
			}

			template<typename RandomAccessIterator, typename Compare>
			requires std::sortable<RandomAccessIterator, Compare>
			void slow_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
				_inner_slow_sort(first, (MayanSort::ItSize<It>)0, std::distance(first, last) - 1, comp);
			}
		}

		// Weave merge sort
		namespace _wvmergesort {
			template <typename BidIter, typename Compare>
			void _insertion_sort(BidIter first, BidIter last, Compare compare) {
				typedef typename std::iterator_traits<BidIter>::value_type T;
				if (first == last) return;

				for (BidIter cur = first + 1; cur != last; ++cur) {
					BidIter sift = cur;
					BidIter sift_1 = cur - 1;

					// Compare first so we can avoid 2 moves for
					// an element already positioned correctly.
					if (compare(*sift, *sift_1)) {
						T tmp = *sift;
						do {
							*sift-- = *sift_1;
						} while (sift != first && compare(tmp, *--sift_1));
						*sift = tmp;
					}
				}
			}

			template<typename InputIter, typename OutputIter>
			OutputIter _weave_merge(InputIter first, InputIter mid, InputIter last, OutputIter dest) {
				InputIter a = first, b = mid;
				OutputIter c = dest;
				bool flag = false;

				while (a < mid && b < last) {
					if (flag) *c++ = *a++;
					else *c++ = *b++;
					flag = !flag;
				}
				if (a < mid) c = std::copy(a, mid, c);
				if (b < last) c = std::copy(b, last, c);
				return c;
			}
                        template<typename BidIter, typename T, typename Compare>
                        void _merge_with_buffer(BidIter first, BidIter mid, BidIter last, T *buffer, Compare compare){
				T* end = _weave_merge(first, mid, last, buffer);
				_insertion_sort(buffer, end, compare);
				std::copy(buffer, end, first);
			}
				

			template<typename BidIter, typename T, typename Size, typename Compare>
			void wsort(BidIter first, BidIter last, Compare compare) {
				Size dis = std::distance(first, last);
				if (dis < 2) return;
				BidIter mid = std::next(first, dis / 2);
				wsort<BidIter, T, Size, Compare>(first, mid, compare);
				wsort<BidIter, T, Size, Compare>(mid, last, compare);

				_impl::_tmp_buffer<T, Size> buf_object(dis);
			    _merge_with_buffer(first, mid, last, buf_object.memory, compare);
				delete buf_object;
			}

		}




		template<typename RandomAccessIterator, typename Comp>
		void _bubbleSort(RandomAccessIterator begin, RandomAccessIterator end, Comp comp) {
			bool swapped = true;
			while (swapped) {
				swapped = false;
				for (RandomAccessIterator i = begin; i != end - 1; ++i) {
					if (comp(*(i + 1), *i)) {
						std::iter_swap(i, i + 1);
						swapped = true;
					}
				}
				--end;
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void _selection_sort(RandomAccessIterator begin, RandomAccessIterator end, Compare cmp) {
			for (RandomAccessIterator i = begin; i != end; ++i) {
				RandomAccessIterator min = i;
				for (RandomAccessIterator j = i; j != end; ++j) {
					if (cmp(*j, *min)) {
						min = j;
					}
				}
				std::iter_swap(i, min);
			}
		}

		template <typename RandomAccessIterator, typename Compare>
		void _binary_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
			for (RandomAccessIterator i = first + 1; i != last; ++i) {
				RandomAccessIterator const insertion_point = std::upper_bound(first, i, *i, comp);
				std::rotate(insertion_point, i, i + 1);
			}
		}


		template<typename T, typename RandomAccessIterator, typename Distance, typename Compare>
		void _inner_circleSort(RandomAccessIterator a, Distance n, Compare comp) {
			for (Distance i = 0; i < n - 1; i++) {
				for (Distance j = i + 1; j < n; j++) {
					if (comp(a[j], a[i])) {
						T temp = a[j];
						for (Distance k = j - 1; k >= i; k--) {
							a[k + 1] = a[k];
						}
						a[i] = temp;
					}
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void circle_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			typedef typename MayanSort::ItValue<RandomAccessIterator> T;
			_inner_circleSort<t>(first, std::distance(first, last), comp);
		}

		template<typename RandomAccessIterator, typename Distance, typename Compare>
		void _inner_gnomeSort(RandomAccessIterator a, Distance n, Compare comp) {
			Distance i = 0;
			while (i < n) {
				if (i == 0 || comp(a[i - 1], a[i])) {
					i++;
				}
				else {
					swap(a[i], a[i - 1]);
					i--;
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void gnome_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			_inner_gnomeSort(first, std::distance(first, last), comp);
		}

		/* function to sort arr using shellSort */
		template<typename T, typename RandomAccessIterator, typename Size, typename Comp>
		void _inner_shellSort(RandomAccessIterator arr, Size n, Comp comp) {
			// Start with a big gap, then reduce the gap
			for (Size gap = n / 2; gap > 0; gap /= 2) {
				// Do a gapped insertion sort for this gap size.
				// The first gap elements arr[0..gap-1] are already in gapped order
				// keep adding one more element until the entire array is
				// gap sorted
				for (Size i = gap; i < n; i += 1) {
					// add arr[i] to the elements that have been gap sorted
					// save arr[i] in temp and make a hole at position i
					T temp = arr[i];

					// shift earlier gap-sorted elements up until the correct
					// location for arr[i] is found
					Size j;
					for (j = i; j >= gap && comp(temp, arr[j - gap]); j -= gap) arr[j] = arr[j - gap];

					// put temp (the original arr[i]) in its correct location
					arr[j] = temp;
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void shell_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			typedef typename MayanSort::ItValue<RandomAccessIterator> T;
			_inner_shellSort<T>(first, std::distance(first, last), comp);
		}

		template<typename T, typename RandomAccessIterator, typename Size, typename Comp>
		void _patience_sort(RandomAccessIterator arr, Size n, Comp comp) {
			std::vector<std::vector<T>> piles;
			for (Size i = 0; i < n; i++) {
				T x = arr[i];
				bool found = false;
				for (auto& pile : piles) {
					if (!pile.empty() && comp(pile.back(), x)) {
						pile.push_back(x);
						found = true;
						break;
					}
				}
				if (!found) {
					piles.emplace_back();
					piles.back().push_back(x);
				}
			}
			Size i = 0;
			while (!piles.empty()) {
				auto min_it = piles.begin();
				for (auto it = piles.begin() + 1; it != piles.end(); ++it) {
					if (comp((*it).front(), (*min_it).front())) {
						min_it = it;
					}
				}
				arr[i++] = (*min_it).front();
				(*min_it).erase((*min_it).begin());
				if ((*min_it).empty()) {
					piles.erase(min_it);
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void patience_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			typedef typename MayanSort::ItValue<RandomAccessIterator> T;
			_patience_sort<T>(first, std::distance(first, last), comp);
		}

		// A function to sort the algorithm using Odd Even sort
		template<typename RandomAccessIterator, typename Size, typename Comp>
		void _inner_oddEvenSort(RandomAccessIterator arr, Size n, Comp comp) {
			bool isSorted = false; // Initially array is unsorted

			while (!isSorted)
			{
				isSorted = true;

				// Perform Bubble sort on odd indexed element
				for (Size i = 1; i <= n - 2; i = i + 2)
				{
					if (comp(arr[i + 1], arr[i]))
					{
						std::swap(arr[i], arr[i + 1]);
						isSorted = false;
					}
				}

				// Perform Bubble sort on even indexed element
				for (Size i = 0; i <= n - 2; i = i + 2)
				{
					if (comp(arr[i + 1], arr[i]))
					{
						std::swap(arr[i], arr[i + 1]);
						isSorted = false;
					}
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void odd_even_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			_inner_oddEvenSort(first, std::distance(first, last), comp);
		}

		template<typename RandomAccessIterator, typename Distance, typename Comp>
		void _inner_sillySort(RandomAccessIterator arr, Distance n, Comp comp) {
			for (Distance i = 0; i < n - 1; i++) {
				if (comp(arr[i + 1], arr[i])) {
					std::swap(arr[i], arr[i + 1]);
					i = -1;
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void silly_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			_inner_sillySort(first, std::distance(first, last), comp);
		}

		template<typename ForwardIt, typename Comp>
		void _double_select_sort(ForwardIt begin, ForwardIt end, Comp comp) {
			if (begin == end) return;    // length = 0
			ForwardIt right = begin;
			right++;
			if (right == end) return;    // length = 1

			ForwardIt half = begin;     // right=arr[1] half=arr[0]
			while (right != end) {
				right++; half++;
				if (right == end) break;
				right++;
			}                           // right=arr[len-1] half=arr[(int)len/2]
			// right = end
			// half  = halway between (rounded down: at the end half=left or half=left-1)


			ForwardIt left = begin;
			right = end;

			// keep looping until the two sorted sections meet
			// half is a ForwardIt to the middle element rounded down so if right misses left on odd len container it will hit half
			while (left != right && half != right) {
				ForwardIt min = left;      //one past last elt of the left sorted section
				ForwardIt max = left;
				ForwardIt temp = max;
				temp++;
				while (temp != right) {   //loop until max is one before first elt of right sorted section
					temp++;
					max++;
				}
				ForwardIt secondLast = max;

				// loop thru the unsorted array to find iterators to smallest and largest elements
				for (ForwardIt i = left; i != right; ++i) {
					if (comp(*i, *min)) min = i;           //if the element is smallest in unsorted array
					if (comp(*max, *i)) max = i;           //if the element is greatest in unsorted array
				}

				// checks to make sure you don't accidentally swap min away
				if (min == secondLast) {
					min = max;
				}
				right = secondLast;
				std::swap(*max, *right);       // used to have max != right-1 and min != left checks but they're a net computation cost
				std::swap(*min, *left++);
			}
		}

		template <typename ForwardIterator, typename StrictWeakOrdering>
		void _comb11(ForwardIterator first, ForwardIterator last, StrictWeakOrdering compare)
		{
			typedef typename std::iterator_traits<ForwardIterator>::difference_type difference_type;

			ForwardIterator iCurrent, iNext;
			difference_type length = std::distance(first, last);
			difference_type nSpace = length;

			for (bool bSwapped = false; (nSpace > 1) || bSwapped; )
			{
				nSpace = ((nSpace * 10) + 3) / 13; // Integer division is less than ideal.

				if ((nSpace == 9) || (nSpace == 10)) nSpace = 11;

				iCurrent = iNext = first;
				std::advance(iNext, nSpace);

				for (bSwapped = false; iNext != last; iCurrent++, iNext++)
				{
					if (compare(*iNext, *iCurrent))
					{
						std::iter_swap(iCurrent, iNext);
						bSwapped = true;
					}
				}
			}
		}

		template<typename sort_element_t, typename iter_t, typename compare_func_t>
		void _double_insert_sort(iter_t beg, iter_t end, compare_func_t comp)
		{
			if (end - beg > 1)
			{
				for (iter_t i = beg + 1, *j = end - 1; ; )
				{
					sort_element_t val = *i;
					iter_t t = i;
					for (; t > beg && comp(val, t[-1]); --t)
					{
						*t = t[-1];
					}
					*t = val;

					if (++i > j)
					{
						if (comp(*i, *j))
							++j;
						else
							break;
					}

					val = j[-1];
					t = j;
					for (; t < end && comp(*t, val); ++t)
					{
						t[-1] = *t;
					}
					t[-1] = val;

					if (i > --j)
					{
						if (comp(*j, j[-1]))
							--i;
						else
							break;
					}
				}
			}
		}

		template<typename RandomAccessIterator, typename Compare>
		void double_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
			typedef typename MayanSort::ItValue<RandomAccessIterator> T;
			_double_insert_sort<T>(first, last, comp);
		}
	}

}
