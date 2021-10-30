# C++14 LRU Cache
builds & coverage goes here

Header only C++14 LRU Cache

## Requirements
* C++14 compiler
* CMake 3.15

## Project dependencies
* Catch2
* cppcheck
* clang-tidy
* clang-format

```
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
