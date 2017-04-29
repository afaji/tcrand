# tcrand

**tcrand** is a C++ library for random generation of various competitive programming data structures, such as arrays and trees.

**WARNING**: This library is still in **beta** version. Use at your own risk!

## Documentation

API reference can be found [here](docs/api/README.md).

## Example

```cpp
#include <tcrand/tree.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace tcrand;

int main() {
    // Create a tree randomizer.
    TreeRandomizer tree_rand;

    // Set the desired properties.
    tree_rand
        .node_count(1000);
        .depth(50);

    // Randomly generate a tree.
    Tree tree = tree_rand.next();
    int N = tree.node_count();
    vector<int> U = tree.edges().first;
    vector<int> V = tree.edges().second;

    // Print the tree.
    cout << N << endl;
    for (int i = 0; i < N - 1; i++) {
        cout << U[i] << " " << V[i] << endl;
    }
}

```

## Installation

This is a header-only library. To use:

- Make sure you have a C++11 compiler. 
- Clone this repository (suppose that you clone it to `~/tcrand`).
- Add `-std=c++11 -I ~/tcrand/include` to your C++ compilation flags.

## License

**tcrand** is released under MIT license.
