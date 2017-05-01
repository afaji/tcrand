# vector

Random generation of vectors.

```cpp
#include <tcrand/vector.hpp>
```

## Example

```cpp
// Create a vector randomizer.
VectorRandomizer<int> vector_rand;

// Set up the desired properties.
vector_rand
        .length(10)
        .engine([]{return rand_int(1, 100);})
        .unique();

// v is a vector containing 10 unique integers between 1 and 100, inclusive.
vector<int> v = vector_rand.next();
```

## Randomizer class

### VectorRandomizer

```cpp
template<typename T>
class VectorRandomizer;
```

#### Properties

- <a href="#length">length</a>
- <a href="#engine">engine</a>
- <a href="#unique">unique</a>
- <a href="#distinct_elements">distinct_elements</a>

---

- <a href="#length" name="length">length</a>

  ```cpp
  VectorRandomizer& length(int);
  ```

  The desired number of elements of the vector.

- <a href="#engine" name="engine">engine</a>

  ```cpp
  VectorRandomizer& engine(function<T()>);
  ```

  The random generator engine for each element of the vector. The engine is a callable object, which can be a function pointer, function object, or lambda expression.
  
  Examples:
  
  ```cpp
  int fun() {
      return /* random number impl here */;
  }
  
  
  ...
  
  vector_rand.engine(fun);
  ```
  
  ```cpp 
  class Fun {
  public:
      int operator() {
          return /* random number impl here */;
      }
  };
  
  ...
  
  Fun fun;
  vector_rand.engine(fun);
  ```
  
  ```cpp 
  vector_rand.engine([] { return /* random number impl here */; });
  ```
  
  The random number generator functions in <a href="utility">utility</a> can be used to create the engine.

- <a href="#unique" name="unique">unique</a>

  ```cpp
  VectorRandomizer& unique();
  ```

  If set, then the elements of the vector will be unique (no duplicates).
  
  Must not be used together with <a href="#distinc_elements">distinct_elements</a>.

- <a href="#distinc_elements" name="distinct_elements">distinct_elements</a>

  ```cpp
  VectorRandomizer& distinct_elements(int)
  ```

  The desired number of distinct elements of the vector.
  
  Must not be used together with <a href="#unique">unique</a>.

#### Generating vector

- <a href="#next" name="next">next()</a>

  ```cpp
  vector<T> next();
  ```

  Returns a vector with the desired properties.
