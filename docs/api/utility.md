# utility

Various utility functions for generating random scalars and vectors.

```cpp
#include <tcrand/utility.hpp>
```

## Example

```cpp
// x is a random number between 1 and 100, inclusive.
int x = rand_int(1, 100);

// w is a random subsequence (not necessarily contiguous) of v.
vector<int> v = {1, 2, 3, 4};
vector<int> w = rand_subsequence(v);
```

## Functions

### Scalar functions

- <a href="#rand_int__int" name="rand_int__int">rand_int(int maxValEx)</a>

  ```cpp
  int rand_int(int maxValEx);
  ```

  Returns a uniformly distributed random `int` between 0 (inclusive) and `maxValEx` (exclusive).

- <a href="#rand_int__int_int" name="rand_int__int_int">rand_int(int minVal, int maxVal)</a>

  ```cpp
  int rand_int(int minVal, int maxVal);
  ```

  Returns a uniformly distributed random `int` between `minVal` (inclusive) and `maxVal` (inclusive).

- <a href="#rand_long__longlong" name="rand_long__longlong">rand_long(long long maxValEx)</a>

  ```cpp
  long long rand_long(long long maxValEx);
  ```

  Returns a uniformly distributed random `long long` between 0 (inclusive) and `maxValEx` (exclusive).

- <a href="#rand_long__longlong_longlong" name="rand_long__longlong_longlong">rand_long(long long minVal, long long maxVal)</a>

  ```cpp
  long long rand_long(long long minVal, long long maxVal);
  ```

  Returns a uniformly distributed random `long long` between `minVal` (inclusive) and `maxVal` (inclusive).

- <a href="#rand_double__double" name="rand_double__double">rand_double(double maxVal)</a>

  ```cpp
  double rand_double(double maxVal);
  ```

  Returns a uniformly distributed random `double` between 0 (inclusive) and `maxVal` (inclusive).

- <a href="#rand_double__double_double" name="rand_double__double_double">rand_double(double minVal, double maxVal)</a>

  ```cpp
  double rand_double(double minVal, double maxVal);
  ```

  Returns a uniformly distributed random `double` between `minVal` (inclusive) and `maxVal` (inclusive).

### Vector functions

All functions below accept a `V`, which is either an `std::vector` or `std::string`.

- <a href="#rand_subarray__V" name="rand_subarray__V">rand_subarray(const V& v)</a>

  ```cpp
  template<typename V>
  V rand_subarray(const V& v);
  ```

  Returns a uniformly distributed random contiguous subarray of `v`.

- <a href="rand_subarray__V_int" name="rand_subarray__V_int">rand_subarray(const V& v, int length)</a>

  ```cpp
  template<typename V>
  V rand_subarray(const V& v, int length);
  ```

  Returns a uniformly distributed random contiguous subarray of length `length` of `v`.

- <a href="#rand_subsequence__V" name="rand_subsequence__V">rand_subsequence(const V& v)</a>

  ```cpp
  template<typename V>
  V rand_subsequence(const V& v);
  ```

  Returns a uniformly distributed random subsequence (not necessarily contiguous) of `v`.

- <a href="#rand_subsequence__V_int" name="rand_subsequence__V_int">rand_subsequence(const V& v, int length)</a>

  ```cpp
  template<typename V>
  V rand_subsequence(const V& v, int length);
  ```

  Returns a uniformly distributed random subsequence (not necessarily contiguous) of length `length` of `v`.
