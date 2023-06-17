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

// From https://github.com/JamesQuintero/ShiftSort/blob/master/C%2B%2B/ShiftSort.hpp
// Modified by pysoft in 2023 to use in MayanSort.

// Original File Header:
/*
* Copyright (C) 2017 James Quintero
*/

#pragma once
#include <iostream>
#include <iterator>
#include <vector>

namespace MayanSort {
    namespace _shiftsort {
        template <class RandomAccessIterator, class Size, class Compare>
        void _merge(
            RandomAccessIterator array, 
            Size first_index, Size second_index, Size third_index, 
            Compare comp
        )
        {

            using Type = typename std::iterator_traits<RandomAccessIterator>::value_type;
            //if first_list>second_list, have second list be the list that gets merged into the first list
            if (second_index - first_index > third_index - second_index)
            {
                std::vector<Type> temp_2nd(third_index - second_index);
                Size counter = 0;
                for (Size y = second_index; y < third_index; y++)
                {
                    temp_2nd[counter] = array[y];
                    counter++;
                }

                //starts off at length of 2nd list
                Size second_counter = third_index - second_index;
                Size left = second_index - 1;
                while (second_counter > 0)
                {
                    //shift if left is greater than right
                    if (left >= first_index && comp(temp_2nd[second_counter - 1], array[left]))
                    {
                        array[left + second_counter] = array[left];
                        left--;
                    }
                    //add from 2nd if greater than left
                    else
                    {
                        array[left + second_counter] = temp_2nd[second_counter - 1];
                        second_counter--;
                    }
                }
                //delete[] temp_2nd;

            }
            else
            {
                std::vector<Type> temp_1st(third_index - second_index);
                Size counter = 0;
                for (Size y = first_index; y < second_index; y++)
                {
                    temp_1st[counter] = array[y];
                    counter++;
                }

                //starts off at length of 2nd list
                Size first_counter = 0;
                Size length = second_index - first_index;
                Size temp_length = second_index - first_index;
                Size right = second_index;
                while (first_counter < length)
                {
                    //shift if left is greater than right
                    if (right < third_index && comp(array[right], temp_1st[first_counter]))
                    {
                        array[right - temp_length] = array[right];
                        right++;
                    }
                    //add from 2nd if greater than left
                    else
                    {
                        array[right - temp_length] = temp_1st[first_counter];
                        first_counter++;
                        temp_length--;
                    }
                }

                //delete[] temp_1st;
            }


        }

        template <class RandomAccessIterator, class Size, class Compare>
        void _split(RandomAccessIterator array, Size* zero_indices, Size i, Size j, Compare comp)
        {
            //if have exactly 3 indices, then merge the 2 lists
            if ((j - i) == 2)
            {
                _merge(array, zero_indices[j], zero_indices[j - 1], zero_indices[i], comp);
                return;
            }
            //too few indices to know the bounds of the sublists in array
            else if ((j - i) < 2)
                return;

            //at this point, have >3 items, so split in half again
            Size new_j = i + (j - i) / 2;
            Size new_i = new_j + 1;

            //continues splitting first half
            _split(array, zero_indices, i, new_j, comp);
            //continues splitting second half
            _split(array, zero_indices, new_i, j, comp);

            //merges first half
            _merge(array, zero_indices[new_i], zero_indices[new_j], zero_indices[i], comp);
            //merges second half
            _merge(array, zero_indices[j], zero_indices[new_i], zero_indices[i], comp);
        }

        template <class RandomAccessIterator, class Size, class Compare>
        void _shiftsort_loop(RandomAccessIterator array, Size size, Compare comp)
        {

            Size temp = size / 2 + 2;
            //list of indices denoting the start of sorted sublists in array
            Size* zero_indices = new Size[temp];
            zero_indices[0] = size;

            Size end_tracker = 1;

            //start at end of the array because swapping for ascending order is easier
            for (Size x = size - 1; x >= 1; x--)
            {
                //if current element is out of order in respect to the element before it
                if (comp(array[x], array[x - 1]))
                {
                    //if both current element is out of order and the element before it, then we need to swap them
                    if (x > 1 && comp(array[x - 1], array[x - 2]))
                    {
                        //swaps array elements
                        //Type temp = array[x-2];
                        //array[x-2] = array[x];
                        //array[x] = temp;
                        std::swap(array[x - 2], array[x]);

                        //not at last index
                        if (x != size - 1)
                        {
                            //if previously traversed index is now out of order
                            if (comp(array[x + 1], array[x]))
                            {
                                zero_indices[end_tracker] = x + 1;
                                end_tracker++;
                            }
                        }

                    }
                    else
                    {
                        zero_indices[end_tracker] = x;
                        end_tracker++;
                    }
                    //skips an index
                    x--;
                }
            }
            zero_indices[end_tracker] = 0;

            //runs divide and conquer algorithm on the derivative index
            _split(array, zero_indices, (Size)0, end_tracker, comp);

            delete[] zero_indices;
        }


        template <class RandomAccessIterator, class Compare>
        void shiftsort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
            _shiftsort_loop(first, std::distance(first, last), comp);
        }
    }
}
