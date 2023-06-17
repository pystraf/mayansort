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

// From https://github.com/kaybee1928/ARoot-Sort/blob/master/ARootSort/ARootSort
// Modified by pysoft in 2023 to use in MayanSort.

// Original File Header:

/*
* * * * * * * * * * * * * * * * * * *
*       Author : Ganzaki            *
*                                   *
*       language : c++              *
*                                   *
* * * * * * * * * * * * * * * * * * *
*/

#include <cmath>
#include <vector>
#include <utility>
#include "mayandef.hpp"


namespace MayanSort {
    namespace ksb
    {

        //arrayReverse is used to reverse array element from index start to end
        template<typename RandomAccessIterator, typename Size>
        void _array_reverse(RandomAccessIterator array, Size start, Size end)
        {
            Size middle = start + (end - start) / 2;
            Size temp = start + end - 1;
            for (Size iterator = start; iterator < middle; ++iterator)
                std::swap(array[iterator], array[temp - iterator]);
        }

        //To merger array[start to middle] and array[middle to end] in ascending order
        template<typename T, typename RandomAccessIterator, typename Size, typename Comp>
        void _merge(RandomAccessIterator array, Size start, Size middle, Size end, Comp comp)
        {
            Size first_array_size = middle - start;

            T* temp_array = new T[first_array_size];

            for (Size iterator = 0; iterator < first_array_size; ++iterator)
                temp_array[iterator] = array[start + iterator];

            Size temp_array_iterator = 0;
            Size second_array_iterator = middle;
            Size array_iterator = start;

            while ((temp_array_iterator < first_array_size) &&
                (second_array_iterator < end))
            {

                if (comp(temp_array[temp_array_iterator], array[second_array_iterator]))
                {
                    array[array_iterator] = temp_array[temp_array_iterator];

                    array_iterator++;
                    temp_array_iterator++;

                }

                else
                {
                    array[array_iterator] = array[second_array_iterator];

                    array_iterator++;
                    second_array_iterator++;
                }

            }
            while (temp_array_iterator < first_array_size)
            {
                array[array_iterator] = temp_array[temp_array_iterator];

                array_iterator++;
                temp_array_iterator++;
            }
            delete[] temp_array;
        }

        //Merger version 1 is used to sort array which are mostly sorted
        template<typename T, typename RandomAccessIterator, typename Size, typename Comp>
        void _merge_mostly_sorted(RandomAccessIterator array, Size start, Size end, Comp comp)
        {
            Size merge_iterator_start = start;
            Size merge_iterator_middle = start;
            Size merge_iterator_end = start + 1;

            bool isMerge = false;

            Size number_of_sorted_subarray = 0;

            while (true)
            {
                while ((merge_iterator_end < end) &&
                    !comp(array[merge_iterator_end], array[merge_iterator_end - 1]))

                    merge_iterator_end++;

                if (isMerge)
                {

                    _merge<T>(array, merge_iterator_start,
                        merge_iterator_middle,
                        merge_iterator_end, comp);

                    merge_iterator_start = merge_iterator_end;
                    merge_iterator_end++;

                    isMerge = false;

                }
                else
                {

                    isMerge = true;

                    merge_iterator_middle = merge_iterator_end;
                    merge_iterator_end++;

                    number_of_sorted_subarray++;

                    if (merge_iterator_middle >= end)
                    {
                        if (number_of_sorted_subarray < 2)
                            break;

                        else
                        {
                            number_of_sorted_subarray = 0;

                            merge_iterator_start = start;
                            merge_iterator_middle = start;
                            merge_iterator_end = start + 1;

                            isMerge = false;
                        }
                    }
                }
            }
        }

        //Merger version 2 is used to sort array which are less started
        template<typename T, typename RandomAccessIterator, typename Size, typename Comp>
        void _merge_less_started(RandomAccessIterator array, Size start, Size end, Comp comp)
        {

            Size merge_iterator_start = 0;
            Size merge_iterator_middle = 0;
            Size merge_iterator_end = 0;

            Size step_counter = 1;

            std::vector<Size> sorted_subarray_end_points;

            sorted_subarray_end_points.push_back((Size)0);

            for (Size iterator = start + 1; iterator < end; ++iterator)
            {
                while ((iterator < end) && !comp(array[iterator], array[iterator - 1]))
                    iterator++;

                sorted_subarray_end_points.push_back(iterator);
            }

            if (sorted_subarray_end_points[sorted_subarray_end_points.size() - 1] != end)
                sorted_subarray_end_points.push_back(end);

            Size number_of_sorted_subarray = sorted_subarray_end_points.size() - 1;

            while (true)
            {
                merge_iterator_start = 0;
                merge_iterator_middle = merge_iterator_start + step_counter;
                merge_iterator_end = std::min(merge_iterator_middle + step_counter,
                    number_of_sorted_subarray);

                if (merge_iterator_middle >= number_of_sorted_subarray)
                    break;

                while (merge_iterator_middle < number_of_sorted_subarray)
                {

                    _merge<T>(array, sorted_subarray_end_points[merge_iterator_start],
                        sorted_subarray_end_points[merge_iterator_middle],
                        sorted_subarray_end_points[merge_iterator_end], comp);

                    merge_iterator_start = merge_iterator_end;
                    merge_iterator_middle = merge_iterator_start + step_counter;
                    merge_iterator_end = std::min(merge_iterator_middle + step_counter,
                        number_of_sorted_subarray);

                }

                step_counter = step_counter * 2;

            }
        }


        //Function that reverses large enough decreasingly sorted subarray and
        //decides if array has to go through version 1 or 2 of merger
        template<typename T, typename RandomAccessIterator, typename Size, typename Comp>
        void _aroot_sort_loop(RandomAccessIterator array, Size start, Size end, Comp comp)
        {

            Size array_iterator_1 = 0;
            Size array_iterator_2 = start + 1;


            //It is not proved to be optimal, just intuitively taken

            Size optimal_near_sorted_parameter = (Size)(std::log(end - start) / std::log(2));

            Size number_of_decreasing_subarray = 0;


            for (Size iterator = start + 1; iterator < end; ++iterator)
            {

                while ((array_iterator_2 < end) &&
                    !comp(array[array_iterator_2 - 1], array[array_iterator_2]))

                    array_iterator_2++;

                if (array_iterator_2 - array_iterator_1 > 1)
                {
                    _array_reverse(array, array_iterator_1, array_iterator_2);

                    number_of_decreasing_subarray++;
                }

                array_iterator_1 = array_iterator_2;

            }

            if (number_of_decreasing_subarray <= optimal_near_sorted_parameter)
                _merge_mostly_sorted<T>(array, start, end, comp);
            else
                _merge_less_started<T>(array, start, end, comp);
        }

        template<typename RandomAccessIterator, typename Comp>
        void aroot_sort(RandomAccessIterator first, RandomAccessIterator last, Comp comp) {
            typedef typename MayanSort::ItValue<RandomAccessIterator> T;
            typedef typename MayanSort::ItSize<RandomAccessIterator> Size;
            _aroot_sort_loop<T>(first, (Size)0, std::distance(first, last), comp);
        }


    }
}