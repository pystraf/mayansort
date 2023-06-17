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

// Copied from https://github.com/MichaelAxtmann/DualPivotQuicksort/blob/master/include/dual_pivot_quicksort.hpp

// Original File Header: 
/*
 * Copyright (c) 2009, 2016, Oracle and/or its affiliates. All rights reserved.
 * Copyright (c) 2019 Michael Axtmann <michael.axtmann@gmail.com>
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

 // This is a modified file from
 // http://hg.openjdk.java.net/jdk/jdk/raw-file/tip/src/java.base/share/classes/java/util/DualPivotQuicksort.java
 // downloaded at the 7th of November 2019.

#pragma once

#include <iterator>
#include <cstddef>

namespace MayanSort {
    namespace dual_pivot_quicksort {


        /**
         * Dual-Pivot Quicksort.
         */
        template<class Iterator, class Comp = std::less<>>
        void sort(Iterator begin, Iterator end, Comp comp = Comp{});
        /**
         * If the length of an array to be sorted is less than this
         * constant, insertion sort is used in preference to Quicksort.
         */
        static size_t INSERTION_SORT_THRESHOLD = 47;

        /**
         * Sorts the specified range of the array by Dual-Pivot Quicksort.
         *
         * @param a the array to be sorted
         * @param left the index of the first element, inclusive, to be sorted
         * @param right the index of the last element, inclusive, to be sorted
         * @param leftmost indicates if this part is the leftmost in the range
         */
        template<class Iterator, class Comp>
        void sort_rec(Iterator a, ptrdiff_t left, ptrdiff_t right, bool leftmost, Comp comp) {
            using T = typename std::iterator_traits<Iterator>::value_type;
            ptrdiff_t length = right - left + 1;

            // Use insertion sort on tiny arrays
            if (length < INSERTION_SORT_THRESHOLD) {
                if (leftmost) {
                    /*
                     * Traditional (without sentinel) insertion sort,
                     * optimized for server VM, is used in case of
                     * the leftmost part.
                     */
                    for (ptrdiff_t i = left, j = i; i < right; j = ++i) {
                        T ai = std::move(a[i + 1]);
                        while (comp(ai, a[j])) {
                            a[j + 1] = std::move(a[j]);
                            if (j-- == left) {
                                break;
                            }
                        }
                        a[j + 1] = std::move(ai);
                    }
                }
                else {
                    /*
                     * Skip the longest ascending sequence.
                     */
                    do {
                        if (left >= right) {
                            return;
                        }
                        ++left;
                    } while (!comp(a[left], a[left - 1]));

                    /*
                     * Every element from adjoining part plays the role
                     * of sentinel, therefore this allows us to avoid the
                     * left range check on each iteration. Moreover, we use
                     * the more optimized algorithm, so called pair insertion
                     * sort, which is faster (in the context of Quicksort)
                     * than traditional implementation of insertion sort.
                     */
                    for (ptrdiff_t k = left; ++left <= right; k = ++left) {
                        T a1 = a[k], a2 = a[left];

                        if (comp(a1, a2)) {
                            a2 = std::move(a1); a1 = std::move(a[left]);
                        }
                        while (comp(a1, a[--k])) {
                            a[k + 2] = std::move(a[k]);
                        }
                        a[++k + 1] = std::move(a1);

                        while (comp(a2, a[--k])) {
                            a[k + 1] = std::move(a[k]);
                        }
                        a[k + 1] = std::move(a2);
                    }
                    T last = std::move(a[right]);

                    while (comp(last, a[--right])) {
                        a[right + 1] = std::move(a[right]);
                    }
                    a[right + 1] = std::move(last);
                }
                return;
            }

            // Inexpensive approximation of length / 7
            ptrdiff_t seventh = (length >> 3) + (length >> 6) + 1;

            /*
             * Sort five evenly spaced elements around (and including) the
             * center element in the range. These elements will be used for
             * pivot selection as described below. The choice for spacing
             * these elements was empirically determined to work well on
             * a wide variety of inputs.
             */
            ptrdiff_t e3 = (left + right) >> 1; // The midpoptrdiff_t // todo correct?
            ptrdiff_t e2 = e3 - seventh;
            ptrdiff_t e1 = e2 - seventh;
            ptrdiff_t e4 = e3 + seventh;
            ptrdiff_t e5 = e4 + seventh;

            // Sort these elements using insertion sort
            if (comp(a[e2], a[e1])) { T t = std::move(a[e2]); a[e2] = std::move(a[e1]); a[e1] = std::move(t); }

            if (comp(a[e3], a[e2])) {
                T t = std::move(a[e3]); a[e3] = std::move(a[e2]); a[e2] = t;
                if (comp(t, a[e1])) { a[e2] = std::move(a[e1]); a[e1] = std::move(t); }
            }
            if (comp(a[e4], a[e3])) {
                T t = std::move(a[e4]); a[e4] = std::move(a[e3]); a[e3] = t;
                if (comp(t, a[e2])) {
                    a[e3] = std::move(a[e2]); a[e2] = t;
                    if (comp(t, a[e1])) { a[e2] = std::move(a[e1]); a[e1] = std::move(t); }
                }
            }
            if (comp(a[e5], a[e4])) {
                T t = std::move(a[e5]); a[e5] = std::move(a[e4]); a[e4] = t;
                if (comp(t, a[e3])) {
                    a[e4] = std::move(a[e3]); a[e3] = t;
                    if (comp(t, a[e2])) {
                        a[e3] = std::move(a[e2]); a[e2] = t;
                        if (comp(t, a[e1])) { a[e2] = std::move(a[e1]); a[e1] = std::move(t); }
                    }
                }
            }

            // Poptrdiff_ters
            ptrdiff_t less = left;  // The index of the first element of center part
            ptrdiff_t great = right; // The index before the first element of right part

            if (a[e1] != a[e2] && a[e2] != a[e3] && a[e3] != a[e4] && a[e4] != a[e5]) {
                /*
                 * Use the second and fourth of the five sorted elements as pivots.
                 * These values are inexpensive approximations of the first and
                 * second terciles of the array. Note that pivot1 <= pivot2.
                 */
                T pivot1 = a[e2];
                T pivot2 = a[e4];

                /*
                 * The first and the last elements to be sorted are moved to the
                 * locations formerly occupied by the pivots. When partitioning
                 * is complete, the pivots are swapped back ptrdiff_to their final
                 * positions, and excluded from subsequent sorting.
                 */
                a[e2] = std::move(a[left]);
                a[e4] = std::move(a[right]);

                /*
                 * Skip elements, which are less or greater than pivot values.
                 */
                while (comp(a[++less], pivot1));
                while (comp(pivot2, a[--great]));

                /*
                 * Partitioning:
                 *
                 *   left part           center part                   right part
                 * +--------------------------------------------------------------+
                 * |  < pivot1  |  pivot1 <= && <= pivot2  |    ?    |  > pivot2  |
                 * +--------------------------------------------------------------+
                 *               ^                          ^       ^
                 *               |                          |       |
                 *              less                        k     great
                 *
                 * Invariants:
                 *
                 *              all in (left, less)   < pivot1
                 *    pivot1 <= all in [less, k)     <= pivot2
                 *              all in (great, right) > pivot2
                 *
                 * Poptrdiff_ter k is the first index of ?-part.
                 */
                 // outer:
                for (ptrdiff_t k = less - 1; ++k <= great; ) {
                    T ak = a[k];
                    if (comp(ak, pivot1)) { // Move a[k] to left part
                        a[k] = std::move(a[less]);
                        /*
                         * Here and below we use "a[i] = b; i++;" instead
                         * of "a[i++] = b;" due to performance issue.
                         */
                        a[less] = std::move(ak);
                        ++less;
                    }
                    else if (comp(pivot2, ak)) { // Move a[k] to right part
                        while (comp(pivot2, a[great])) {
                            if (great-- == k) {
                                // break outer;
                                goto outer1;
                            }
                        }
                        if (comp(a[great], pivot1)) { // a[great] <= pivot2
                            a[k] = std::move(a[less]);
                            a[less] = std::move(a[great]);
                            ++less;
                        }
                        else { // pivot1 <= a[great] <= pivot2
                            a[k] = std::move(a[great]);
                        }
                        /*
                         * Here and below we use "a[i] = b; i--;" instead
                         * of "a[i--] = b;" due to performance issue.
                         */
                        a[great] = std::move(ak);
                        --great;
                    }
                }
            outer1:

                // Swap pivots ptrdiff_to their final positions
                a[left] = std::move(a[less - 1]); a[less - 1] = pivot1;
                a[right] = std::move(a[great + 1]); a[great + 1] = pivot2;

                // Sort left and right parts recursively, excluding known pivots
                sort_rec(a, left, less - 2, leftmost, comp);
                sort_rec(a, great + 2, right, false, comp);

                /*
                 * If center part is too large (comprises > 4/7 of the array),
                 * swap ptrdiff_ternal pivot values to ends.
                 */
                if (less < e1 && e5 < great) {
                    /*
                     * Skip elements, which are equal to pivot values.
                     */
                    while (a[less] == pivot1) {
                        ++less;
                    }

                    while (a[great] == pivot2) {
                        --great;
                    }

                    /*
                     * Partitioning:
                     *
                     *   left part         center part                  right part
                     * +----------------------------------------------------------+
                     * | == pivot1 |  pivot1 < && < pivot2  |    ?    | == pivot2 |
                     * +----------------------------------------------------------+
                     *              ^                        ^       ^
                     *              |                        |       |
                     *             less                      k     great
                     *
                     * Invariants:
                     *
                     *              all in (*,  less) == pivot1
                     *     pivot1 < all in [less,  k)  < pivot2
                     *              all in (great, *) == pivot2
                     *
                     * Poptrdiff_ter k is the first index of ?-part.
                     */
                     // outer:
                    for (ptrdiff_t k = less - 1; ++k <= great; ) {
                        T ak = a[k];
                        if (ak == pivot1) { // Move a[k] to left part
                            a[k] = std::move(a[less]);
                            a[less] = std::move(ak);
                            ++less;
                        }
                        else if (ak == pivot2) { // Move a[k] to right part
                            while (a[great] == pivot2) {
                                if (great-- == k) {
                                    // break outer;
                                    goto outer2;
                                }
                            }
                            if (a[great] == pivot1) { // a[great] < pivot2
                                a[k] = std::move(a[less]);
                                /*
                                 * Even though a[great] equals to pivot1, the
                                 * assignment a[less] = pivot1 may be incorrect,
                                 * if a[great] and pivot1 are floating-poptrdiff_t zeros
                                 * of different signs. Therefore in float and
                                 * double sorting methods we have to use more
                                 * accurate assignment a[less] = a[great].
                                 */
                                a[less] = pivot1;
                                ++less;
                            }
                            else { // pivot1 < a[great] < pivot2
                                a[k] = std::move(a[great]);
                            }
                            a[great] = std::move(ak);
                            --great;
                        }
                    }
                }
            outer2:

                // Sort center part recursively
                sort_rec(a, less, great, false, comp);

            }
            else { // Partitioning with one pivot
           /*
            * Use the third of the five sorted elements as pivot.
            * This value is inexpensive approximation of the median.
            */
                T pivot = a[e3];

                /*
                 * Partitioning degenerates to the traditional 3-way
                 * (or "Dutch National Flag") schema:
                 *
                 *   left part    center part              right part
                 * +-------------------------------------------------+
                 * |  < pivot  |   == pivot   |     ?    |  > pivot  |
                 * +-------------------------------------------------+
                 *              ^              ^        ^
                 *              |              |        |
                 *             less            k      great
                 *
                 * Invariants:
                 *
                 *   all in (left, less)   < pivot
                 *   all in [less, k)     == pivot
                 *   all in (great, right) > pivot
                 *
                 * Poptrdiff_ter k is the first index of ?-part.
                 */
                for (ptrdiff_t k = less; k <= great; ++k) {
                    if (a[k] == pivot) {
                        continue;
                    }
                    T ak = a[k];
                    if (comp(ak, pivot)) { // Move a[k] to left part
                        a[k] = std::move(a[less]);
                        a[less] = std::move(ak);
                        ++less;
                    }
                    else { // a[k] > pivot - Move a[k] to right part
                        while (comp(pivot, a[great])) {
                            --great;
                        }
                        if (comp(a[great], pivot)) { // a[great] <= pivot
                            a[k] = std::move(a[less]);
                            a[less] = std::move(a[great]);
                            ++less;
                        }
                        else { // a[great] == pivot
                       /*
                        * Even though a[great] equals to pivot, the
                        * assignment a[k] = pivot may be incorrect,
                        * if a[great] and pivot are floating-poptrdiff_t
                        * zeros of different signs. Therefore in float
                        * and double sorting methods we have to use
                        * more accurate assignment a[k] = a[great].
                        */
                            a[k] = pivot;
                        }
                        a[great] = std::move(ak);
                        --great;
                    }
                }

                /*
                 * Sort left and right parts recursively.
                 * All elements from center part are equal
                 * and, therefore, already sorted.
                 */
                sort_rec(a, left, less - 1, leftmost, comp);
                sort_rec(a, great + 1, right, false, comp);
            }
        }


        template<class Iterator, class Comp>
        void sort(Iterator begin, Iterator end, Comp comp) {
            if (begin == end) return;
            sort_rec(begin, 0, (end - begin) - 1, true, comp);
        }

    } // namespace dual_pivot_quicksort
}