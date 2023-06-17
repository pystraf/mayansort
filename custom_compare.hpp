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

// custom_compare.hpp: Compare functions for some sort algorithms.

// For sqrtsort:
namespace MayanSort {
	template<typename It, typename Comp>
	struct SqrtSortComp {
		Comp comp;
		int operator()(It first, It second) {
			if (comp(*first, *second)) return -1;
			return 1;
		}
	};

	template<typename T, typename Comp>
	struct CFuncComp {
		Comp comp;
		int operator()(const void *a, const void *b) {
			T x = *(T*)a, y = *(T*)b;
			if (comp(x, y)) return -1;
			return 1;
		}
	};
}
