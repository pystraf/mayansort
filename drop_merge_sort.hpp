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

// Copied from https://github.com/adrian17/cpp-drop-merge-sort/blob/master/drop_merge_sort.hpp

#include <vector>
#include <algorithm>
#include <type_traits>
#include <iterator>

/*
    A C++ reimplementation of a drop-merge sort, originally made by Emil Ernerfeldt:
    https://github.com/emilk/drop-merge-sort
    There are two versions of this function.
    The first one is identical to the reference Rust implementation.
    It doesn't support uncopyable types.
    Here, it's used only for trivially copyable types.
    (just like the Rust version, which required the Copy trait).
    The second version is copy-free, so it supports noncopyable types like std::unique_ptr.
    It's also more efficient for types with expensive copying, like std::string.
    However, being copy-free involves some extra bookkeeping, so for trivial types
    it's a bit less efficient than the original function - which is why I use both versions.
*/

namespace MayanSort {

    namespace detail {

        constexpr static bool double_comparison = true;

        // with-trivial-copies version
        template<typename Iter, typename Comp>
        void dmsort(Iter begin, Iter end, Comp comp, std::true_type) {
            size_t size = end - begin;

            if (size < 2)
                return;

            using Value = typename std::iterator_traits<Iter>::value_type;
            std::vector<Value> dropped;

            size_t num_dropped_in_row = 0;
            auto write = begin;
            auto read = begin;

            constexpr size_t recency = 8;

            while (read != end) {
                if (begin != write && comp(*read, *(write - 1))) {

                    if (double_comparison && num_dropped_in_row == 0 && write > begin + 1 && !comp(*read, *(write - 2))) {
                        dropped.push_back(*(write - 1));
                        *(write - 1) = *read;
                        ++read;
                        continue;
                    }

                    if (num_dropped_in_row < recency) {
                        dropped.push_back(*read);
                        ++read;
                        ++num_dropped_in_row;
                    }
                    else {
                        for (int i = 0; i < num_dropped_in_row; ++i) {
                            dropped.pop_back();
                        }
                        read -= num_dropped_in_row;

                        --write;
                        dropped.push_back(*write);

                        num_dropped_in_row = 0;
                    }
                }
                else {
                    // Here we don't need to guard against self-move because such an
                    // operation can't destroy the value for trivially copyable types
                    *write = std::move(*read);
                    ++read;
                    ++write;
                    num_dropped_in_row = 0;
                }
            }

            std::sort(dropped.begin(), dropped.end(), comp);

            auto back = end;

            while (!dropped.empty()) {
                auto& last_dropped = dropped.back();

                while (begin != write && comp(last_dropped, *(write - 1))) {
                    --back;
                    --write;
                    *back = std::move(*write);
                }
                --back;
                *back = std::move(last_dropped);
                dropped.pop_back();
            }
        }

        // move-only version
        template<typename Iter, typename Comp>
        void dmsort(Iter begin, Iter end, Comp comp, std::false_type) {
            size_t size = end - begin;

            if (size < 2)
                return;

            using Value = typename std::iterator_traits<Iter>::value_type;
            std::vector<Value> dropped;

            size_t num_dropped_in_row = 0;
            auto write = begin;
            auto read = begin;

            constexpr size_t recency = 8;

            while (read != end) {
                if (begin != write && comp(*read, *(write - 1))) {

                    if (double_comparison && num_dropped_in_row == 0 && write > begin + 1 && !comp(*read, *(write - 2))) {
                        dropped.push_back(std::move(*(write - 1)));
                        *(write - 1) = std::move(*read);
                        ++read;
                        continue;
                    }

                    if (num_dropped_in_row < recency) {
                        dropped.push_back(std::move(*read));
                        ++read;
                        ++num_dropped_in_row;
                    }
                    else {
                        for (int i = 0; i < num_dropped_in_row; ++i) {
                            --read;
                            *read = std::move(*(dropped.end() - 1));
                            dropped.pop_back();
                        }

                        --write;
                        dropped.push_back(std::move(*write));

                        num_dropped_in_row = 0;
                    }
                }
                else {
                    if (read != write) {
                        *write = std::move(*read);
                    }
                    ++read;
                    ++write;
                    num_dropped_in_row = 0;
                }
            }

            std::sort(dropped.begin(), dropped.end(), comp);

            auto back = end;

            while (!dropped.empty()) {
                auto& last_dropped = dropped.back();

                while (begin != write && comp(last_dropped, *(write - 1))) {
                    --back;
                    --write;
                    *back = std::move(*write);
                }
                --back;
                *back = std::move(last_dropped);
                dropped.pop_back();
            }
        }
    }
    template<typename Iter, typename Comp>
    void dmsort(Iter begin, Iter end, Comp comp) {
        using Value = typename std::iterator_traits<Iter>::value_type;
        detail::dmsort(begin, end, comp, std::is_trivially_copyable<Value>());
    }
    template<typename Iter>
    void dmsort(Iter begin, Iter end) {
        dmsort(begin, end, std::less<>());
    }
}