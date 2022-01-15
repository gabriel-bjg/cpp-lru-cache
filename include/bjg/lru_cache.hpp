#ifndef BJG_LRU_CACHE_HPP
#define BJG_LRU_CACHE_HPP

#include <list>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace bjg {

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
    using items_list = std::list<item_type>;
    using items_list_iterator = typename items_list::iterator;

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
    const Value &get(const Key &key) {
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
     * This is a simpler version for the GuardedScope mechanism presented here:
     * https://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758 If you already have a pattern/mechanism in
     * your project for handling this situation, please consider adapting the lru_cache according to your project and drop this
     * one.
     */
    template <typename F>
    class guarded_scope {
       public:
        explicit guarded_scope(F &&f) noexcept : f_(std::move(f)), dismiss_{false} {}

        ~guarded_scope() noexcept(noexcept(f_)) {
            if (!dismiss_) f_();
        }

        void dismiss() noexcept { dismiss_ = true; }

       private:
        F f_;
        bool dismiss_;
    };

    /**
     * @brief Calls a function and reverts its behavior if the call fails.
     *
     * @param f The function to call.
     * @param r The function which reverts f's behavior.
     */
    template <typename F, typename R>
    void guarded_call(F &&f, R &&r) {
        auto guard = guarded_scope<R>(std::forward<R>(r));
        f();
        guard.dismiss();
    }

    /**
     * @brief Evict the least recent item if the lru cache size exceeds the maximum capacity.
     */
    void restrict_capacity() {
        if (items_.size() > capacity_) {
            keys_.erase(items_.back().first);
            items_.pop_back();  // never throws as capacity is always > 0 and items_.size() > capacity
        }
    }

    /**
     * @brief Insert an item as the most recent one to the lru cache.
     *
     * @param item The item to insert.
     */
    void insert_new_item(const item_type &item) {
        auto emplaced_item = std::make_pair(keys_.end(), false);
        items_.push_front(item);
        guarded_call([this, &item, &emplaced_item]() { emplaced_item = keys_.emplace(item.first, items_.begin()); },
                     [this]() { items_.pop_front(); });
        guarded_call([this]() { restrict_capacity(); },
                     [this, &emplaced_item]() {
                         // If the code execution reaches this point, emplaced_item contains a valid iterator
                         keys_.erase(emplaced_item.first);
                         items_.pop_front();
                     });
    }

    /**
     * @brief Mark an item as the most recent one.
     *
     * @param key The key of the item to mark.
     * @pre @p key must exist in @p items_
     */
    void move_to_front(const Key &key) {
        const auto existing_item = keys_.find(key);
        if (existing_item->second == items_.begin()) return;

        const auto next_it = std::next(existing_item->second);
        items_.splice(items_.begin(), items_, existing_item->second, next_it);
        guarded_call([this, &existing_item]() { auto it_copy = items_.begin(); std::swap(existing_item->second, it_copy); },
                     [this, &next_it]() { items_.splice(next_it, items_, items_.begin(), std::next(items_.begin())); });
    }

    /**
     * @brief Update the value of the most recent used item.
     *
     * @param value The new value.
     */
    void update_front_value(const Value &value) noexcept(std::is_nothrow_copy_assignable<Value>()) {
        auto value_copy = value;
        std::swap(items_.front().second, value_copy);
    }

    /**
     * @brief Return the value of the most recent used item.
     *
     * @pre @p items_ must contain at least one item.
     *
     * @return The value of the most recent used item.
     */
    const Value &get_front_value() const noexcept { return items_.front().second; }

    std::size_t capacity_;
    items_list items_;
    std::unordered_map<const Key, items_list_iterator, std::hash<Key>> keys_;
};
}  // namespace bjg

#endif
