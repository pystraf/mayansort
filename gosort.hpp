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

// From https://github.com/golang/go/blob/dev.boringcrypto.go1.18/src/sort/sort.go
// Downloaded at May 13, 2023.
// Modified by pysoft in 2023 to use in MayanSort.

// Original File Header:
// Copyright 2009 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

//go:generate go run genzfunc.go

// Package sort provides primitives for sorting slices and user-defined collections
#include <utility>
#include "mayandef.hpp"

namespace MayanSort {
	namespace gosort {
		// insertionSort sorts data[a:b] using insertion sort.
		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _insertion_sort(RandomAccessIterator arr, Size a, Size b, Comp comp) {
			for (Size i = a + 1; i < b; i++)
				for (Size j = i; j > a && comp(arr[j], arr[j - 1]); j--) std::swap(arr[j], arr[j - 1]);
		}

		// siftDown implements the heap property on data[lo:hi].
        // first is an offset into the array where the root of the heap lies.
		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _heap_sift(RandomAccessIterator arr, Size lo, Size hi, Size first, Comp comp) {
			Size root = lo;
			while (true) {
				Size child = 2 * root + 1;
				if (child >= hi) break;
				if (child + 1 < hi && comp(arr[first + child], arr[first + child + 1])) child++;
				if (comp(arr[first + child], arr[first + root])) return;
				std::swap(arr[first + root], arr[first + child]);
				root = child;
			}
		}
		
		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _heap_sort(RandomAccessIterator arr, Size a, Size b, Comp comp) {
			Size first = a, lo = 0, hi = b - a;

			// Build heap with greatest element at top.
			for (Size i = (hi - 1) / 2; i >= 0; i--) _heap_sift(arr, i, hi, first, comp);

			// Pop elements, largest first, into end of data.
			for (Size i = hi - 1; i >= 0; i--) {
				std::swap(arr[first], arr[first + i]);
				_heap_sift(arr, lo, i, first, comp);
			}
		}

		// Quicksort, loosely following Bentley and McIlroy,
		// ``Engineering a Sort Function,'' SP&E November 1993.

		// medianOfThree moves the median of the three values data[m0], data[m1], data[m2] into data[m1].
		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _median_of_3(RandomAccessIterator arr, Size m1, Size m0, Size m2, Comp comp) {
			// sort 3 elements
			if (comp(arr[m1], arr[m0])) std::swap(arr[m1], arr[m0]);
			// data[m0] <= data[m1]
			if (comp(arr[m2], arr[m1])) {
				std::swap(arr[m2], arr[m1]);
				// data[m0] <= data[m2] && data[m1] < data[m2]
				if (comp(arr[m1], arr[m0])) std::swap(arr[m1], arr[m0]);
			}
			// now data[m0] <= data[m1] <= data[m2]
		}

		template<typename RandomAccessIterator, typename Size>
		void swap_range(RandomAccessIterator arr, Size a, Size b, Size n) {
			for (Size i = 0; i < n; i++) std::swap(arr[a + i], arr[b + i]);
		}

		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		std::pair<Size, Size> _do_pivot(RandomAccessIterator arr, Size lo, Size hi, Comp comp) {
			Size m = (lo + hi) >> 1;
			if (hi - lo > 40) {
				// Tukey's ``Ninther,'' median of three medians of three.
				Size s = (hi - lo) / 8;
				_median_of_3(arr, lo, lo + s, lo + 2 * s, comp);
				_median_of_3(arr, m, m - s, m + s, comp);
				_median_of_3(arr, hi - 1, hi - 1 - s, hi - 1 - 2 * s, comp);
			}
			_median_of_3(arr, lo, m, hi - 1, comp);

			// Invariants are:
			//	data[lo] = pivot (set up by ChoosePivot)
			//	data[lo < i < a] < pivot
			//	data[a <= i < b] <= pivot
			//	data[b <= i < c] unexamined
			//	data[c <= i < hi-1] > pivot
			//	data[hi-1] >= pivot
			Size pivot = lo, a = lo + 1, c = hi - 1;

			for (; a < c && comp(arr[a], arr[pivot]); a++);
			Size b = a;
			while (true) {
				for (; b < c && comp(arr[b], arr[pivot]); b++);
				for (; b < c && comp(arr[pivot], arr[c - 1]); c--);
				if (b >= c) break;
				// data[b] > pivot; data[c-1] <= pivot
				std::swap(arr[b], arr[c - 1]);
				b++;
				c--;
			}

			// If hi-c<3 then there are duplicates (by property of median of nine).
	        // Let's be a bit more conservative, and set border to 5.
			bool protect = hi - c < 5;
			if (!protect && hi - c < (hi - lo) / 4) {
				// Lets test some points for equality to pivot
				int dups = 0;
				if (comp(arr[hi - 1], arr[pivot])) { // data[hi-1] = pivot
					std::swap(arr[c], arr[hi - 1]);
					c++;
					dups++;
				}
				if (comp(arr[pivot], arr[b - 1])) {  // data[b-1] = pivot
					b--;
					dups++;
				}
				// m-lo = (hi-lo)/2 > 6
		        // b-lo > (hi-lo)*3/4-1 > 8
		        // ==> m < b ==> data[m] <= pivot
				if (comp(arr[pivot], arr[m])) {      // data[m] = pivot
					std::swap(arr[m], arr[b - 1]);
					b--;
					dups++;
				}
				// if at least 2 points are equal to pivot, assume skewed distribution
				protect = dups > 1;
			}

			if (protect) {
				// Protect against a lot of duplicates
				// Add invariant:
				//	data[a <= i < b] unexamined
				//	data[b <= i < c] = pivot
				while (true) {
					for (; a < b && comp(arr[pivot], arr[b - 1]); b--);
					for (; a < b && comp(arr[a], arr[pivot]); a++);
					if (a >= b) break;
					// data[a] == pivot; data[b-1] < pivot
					std::swap(arr[a], arr[b - 1]);
					a++;
					b--;
				}
			}
			// Swap pivot into middle
			std::swap(arr[pivot], arr[b - 1]);


			Size midlo = b - 1, midhi = c;
			return std::pair<Size, Size>(midlo, midhi);
		}

		// Do ShellSort pass with gap 6
		// It could be written in this simplified form cause b-a <= 12
		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _shell_sort(RandomAccessIterator arr, Size a, Size b, Comp comp) {
			for (Size i = a + 6; i < b; i++)
				if (comp(arr[i], arr[i - 6])) std::swap(arr[i], arr[i - 6]);
		}

		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _quick_sort_loop(RandomAccessIterator arr, Size a, Size b, Size maxDepth, Comp comp) {
			while (b - a > 12) {  // Use ShellSort for slices <= 12 elements
				if (maxDepth == 0) {
					_heap_sort(arr, a, b, comp);
					return;
				}

				maxDepth--;
				Size mlo, mhi;
				std::tie(mlo, mhi) = _do_pivot(arr, a, b, comp);
				// Avoiding recursion on the larger subproblem guarantees
		        // a stack depth of at most lg(b-a).
				if (mlo - a < b - mhi) {
					_quick_sort_loop(arr, a, mlo, maxDepth, comp);
					a = mhi;  // i.e., quickSort(data, mhi, b)
				}
				else {
					_quick_sort_loop(arr, mhi, b, maxDepth, comp);
					b = mlo;  // i.e., quickSort(data, a, mlo)
				}
			}
			if (b - a > 1) {
				// Do ShellSort pass with gap 6
		        // It could be written in this simplified form cause b-a <= 12
				_shell_sort(arr, a, b, comp);
				_insertion_sort(arr, a, b, comp);
			}
		}

		// maxDepth returns a threshold at which quicksort should switch
        // to heapsort. It returns 2*ceil(lg(n+1)).
		template<typename Size>
		Size _max_depth(Size n) {
			Size depth = 0;
			for (Size i = n; i > 0; i >>= 1) depth++;
			return depth * 2;
		}

		template<typename RandomAccessIterator, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void sort_unstable(RandomAccessIterator first, RandomAccessIterator last, Comp comp) {
			typedef typename MayanSort::ItSize<RandomAccessIterator> Size;
			Size sz = std::distance(first, last);
			_quick_sort_loop(first, (Size)0, sz, _max_depth(sz), comp);
		}

		// Notes on stable sorting:
		// The used algorithms are simple and provable correct on all input and use
		// only logarithmic additional stack space. They perform well if compared
		// experimentally to other stable in-place sorting algorithms.
		//
		// Remarks on other algorithms evaluated:
		//  - GCC's 4.6.3 stable_sort with merge_without_buffer from libstdc++:
		//    Not faster.
		//  - GCC's __rotate for block rotations: Not faster.
		//  - "Practical in-place mergesort" from  Jyrki Katajainen, Tomi A. Pasanen
		//    and Jukka Teuhola; Nordic Journal of Computing 3,1 (1996), 27-40:
		//    The given algorithms are in-place, number of Swap and Assignments
		//    grow as n log n but the algorithm is not stable.
		//  - "Fast Stable In-Place Sorting with O(n) Data Moves" J.I. Munro and
		//    V. Raman in Algorithmica (1996) 16, 115-160:
		//    This algorithm either needs additional 2n bits or works only if there
		//    are enough different elements available to encode some permutations
		//    which have to be undone later (so not stable on any input).
		//  - All the optimal in-place sorting/merging algorithms I found are either
		//    unstable or rely on enough different elements in each step to encode the
		//    performed block rearrangements. See also "In-Place Merging Algorithms",
		//    Denham Coates-Evely, Department of Computer Science, Kings College,
		//    January 2004 and the references in there.
		//  - Often "optimal" algorithms are optimal in the number of assignments
		//    but Interface has only Swap as operation.

		// Stable sorts data in ascending order as determined by the Less method,
		// while keeping the original order of equal elements.
		//
		// It makes one call to data.Len to determine n, O(n*log(n)) calls to
		// data.Less and O(n*log(n)*log(n)) calls to data.Swap.


		// rotate rotates two consecutive blocks u = data[a:m] and v = data[m:b] in data:
		// Data of the form 'x u v y' is changed to 'x v u y'.
		// rotate performs at most b-a many calls to data.Swap,
		// and it assumes non-degenerate arguments: a < m && m < b.
		template<typename RandomAccessIterator, typename Size>
		void rotate(RandomAccessIterator arr, Size a, Size m, Size b) {
			Size i = m - a, j = b - m;
			while (i != j)
				if (i > j) {
					swap_range(arr, m - i, m, j);
					i -= j;
				}
				else {
					swap_range(arr, m - i, m + j - i, i);
					j -= i;
				}
			swap_range(arr, m - i, m, i);
		}


		// symMerge merges the two sorted subsequences data[a:m] and data[m:b] using
		// the SymMerge algorithm from Pok-Son Kim and Arne Kutzner, "Stable Minimum
		// Storage Merging by Symmetric Comparisons", in Susanne Albers and Tomasz
		// Radzik, editors, Algorithms - ESA 2004, volume 3221 of Lecture Notes in
		// Computer Science, pages 714-723. Springer, 2004.
		//
		// Let M = m-a and N = b-n. Wolog M < N.
		// The recursion depth is bound by ceil(log(N+M)).
		// The algorithm needs O(M*log(N/M + 1)) calls to data.Less.
		// The algorithm needs O((M+N)*log(M)) calls to data.Swap.
		//
		// The paper gives O((M+N)*log(M)) as the number of assignments assuming a
		// rotation algorithm which uses O(M+N+gcd(M+N)) assignments. The argumentation
		// in the paper carries through for Swap operations, especially as the block
		// swapping rotate uses only O(M+N) Swaps.
		//
		// symMerge assumes non-degenerate arguments: a < m && m < b.
		// Having the caller check this condition eliminates many leaf recursion calls,
		// which improves performance.


		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _sym_merge(RandomAccessIterator arr, Size a, Size m, Size b, Comp comp) {
			// Avoid unnecessary recursions of symMerge
			// by direct insertion of data[a] into data[m:b]
			// if data[a:m] only contains one element.
			if (m - a == 1) {
				// Use binary search to find the lowest index i
				// such that data[i] >= data[a] for m <= i < b.
				// Exit the search loop with i == b in case no such index exists.
				Size i = m, j = b;
				while (i < j) {
					Size h = (i + j) / 2;
					if (comp(arr[h], arr[a])) i = h + 1;
					else j = h;
				}

				// Swap values until data[a] reaches the position before i.
				for (Size k = a; k < i - 1; k++) std::swap(arr[k], arr[k + 1]);
				return;
			}

			// Avoid unnecessary recursions of symMerge
			// by direct insertion of data[m] into data[a:m]
			// if data[m:b] only contains one element.
			if (b - m == 1) {
				// Use binary search to find the lowest index i
				// such that data[i] > data[m] for a <= i < m.
				// Exit the search loop with i == m in case no such index exists.
				Size i = a, j = m;
				while (i < j) {
					Size h = (i + j) / 2;
					if (comp(arr[h], arr[m])) i = h + 1;
					else j = h;
				}

				// Swap values until data[m] reaches the position i.
				for (Size k = m; k > i; k--) std::swap(arr[k], arr[k - 1]);
				return;
			}

			Size mid = (a + b) / 2;
			Size n = mid + m;
			Size start, r;
			if (m > mid) {
				start = n - b;
				r = mid;
			}
			else {
				start = a;
				r = m;
			}

			Size p = n - 1;
			while (start < r) {
				Size c = (start + r) >> 1;
				if (comp(arr[c], arr[p - c])) start = c + 1;
				else r = c;
			}

			Size end = n - start;
			if (start < m && m < end) rotate(arr, start, m, end);
			if (a < start && start < mid) _sym_merge(arr, a, start, mid, comp);
			if (mid < end && end < b) _sym_merge(arr, mid, end, b, comp);
		}

		template<typename RandomAccessIterator, typename Size, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void _stable(RandomAccessIterator arr, Size n, Comp comp) {
			Size blockSize = 20;
			Size a = 0, b = blockSize;
			while (b <= n) {
				_insertion_sort(arr, a, b, comp);
				a = b;
				b += 2 * blockSize;
			}
			_insertion_sort(arr, a, n, comp);

			while (blockSize < n) {
				a = 0;
				b = 2 * blockSize;
				while (b <= n) {
					_sym_merge(arr, a, a + blockSize, b, comp);
					a = b;
					b += 2 * blockSize;
				}
				Size m = a + blockSize;
				if (m < n) _sym_merge(arr, a, m, n, comp);
				blockSize *= 2;
			}
		}

		template<typename RandomAccessIterator, typename Comp>
		requires std::sortable<RandomAccessIterator, Comp>
		void sort_stable(RandomAccessIterator first, RandomAccessIterator last, Comp comp) {
			_stable(first, std::distance(first, last), comp);
		}

		/*
		Complexity of Stable Sorting
		Complexity of block swapping rotation
		Each Swap puts one new element into its correct, final position.
		Elements which reach their final position are no longer moved.
		Thus block swapping rotation needs |u|+|v| calls to Swaps.
		This is best possible as each element might need a move.
		Pay attention when comparing to other optimal algorithms which
		typically count the number of assignments instead of swaps:
		E.g. the optimal algorithm of Dudzinski and Dydek for in-place
		rotations uses O(u + v + gcd(u,v)) assignments which is
		better than our O(3 * (u+v)) as gcd(u,v) <= u.
		Stable sorting by SymMerge and BlockSwap rotations
		SymMerge complexity for same size input M = N:
		Calls to Less:  O(M*log(N/M+1)) = O(N*log(2)) = O(N)
		Calls to Swap:  O((M+N)*log(M)) = O(2*N*log(N)) = O(N*log(N))
		(The following argument does not fuzz over a missing -1 or
		other stuff which does not impact the final result).
		Let n = data.Len(). Assume n = 2^k.
		Plain merge sort performs log(n) = k iterations.
		On iteration i the algorithm merges 2^(k-i) blocks, each of size 2^i.
		Thus iteration i of merge sort performs:
		Calls to Less  O(2^(k-i) * 2^i) = O(2^k) = O(2^log(n)) = O(n)
		Calls to Swap  O(2^(k-i) * 2^i * log(2^i)) = O(2^k * i) = O(n*i)
		In total k = log(n) iterations are performed; so in total:
		Calls to Less O(log(n) * n)
		Calls to Swap O(n + 2*n + 3*n + ... + (k-1)*n + k*n)
		   = O((k/2) * k * n) = O(n * k^2) = O(n * log^2(n))
		Above results should generalize to arbitrary n = 2^k + p
		and should not be influenced by the initial insertion sort phase:
		Insertion sort is O(n^2) on Swap and Less, thus O(bs^2) per block of
		size bs at n/bs blocks:  O(bs*n) Swaps and Less during insertion sort.
		Merge sort iterations start at i = log(bs). With t = log(bs) constant:
		Calls to Less O((log(n)-t) * n + bs*n) = O(log(n)*n + (bs-t)*n)
		   = O(n * log(n))
		Calls to Swap O(n * log^2(n) - (t^2+t)/2*n) = O(n * log^2(n))
		*/
	}
}
