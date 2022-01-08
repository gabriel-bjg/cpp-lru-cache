# C++14 LRU Cache
[![build](https://github.com/gabriel-bjg/cpp-lru-cache/workflows/Makefile%20build/badge.svg)](https://github.com/gabriel-bjg/cpp-lru-cache/actions/workflows/build_make.yml)
[![codecov](https://codecov.io/gh/gabriel-bjg/cpp-lru-cache/branch/main/graph/badge.svg?token=PA4DL4FXUE)](https://codecov.io/gh/gabriel-bjg/cpp-lru-cache)

Header only C++14 LRU Cache with strong exception safety.
| Public API | Description | Complexity | Exception safety |
| --- | --- | --- | --- |
`constructor` | Create a new lru cache with a limited capacity | constant | strong |
`empty` | Check if the lru cache has no items | constant | nothrow |
`size` | Get the number of items in the lru cache | constant | nothrow |
`clear` | Remove all items from the lru cache | linear | nothrow |
`put` | Add an item to the lru cache or update the existing item's value. Mark it as the most recent one | amortized constant on average, worst case linear | strong |
`get` | Get the value of an existing item and mark the item as the most recent one | constant on average, worst case linear | strong |
`contains` | Check if the lru cache contains an item with the given key | constant on average, worst case linear | strong |

The `insert_new_item` private function uses the catch and re-throw mechanism in order to guarantee strong exception safety. This decision was taken to avoid any other dependencies. You can find other solutions [here](https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758). If you already have a pattern/mechanism in your project for handling this situation, please consider adapting the lru_cache according to your project.

## Requirements
* C++14 compiler
* CMake 3.15

## Project dependencies
* Catch2
* cppcheck
* clang-tidy
* clang-format
* gcovr

Install dependencies on Ubuntu
```
chmod +x ./dependencies/install.sh 
./dependencies/install.sh
```

## Build
Build the project
```
make build
```

Run tests
```
make check
```

Run tests with coverage report. The report can be found at: `build/coverage.xml`
```
make check-with-coverage
```

Clean build directory
```
make clean
```

## Usage examples
```c++
// Create a cache with a capacity of 25
lru_cache<int, std::string> cache{25};

// Put some items in the cache
cache.put(std::make_pair(1, "one"));
cache.put(std::make_pair(2, "two"));
cache.put(std::make_pair(3, "three"));

// Get item's value
const auto& item_value = cache.get(1);

// Check if the cache contains an item
if (!cache.contains(5)) {
    std::cout << "Item not available in the lru cache\n";
}

// Clear the cache
cache.clear();

// Check if the cache is empty
if (cache.empty()) {
    std::cout << "The cache is empty\n";
}
```

For more examples please see [behavioral tests](./test/lru_cache_tests.cpp)
