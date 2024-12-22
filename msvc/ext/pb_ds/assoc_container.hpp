#pragma once
#include <unordered_map>
#include <unordered_set>
namespace std
{
    namespace __gnu_pbds
    {
        struct null_type
        {
        };

        template <typename Key, typename Value, typename Hash, typename Compare, enable_if_t<!is_same_v<Value, null_type>, int> a = 0>
        using gp_hash_table = unordered_map<Key, Value, Hash, Compare>;

        template <typename Key, typename Value, typename Hash, typename Compare, enable_if_t<is_same_v<Value, null_type>, int> a = 0>
        using gp_hash_table = unordered_set<Key, Hash, Compare>;
    }

    template <typename Key, typename Value, typename Hash, typename Compare>
    void swap(__gnu_pbds::gp_hash_table<Key, Value, Hash, Compare>& g1, __gnu_pbds::gp_hash_table<Key, Value, Hash, Compare>& g2) noexcept
    {
        g1.swap(g2);
    }
}