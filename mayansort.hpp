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

// mayansort.hpp: Main file.


#include "mayandef.hpp"
#include <algorithm>
#include <random>

#include "grailsort.hpp"
#include "wikisort.h"
#include "pdqsort.hpp"
#include "quick_merge_sort.hpp"
#include "drop_merge_sort.hpp"
#include "timsort.hpp"
#include "vergesort.hpp"
#include "dual_pivot_quicksort.hpp"
#include "poplar.hpp"
#include "gosort.hpp"
#include "shiftsort.hpp"
#include "stlib.hpp"
#include "indiesort.hpp"
#include "nanosort.hpp"
#include "aroot.hpp"
#include "hayate.hpp"
#include "sqrtsort.hpp"

#include "mayanimpl.hpp"

namespace MayanSort {
    
    // Intro sort (unstable)
    _SortTpl _SortHead IntroSort(It first, It last, Comp comp) {
        std::sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead IntroSort(It first, It last) {
        _CompD;
        IntroSort<It, Compare>(first, last, Compare());
    }

    // Bottom-up merge sort (stable)
    _SortTpl _SortHead MergeSortBottomUp(It first, It last, Comp comp) {
        std::stable_sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead MergeSortBottomUp(It first, It last) {
        _CompD;
        MergeSortBottomUp<It, Compare>(first, last, Compare());
    }

    // Wikisort (stable)
    // Implementation: https://github.com/BonzaiThePenguin/WikiSort/blob/master/WikiSort.cpp
    // See the wikisort.h file.
    _SortTpl _SortHead WikiSort(It first, It last, Comp comp) {
        Wiki::Sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead WikiSort(It first, It last) {
        _CompD;
        WikiSort<It, Compare>(first, last, Compare());
    }

    // Pattern-defating quicksort (unstable)
    // Implementation: https://github.com/orlp/pdqsort/blob/master/pdqsort.h
    // See the pdqsort.hpp file.
    _SortTpl _SortHead PDQSort(It first, It last, Comp comp) {
        pdqsort(first, last, comp);
    }

    _SortTplD _SortHead PDQSort(It first, It last) {
        _CompD;
        PDQSort<It, Compare>(first, last, Compare());
    }

    _SortTpl _SortHead PDQSortBranchless(It first, It last, Comp comp) {
        pdqsort_branchless(first, last, comp);
    }

    _SortTplD _SortHead PDQSortBranchless(It first, It last) {
        _CompD;
        PDQSort<It, Compare>(first, last, Compare());
    }

    // GrailSort (stable)
    // Implementation: https://github.com/HolyGrailSortProject/Rewritten-Grailsort/blob/master/C%2B%2B/Morwenn's%20rewrite%20of%20Summer%20Dragonfly's%20GrailSort/grailsort.h
    // See the grailsort.hpp file.
    _SortTpl _SortHead GrailSort(It first, It last, Comp comp) {
        grailsort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead GrailSort(It first, It last) {
        _CompD;
        GrailSort<It, Compare>(first, last, Compare());
    }


    // Quick Merge Sort (unstable)
    // Implementation: https://github.com/Morwenn/quick_merge_sort/blob/trunk/quick_merge_sort.h%2B%2B
    // See the quick_merge_sort.hpp file.
    _SortTpl _SortHead QuickMergeSort(It first, It last, Comp comp) {
        quick_merge_sort<It, Comp>(first, last, distance(first, last), comp);
    }

    _SortTplD _SortHead QuickMergeSort(It first, It last) {
        _CompD;
        QuickMergeSort<It, Compare>(first, last, Compare());
    }

    // Merge Sort (stable)
    // Implementation by myself
    _SortTpl _SortHead MergeSort(It first, It last, Comp comp) {
        ItSize<It> size = std::distance(first, last);
        if (size < 2) return;
        It mid = first + size / 2;
        MergeSort(first, mid, comp);
        MergeSort(mid, last, comp);
        std::inplace_merge<It, Comp>(first, mid, last, comp);
    }

    _SortTplD _SortHead MergeSort(It first, It last) {
        _CompD;
        MergeSort<It, Compare>(first, last, Compare());
    }

    // Drop Merge Sort (stable)
    // Implementation: https://github.com/adrian17/cpp-drop-merge-sort/blob/master/drop_merge_sort.hpp
    _SortTpl _SortHead DropMergeSort(It first, It last, Comp comp) {
        dmsort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead DropMergeSort(It first, It last) {
        _CompD;
        DropMergeSort<It, Compare>(first, last, Compare());
    }

    // Quick Sort (unstable)
    // Implementation: https://blog.csdn.net/chuanzhouxiao/article/details/85490874


    _SortTpl _SortHead QuickSort(It first, It last, Comp comp) {
        vergesort::detail::quicksort(first, last, std::distance(first, last), comp);
    }

    _SortTplD _SortHead QuickSort(It first, It last) {
        _CompD;
        QuickSort<It, Compare>(first, last, Compare());
    }


    // Lazy Stable Sort (stable)
    // Implementation: https://github.com/HolyGrailSortProject/Rewritten-Grailsort/blob/master/C%2B%2B/Morwenn's%20rewrite%20of%20Summer%20Dragonfly's%20GrailSort/grailsort.h
    // See the grailsort.hpp file.
    _SortTpl _SortHead LazyStableSort(It first, It last, Comp comp) {
        grailsort_detail::GrailSort sorter;
        typedef grailsort_detail::ThreeWayCompare<Comp> CompFunctor;
        CompFunctor compare(std::move(comp));
        sorter.LazyStableSort<It, CompFunctor>(first, 0, (int)std::distance(first, last), compare);
    }

    _SortTplD _SortHead LazyStableSort(It first, It last) {
        _CompD;
        LazyStableSort<It, Compare>(first, last, Compare());
    }

    // TimSort (stable)
    // Implementation: https://github.com/timsort/cpp-TimSort/blob/master/include/gfx/timsort.hpp
    // See the timsort.hpp file.
    _SortTpl _SortHead TimSort(It first, It last, Comp comp) {
        gfx::timsort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead TimSort(It first, It last) {
        _CompD;
        TimSort<It, Compare>(first, last, Compare());
    }

    // GoSort
    // Implementation: https://github.com/golang/go/blob/dev.boringcrypto.go1.18/src/sort/sort.go

    // Rewritten for C++

    _SortTpl _SortHead GoSort(It first, It last, Comp comp) {
        gosort::sort_unstable(first, last, comp);
    }

    _SortTplD _SortHead GoSort(It first, It last) {
        _CompD;
        GoSort<It, Compare>(first, last, Compare());
    }

    _SortTpl _SortHead GoStableSort(It first, It last, Comp comp) {
        gosort::sort_stable(first, last, comp);
    }

    _SortTplD _SortHead GoStableSort(It first, It last) {
        _CompD;
        GoStableSort<It, Compare>(first, last, Compare());
    }

    // Dual Pivot Quick Sort
    // Implementation: https://www.geeksforgeeks.org/dual-pivot-quicksort/

    _SortTpl _SortHead QuickSortDualPivot(It first, It last, Comp comp) {
        _impl::_dualsort::dqsort(first, last, comp);
    }

    _SortTplD _SortHead QuickSortDualPivot(It first, It last) {
        _CompD;
        QuickSortDualPivot<It, Compare>(first, last, Compare());
    }

    // Bubble sort
    // Implementation by myself.

    _SortTpl _SortHead BubbleSort(It first, It last, Comp comp) {
        _impl::_bubbleSort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead BubbleSort(It first, It last) {
        _CompD;
        BubbleSort<It, Compare>(first, last, Compare());
    }

    // Selection Sort
    // Implementation by myself.

    _SortTpl _SortHead SelectionSort(It first, It last, Comp comp) {
        _impl::_selection_sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead SelectionSort(It first, It last) {
        _CompD;
        SelectionSort<It, Compare>(first, last, Compare());
    }


    // Insertion Sort
    // Implementation by myself.
    _SortTpl _SortHead InsertSort(It first, It last, Comp comp) {
        InsertionSort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead InsertSort(It first, It last) {
        _CompD;
        InsertSort<It, Compare>(first, last, Compare());
    }

    // Binary Insertion Sort
    // Implementation by myself.

    _SortTpl _SortHead InsertSortBinary(It first, It last, Comp comp) {
        _impl::_binary_insertion_sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead InsertSortBinary(It first, It last) {
        _CompD;
        InsertSortBinary<It, Compare>(first, last, Compare());
    }


    // VergeSort
    // Implementation: https://github.com/Morwenn/vergesort/blob/master/vergesort.h
    // See the vergesort.hpp file.
    _SortTpl _SortHead VergeSort(It first, It last, Comp comp) {
        vergesort::vergesort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead VergeSort(It first, It last) {
        _CompD;
        VergeSort<It, Compare>(first, last, Compare());
    }

    // Dual Pivot Quick Sort (Fast)
    // Implementation: https://github.com/MichaelAxtmann/DualPivotQuicksort/blob/master/include/dual_pivot_quicksort.hpp
    // See the dual_pivot_quicksort.hpp file.
    _SortTpl _SortHead QuickSortDualPivotFast(It first, It last, Comp comp) {
        dual_pivot_quicksort::sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead QuickSortDualPivotFast(It first, It last) {
        _CompD;
        QuickSortDualPivotFast<It, Compare>(first, last, Compare());
    }

    // Poplar Heap Sort (unstable)
    // Implementation: https://github.com/Morwenn/poplar-heap/blob/master/poplar.h
    // See the poplar.hpp file.
    _SortTpl _SortHead PoplarHeapSort(It first, It last, Comp comp) {
        poplar::make_heap<It, Comp>(first, last, comp);
        poplar::sort_heap<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead PoplarHeapSort(It first, It last) {
        _CompD;
        PoplarHeapSort<It, Compare>(first, last, Compare());
    }

    // Circle Sort (unstable)
    // Implementation by myself.

    _SortTpl _SortHead CircleSort(It first, It last, Comp comp) {
        _impl::circle_sort(first, last, comp);
    }

    _SortTplD _SortHead CircleSort(It first, It last) {
        _CompD;
        CircleSort<It, Compare>(first, last, Compare());
    }

    // Gnome Sort (stable)
    // Implementation by myself

    _SortTpl _SortHead GnomeSort(It first, It last, Comp comp) {
        _impl::gome_sort(first, last, comp);
    }

    _SortTplD _SortHead GnomeSort(It first, It last) {
        _CompD;
        GnomeSort<It, Compare>(first, last, Compare());
    }


    // Comb Sort (unstable)
    // Implementation: https://www.geeksforgeeks.org/cpp-program-for-comb-sort/

    _SortTpl _SortHead CombSort(It first, It last, Comp comp) {
        _impl::_combsort::combsort(first, last, comp);
    }

    _SortTplD _SortHead CombSort(It first, It last) {
        _CompD;
        CombSort<It, Compare>(first, last, Compare());
    }

    // Shell Sort (unstable)
    // Implementation: https://www.geeksforgeeks.org/cpp-program-for-shellsort/

    _SortTpl _SortHead ShellSort(It first, It last, Comp comp) {
        _impl::shell_sort(first, last, comp);
    }

    _SortTplD _SortHead ShellSort(It first, It last) {
        _CompD;
        ShellSort<It, Compare>(first, last, Compare());
    }

    // Ternary Heap Sort (unstable)
    // Implementation by myself.

    _SortTpl _SortHead TernaryHeapSort(It first, It last, Comp comp) {
        _impl::_ternarysort::ternary_heap_sort(first, last, comp);
    }

    _SortTplD _SortHead TernaryHeapSort(It first, It last) {
        _CompD;
        TernaryHeapSort<It, Compare>(first, last, Compare());
    }


    // Patience Sort (unstable)
    // Implementation by myself.

    _SortTpl _SortHead PatienceSort(It first, It last, Comp comp) {
        _impl::patience_sort(first, last, comp);
    }

    _SortTplD _SortHead PatienceSort(It first, It last) {
        _CompD;
        PatienceSort<It, Compare>(first, last, Compare());
    }

    // Odd Even Sort (unstable)
    // Implementation: https://www.geeksforgeeks.org/odd-even-sort-brick-sort/
    _SortTpl _SortHead OddEvenSort(It first, It last, Comp comp) {
        _impl::odd_even_sort(first, last, comp);
    }

    _SortTplD _SortHead OddEvenSort(It first, It last) {
        _CompD;
        OddEvenSort<It, Compare>(first, last, Compare());
    }

    // Silly Sort (unstable)
    // Implementation by myself.

    _SortTpl _SortHead SillySort(It first, It last, Comp comp) {
        _impl::_sillySort(first, std::distance(first, last), comp);
    }

    _SortTplD _SortHead SillySort(It first, It last) {
        _CompD;
        SillySort<It, Compare>(first, last, Compare());
    }

    // Bitonic Sort (unstable)
    // Implementation: https://www.geeksforgeeks.org/bitonic-sort/
    _SortTpl _SortHead BitonicSort(It first, It last, Comp comp) {
        _impl::_bitonicsort::bitonic_sort(first, last, comp);
    }

    _SortTplD _SortHead BitonicSort(It first, It last) {
        _CompD;
        BitonicSort<It, Compare>(first, last, Compare());
    }

    // Smooth Sort (unstable)
    // Implementation: https://baobaobear.github.io/post/20191017-weakheapsort/
    _SortTpl _SortHead SmoothSort(It first, It last, Comp comp) {
        _impl::_smoothsort::smooth_sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead SmoothSort(It first, It last) {
        _CompD;
        SmoothSort<It, Compare>(first, last, Compare());
    }

    // Weak Heap Sort (unstable)
    // Implementation: https://baobaobear.github.io/post/20191017-weakheapsort/
    _SortTpl _SortHead WeakHeapSort(It first, It last, Comp comp) {
        _impl::_weaksort::weakheap_sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead WeakHeapSort(It first, It last) {
        _CompD;
        WeakHeapSort<It, Compare>(first, last, Compare());
    }


    // CombSort-11 (unstable)
    // Implementation: https://github.com/electronicarts/EASTL/blob/master/include/EASTL/sort.h#L1767
    _SortTpl _SortHead CombSort11(It first, It last, Comp comp) {
        _impl::_comb11<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead CombSort11(It first, It last) {
        _CompD;
        CombSort11<It, Compare>(first, last, Compare());
    }

    // Double Selection Sort (unstable)
    // Implementation: https://github.com/JoshSilverb/double_ended_sorts/blob/master/double_selection_sort.h
    _SortTpl _SortHead DoubleSelectionSort(It first, It last, Comp comp) {
        typedef ItValue<It> Item;
        _impl::_double_select_sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead DoubleSelectionSort(It first, It last) {
        _CompD;
        DoubleSelectionSort<It, Compare>(first, last, Compare());
    }

    // Shift Sort (unstable)
    // Implementation: https://github.com/JamesQuintero/ShiftSort/blob/master/C%2B%2B/ShiftSort.hpp
    _SortTpl _SortHead ShiftSort(It first, It last, Comp comp) {
        _shiftsort::shiftsort(first, last, comp);
    }

    _SortTplD _SortHead ShiftSort(It first, It last) {
        _CompD;
        ShiftSort<It, Compare>(first, last, Compare());
    }
    
    // Bogo Sort (unstable, slow)
    // Implementation by myself.
    _SortTpl _SortHead BogoSort(It first, It last, Comp comp) {
        std::random_device rdv;
        while (!std::is_sorted(first, last, comp)) std::shuffle(first, last, rdv);
    }

    _SortTplD _SortHead BogoSort(It first, It last) {
        _CompD;
        BogoSort<It, Compare>(first, last, Compare());
    }

    // BogoBogo Sort (unstable, very slow)
    // Implementation by myself.
    _SortTpl _SortHead BogoBogoSort(It first, It last, Comp comp) {
        if (std::distance(first, last) < 2) return;

        std::random_device rdv;
        typedef ItSize<It> Size;
        Size index = 2;

        while (!std::is_sorted(first, last, comp)) {
            BogoSort(first, first + index, comp);
            index++;
            if (!std::is_sorted(first, first + index, comp)) {
                std::shuffle(first, last, rdv);
                index = 2;
            }
            
        }
    }

    _SortTplD _SortHead BogoBogoSort(It first, It last) {
        _CompD;
        BogoBogoSort<It, Compare>(first, last, Compare());
    }

    // Stooge Sort (slow)
    // Implementation: https://www.geeksforgeeks.org/the-slowest-sorting-algorithms/
    _SortTpl _SortHead StoogeSort(It first, It last, Comp comp) {
        _impl::slowest::stooge_sort(first, last, comp);
    }

    _SortTplD _SortHead StoogeSort(It first, It last) {
        _CompD;
        StoogeSort<It, Compare>(first, last, Compare());
    }

    // Slow Sort (slow)
    // Implementation: https://www.geeksforgeeks.org/the-slowest-sorting-algorithms/
    _SortTpl _SortHead SlowSort(It first, It last, Comp comp) {
        _impl::slowest::slow_sort(first, last, comp);
    }

    _SortTplD _SortHead SlowSort(It first, It last) {
        _CompD;
        SlowSort<It, Compare>(first, last, Compare());
    }


    // Rotate Merge Sort (stable)
    // Implementation: https://github.com/ceorron/stable-inplace-sorting-algorithms/blob/master/sort.hpp
    // See the stlib.hpp file.
    _SortTpl _SortHead RotateMergeSort(It first, It last, Comp comp) {
        stlib::rotate_merge_sort(first, last, comp);
    }

    _SortTplD _SortHead RotateMergeSort(It first, It last) {
        _CompD;
        RotateMergeSort<It, Compare>(first, last, Compare());
    }

    // Stable Quick Sort (stable)
    // Implementation: https://github.com/ceorron/stable-inplace-sorting-algorithms/blob/master/sort.hpp
    // See the stlib.hpp file.
    _SortTpl _SortHead StableQuickSort(It first, It last, Comp comp) {
        stlib::stable_quick_sort(first, last, comp);
    }

    _SortTplD _SortHead StableQuickSort(It first, It last) {
        _CompD;
        StableQuickSort<It, Compare>(first, last, Compare());
    }

    // Double Insertion Sort (stable)
    // Implementation: https://baobaobear.github.io/post/20191009-sorting-2/
    _SortTpl _SortHead DoubleInsertSort(It first, It last, Comp comp) {
        _impl::double_insertion_sort(first, last, comp);
    }

    _SortTplD _SortHead DoubleInsertSort(It first, It last) {
        _CompD;
        DoubleInsertSort<It, Compare>(first, last, Compare());
    }
    
    // Indie Sort (unstable)
    // Implementation: https://github.com/mattreecebentley/plf_indiesort/blob/master/plf_indiesort.h
    _SortTpl _SortHead IndieSort(It first, It last, Comp comp) {
        plf::indiesort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead IndieSort(It first, It last) {
        _CompD;
        IndieSort<It, Compare>(first, last, Compare());
    }

    // Nano Sort (unstable)
    // Implementation: https://github.com/zeux/nanosort/blob/master/nanosort.hpp
    _SortTpl _SortHead NanoSort(It first, It last, Comp comp) {
        nanosort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead NanoSort(It first, It last) {
        _CompD;
        NanoSort<It, Compare>(first, last, Compare());
    }

    // ARoot Sort (stable)
    // Implementation: https://github.com/kaybee1928/ARoot-Sort/blob/master/ARootSort/ARootSort
    _SortTpl _SortHead ARootSort(It first, It last, Comp comp) {
        ksb::aroot_sort(first, last, comp);
    }

    _SortTplD _SortHead ARootSort(It first, It last) {
        _CompD;
        ARootSort<It, Compare>(first, last, Compare());
    }

    // Heap Sort (unstable)
    _SortTpl _SortHead HeapSort(It first, It last, Comp comp) {
        std::make_heap<It, Comp>(first, last, comp);
        std::sort_heap<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead HeapSort(It first, It last) {
        _CompD;
        HeapSort<It, Compare>(first, last, Compare());
    }

    // Hayate Sort (unstable)
    // Implementation: https://github.com/EmuraDaisuke/SortingAlgorithm.HayateShiki/blob/master/HayateShiki.h

    _SortTpl _SortHead HayateSort(It first, It last, Comp comp) {
        HayateShiki::sort<It, Comp>(first, last, comp);
    }

    _SortTplD _SortHead HayateSort(It first, It last) {
        _CompD;
        HayateSort<It, Compare>(first, last, Compare());
    }

    // Weave Merge Sort (unstable)
    // Implementation by myself.
    _SortTpl _SortHead WeaveMergeSort(It first, It last, Comp comp) {
        _impl::_wvmergesort::wsort<It, ItValue<It>, ItSize<It>, Comp>(first, last, comp);
    }

    _SortTplD _SortHead WeaveMergeSort(It first, It last) {
        _CompD;
        WeaveMergeSort<It, Compare>(first, last, Compare());
    }

    // Sqrtsort (stable)
    // Implementation: https://github.com/pystraf/sqrt-sort (by myself).
    _SortTpl _SortHead SqrtSort(It first, It last, Comp comp) {
        sqrtsort::sqrtsort(first, last, comp);
    }

    _SortTplD _SortHead SqrtSort(It first, It last) {
        _CompD;
        SqrtSort<It, Compare>(first, last, Compare());
    }
}
