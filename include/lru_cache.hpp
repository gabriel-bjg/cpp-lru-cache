#include <cstdint>
#include <list>
#include <stdexcept>
#include <unordered_map>
#include <utility>

/**
 * @brief Least Recently Used (LRU) cache container with a fixed capacity. After the maximum capacity is reached, least recently
 * used items are evicted from the cache.
 *
 * @tparam Key The key which uniquely identifies an item from the lru cache.
 * @tparam Value The value associated to the @p Key.
 */
template <class Key, class Value>
class lru_cache {
   public:
    using item_type = std::pair<const Key, Value>;

    /**
     * @brief Create a new lru cache with a limited capacity.
     *
     * @param capacity The maximum capacity of the cache. Once this limit is reached, least recent items are evicted.
     *
     * @throws std::length_error if the capacity is zero.
     */
    explicit lru_cache(const std::size_t capacity) : capacity_{capacity} {
        if (capacity_ == 0) {
            throw std::length_error{"Cache capacity must be greater than zero"};
        }
    }

    /**
     * @brief Check if the lru cache has no items.
     *
     * @return true if the lru cache is empty, false otherwise.
     */
    bool empty() const noexcept { return keys_.empty(); }

    /**
     * @brief Return the number of items in the lru cache.
     *
     * @return The number of items.
     */
    std::size_t size() const noexcept { return keys_.size(); }

    /**
     * @brief Remove all items from the lru cache.
     */
    void clear() noexcept {
        keys_.clear();
        items_.clear();
    }

    /**
     * @brief Add an item to the lru cache or update the existing item's value and mark it as the most recent one if the key
     * already exists.
     *
     * @param item The item to insert.
     */
    void put(const item_type &item) {
        if (contains(item.first)) {
            move_to_front(item.first);
            update_front_value(item.second);
        } else {
            insert_new_item(item);
        }
    }

    /**
     * @brief Return the value of an existing item and mark the item as the most recent one.
     *
     * @param key The key of the existing item.
     *
     * @return The value associated to the given key.
     * @throws std::out_of_range if the key does not exist.
     */
    Value &get(const Key &key) {
        if (!contains(key)) {
            throw std::out_of_range{"Key not found"};
        }

        move_to_front(key);
        return get_front_value();
    }

    /**
     * @brief Check if the lru cache contains an item with the given key.
     *
     * @param key The key to check.
     *
     * @return true if the key exists, false otherwise.
     */
    bool contains(const Key &key) const { return keys_.find(key) != keys_.cend(); }

   private:
    /**
     * @brief Evict the least recent item if the lru cache size exceeds the maximum capacity.
     */
    void restrict_capacity() {
        if (items_.size() > capacity_) {
            keys_.erase(items_.back().first);
            items_.pop_back();
        }
    }

    /**
     * @brief Insert an item as the most recent one to the lru cache.
     *
     * @param item The item to insert.
     */
    void insert_new_item(const item_type &item) {
        items_.push_front(item);
        keys_.emplace(item.first, items_.begin());
        restrict_capacity();
    }

    /**
     * @brief Mark an item as the most recent one.
     *
     * @param key The key of the item to mark.
     * @pre @p key must exist in @p items_
     */
    void move_to_front(const Key &key) {
        const auto existing_item = keys_.find(key);
        if (existing_item->second != items_.begin()) {
            items_.splice(items_.begin(), items_, existing_item->second, std::next(existing_item->second));
            existing_item->second = items_.begin();
        }
    }

    /**
     * @brief Update the value of the most recent used item.
     *
     * @param value The new value.
     */
    void update_front_value(const Value &value) noexcept { items_.front().second = value; }

    /**
     * @brief Return the value of the most recent used item.
     *
     * @return The value of the most recent used item.
     */
    Value &get_front_value() noexcept { return items_.front().second; }

    std::size_t capacity_;
    std::list<item_type> items_;
    std::unordered_map<const Key, typename std::list<item_type>::iterator, std::hash<Key>> keys_;
};
