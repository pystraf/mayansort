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

// mayandef.hpp: Base definitions for MayanSort.

#include <concepts>
#include <iterator>

namespace MayanSort {

	template<typename Iterator>
	using ItValue = typename std::iterator_traits<Iterator>::value_type;

	template<typename Iterator>
	using ItSize = typename std::iterator_traits<Iterator>::difference_type;

	#define _SortHead inline void
	#define _SortTpl template<typename It, typename Comp> requires std::sortable<It, Comp>
	#define _SortTplD template<typename It> requires std::sortable<It>
	#define _CompD typedef typename std::less<ItValue<It>> Compare


}