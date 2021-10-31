# C++14 LRU Cache
[![build](https://github.com/gabriel-bjg/cpp-lru-cache/workflows/Makefile%20build/badge.svg)](https://github.com/gabriel-bjg/cpp-lru-cache/actions/workflows/build_make.yml)
[![codecov](https://codecov.io/gh/gabriel-bjg/cpp-lru-cache/branch/main/graph/badge.svg?token=PA4DL4FXUE)](https://codecov.io/gh/gabriel-bjg/cpp-lru-cache)

Header only C++14 LRU Cache with the following public interface:
* `constructor` - Create a new lru cache with a limited capacity.
* `empty` - Check if the lru cache has no items.
* `size` - Get the number of items in the lru cache.
* `clear` - Remove all items from the lru cache.
* `put` - Add an item to the lru cache or update the existing item's value and mark it as the most recent one if the key already exists.
* `get` - Get the value of an existing item and mark the item as the most recent one.
* `contains` - Check if the lru cache contains an item with the given key.

## Requirements
* C++14 compiler
* CMake 3.15

## Project dependencies
* Catch2
* cppcheck
* clang-tidy
* clang-format
* gcovr

## Build
Build the project
```
make build
```

Run tests
```
make check
```

Run tests with coverage report. The report can be found at: build/coverage.xml
```
check-with-coverage
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

// Get item's value from the cache
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
