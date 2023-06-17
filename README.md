# mayansort
Sorting algorithm library for c++20.

# Get Started
```cpp
#include "mayansort.hpp"
#include <vector>
std::vector<int> li{2,4,435,243,4333,5,34,11,9,6,4,3};
int main(){
  MayanSort::GrailSort(li.begin(), li.end());
  for(auto e:li) cout<<e<<" ";
  return 0;
}
```
