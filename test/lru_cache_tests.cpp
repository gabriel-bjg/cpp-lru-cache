#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <stdexcept>
#include <string>
#include <utility>

#include "bjg/lru_cache.hpp"

SCENARIO("Create a lru cache with different sizes", "[lru_cache_constructor]") {
    GIVEN("A lru cache with key:int and value:std::string") {
        using lru_cache_t = bjg::lru_cache<int, std::string>;

        WHEN("The lru cache is created with zero capacity") {
            THEN("The construction throws") {
                CHECK_THROWS_AS([]() { lru_cache_t cache{0}; }(), std::length_error);
            }
        }

        WHEN("The lru cache is created with capacity bigger than 0") {
            THEN("The construction does not throw") {
                CHECK_NOTHROW([]() { lru_cache_t cache{25}; }());
            }
        }
    }
}

SCENARIO("Insert items into lru cache without exceeding capacity", "[lru_cache_insert_items]") {
    GIVEN("An empty lru cache with key:int and value:std::string") {
        using lru_cache_t = bjg::lru_cache<int, std::string>;
        lru_cache_t cache{100};

        REQUIRE(cache.size() == 0);
        REQUIRE(cache.empty());

        WHEN("An item is inserted") {
            cache.put(std::make_pair(1, "one"));

            THEN("The size changes and the item is available") {
                CHECK(cache.size() == 1);
                CHECK_FALSE(cache.empty());
                CHECK(cache.contains(1));
                CHECK(cache.get(1) == "one");
            }
        }

        WHEN("An item is inserted twice, but with different values") {
            cache.put(std::make_pair(1, "one"));
            cache.put(std::make_pair(1, "ONE"));

            THEN("The size changes and the item is available with the last value") {
                CHECK(cache.size() == 1);
                CHECK_FALSE(cache.empty());
                CHECK(cache.contains(1));
                CHECK(cache.get(1) == "ONE");
            }
        }

        WHEN("An item is inserted and the cache is cleared") {
            cache.put(std::make_pair(1, "one"));
            cache.clear();

            THEN("The cache is empty and the item is unavaiable") {
                CHECK(cache.size() == 0);
                CHECK(cache.empty());
                CHECK_FALSE(cache.contains(1));
                CHECK_THROWS_AS([&cache]() { cache.get(1); }(), std::out_of_range);
            }
        }
    }
}

SCENARIO("Insert items into lru cache and reach capacity", "[lru_cache_insert_items_reach_capacity]") {
    GIVEN("A lru cache with key:int, value:std::string, size = 3 and capacity = 5") {
        using lru_cache_t = bjg::lru_cache<int, std::string>;
        lru_cache_t cache{5};

        cache.put(std::make_pair(1, "one"));
        cache.put(std::make_pair(2, "two"));
        cache.put(std::make_pair(3, "three"));

        REQUIRE(cache.size() == 3);
        REQUIRE_FALSE(cache.empty());

        WHEN("Items are inserted and capacity is reached without being exceeded") {
            cache.put(std::make_pair(4, "four"));
            cache.put(std::make_pair(5, "five"));

            THEN("All items are available as no eviction should happen yet") {
                CHECK(cache.size() == 5);
                CHECK_FALSE(cache.empty());
                CHECK(cache.contains(1));
                CHECK(cache.get(1) == "one");
                CHECK(cache.contains(2));
                CHECK(cache.get(2) == "two");
                CHECK(cache.contains(3));
                CHECK(cache.get(3) == "three");
                CHECK(cache.contains(4));
                CHECK(cache.get(4) == "four");
                CHECK(cache.contains(5));
                CHECK(cache.get(5) == "five");
            }
        }

        WHEN("Items are inserted after capacity is reached") {
            cache.put(std::make_pair(4, "four"));
            cache.put(std::make_pair(5, "five"));
            cache.put(std::make_pair(6, "six"));
            cache.put(std::make_pair(7, "seven"));

            THEN("The cache has 5 items and the least recently used items are evicted") {
                CHECK(cache.size() == 5);
                CHECK_FALSE(cache.empty());
                CHECK_FALSE(cache.contains(1));
                CHECK_THROWS_AS([&cache]() { cache.get(1); }(), std::out_of_range);
                CHECK_FALSE(cache.contains(2));
                CHECK_THROWS_AS([&cache]() { cache.get(2); }(), std::out_of_range);
                CHECK(cache.contains(3));
                CHECK(cache.get(3) == "three");
                CHECK(cache.contains(4));
                CHECK(cache.get(4) == "four");
                CHECK(cache.contains(5));
                CHECK(cache.get(5) == "five");
                CHECK(cache.contains(6));
                CHECK(cache.get(6) == "six");
                CHECK(cache.contains(7));
                CHECK(cache.get(7) == "seven");
            }
        }
    }
}

SCENARIO("Operate on a full lru cache", "[lru_cache_full_capacity_operations]") {
    GIVEN("A lru cache with key:int, value:std::string, size = 5, capacity = 5") {
        using lru_cache_t = bjg::lru_cache<int, std::string>;
        lru_cache_t cache{5};

        cache.put(std::make_pair(1, "one"));
        cache.put(std::make_pair(2, "two"));
        cache.put(std::make_pair(3, "three"));
        cache.put(std::make_pair(4, "four"));
        cache.put(std::make_pair(5, "five"));

        REQUIRE(cache.size() == 5);
        REQUIRE_FALSE(cache.empty());

        WHEN("2 items are requested, 1 is updated and 2 more items are added") {
            CHECK(cache.get(1) == "one");
            cache.put(std::make_pair(3, "THREE"));
            CHECK(cache.get(5) == "five");
            cache.put(std::make_pair(6, "six"));
            cache.put(std::make_pair(7, "seven"));

            THEN("The requested, updated and added items are the 5 most recent ones") {
                CHECK(cache.size() == 5);
                CHECK_FALSE(cache.empty());
                CHECK(cache.contains(1));
                CHECK(cache.get(1) == "one");
                CHECK_FALSE(cache.contains(2));
                CHECK_THROWS_AS([&cache]() { cache.get(2); }(), std::out_of_range);
                CHECK(cache.contains(3));
                CHECK(cache.get(3) == "THREE");
                CHECK_FALSE(cache.contains(4));
                CHECK_THROWS_AS([&cache]() { cache.get(4); }(), std::out_of_range);
                CHECK(cache.contains(5));
                CHECK(cache.get(5) == "five");
                CHECK(cache.contains(6));
                CHECK(cache.get(6) == "six");
                CHECK(cache.contains(7));
                CHECK(cache.get(7) == "seven");
            }
        }

        WHEN("Cache is cleared") {
            cache.clear();

            THEN("No items are present in the lru cache") {
                CHECK(cache.size() == 0);
                CHECK(cache.empty());
            }
        }
    }
}
