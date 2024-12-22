#pragma once
#include <cstring>
#include <string>
#include <cstdlib>
#include <type_traits>
#include <cassert>
#include <utility>
#include <cstdint>
#include <functional>
#include <iterator>
#include <algorithm>
#include <unordered_set>

#define EMH_LIKELY(condition)   (condition)
#define EMH_UNLIKELY(condition) (condition)

#define EMH_KEY(p, n)     p[n].first
#define EMH_VAL(p, n)     p[n].second
#define EMH_KV(p, n)      p[n]

#define EMH_INDEX(i, n)   i[n]
#define EMH_BUCKET(i, n)  i[n].bucket
#define EMH_HSLOT(i, n)   i[n].slot
#define EMH_SLOT(i, n)    (i[n].slot & _mask)
#define EMH_PREVET(i, n)  i[n].slot

#define EMH_KEYMASK(key, mask)  ((size_type)(key) & ~mask)
#define EMH_EQHASH(n, key_hash) (EMH_KEYMASK(key_hash, _mask) == (_index[n].slot & ~_mask))
#define EMH_NEW(key, val, bucket, key_hash) \
    new(_pairs + _num_filled) value_type(key, val); \
    _etail = bucket; \
    _index[bucket] = {bucket, _num_filled++ | EMH_KEYMASK(key_hash, _mask)}

#define EMH_EMPTY(i, n) (0 > (int)i[n].bucket)

namespace emhash8 {

#ifndef EMH_DEFAULT_LOAD_FACTOR
    constexpr static float EMH_DEFAULT_LOAD_FACTOR = 0.80f;
    constexpr static float EMH_MIN_LOAD_FACTOR = 0.25f; //< 0.5
#endif
#if EMH_CACHE_LINE_SIZE < 32
    constexpr static uint32_t EMH_CACHE_LINE_SIZE = 64;
#endif

    template <typename KeyT, typename ValueT, typename HashT = std::hash<KeyT>, typename EqT = std::equal_to<KeyT>>
    class HashMap
    {
    public:
        using htype = HashMap<KeyT, ValueT, HashT, EqT>;
        using value_type = std::pair<KeyT, ValueT>;
        using key_type = KeyT;
        using mapped_type = ValueT;

        using size_type = uint32_t;

        using hasher = HashT;
        using key_equal = EqT;

        constexpr static size_type INACTIVE = 0 - 1u;
        //constexpr uint32_t END      = 0-0x1u;
        constexpr static size_type EAD = 2;

        struct Index
        {
            size_type bucket;
            size_type slot;
        };

        class const_iterator;
        class iterator
        {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = typename htype::value_type;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;

            iterator() : kv_(nullptr) {}
            iterator(const_iterator& cit) {
                kv_ = cit.kv_;
            }

            iterator(const htype* hash_map, size_type bucket) {
                kv_ = hash_map->_pairs + (int)bucket;
            }

            iterator& operator++()
            {
                kv_++;
                return *this;
            }

            iterator operator++(int)
            {
                auto cur = *this; kv_++;
                return cur;
            }

            iterator& operator--()
            {
                kv_--;
                return *this;
            }

            iterator operator--(int)
            {
                auto cur = *this; kv_--;
                return cur;
            }

            reference operator*() const { return *kv_; }
            pointer operator->() const { return kv_; }

            bool operator == (const iterator& rhs) const { return kv_ == rhs.kv_; }
            bool operator != (const iterator& rhs) const { return kv_ != rhs.kv_; }
            bool operator == (const const_iterator& rhs) const { return kv_ == rhs.kv_; }
            bool operator != (const const_iterator& rhs) const { return kv_ != rhs.kv_; }

        public:
            value_type* kv_;
        };

        class const_iterator
        {
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = typename htype::value_type;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const value_type&;

            const_iterator(const iterator& it) {
                kv_ = it.kv_;
            }

            const_iterator(const htype* hash_map, size_type bucket) {
                kv_ = hash_map->_pairs + (int)bucket;
            }

            const_iterator& operator++()
            {
                kv_++;
                return *this;
            }

            const_iterator operator++(int)
            {
                auto cur = *this; kv_++;
                return cur;
            }

            const_iterator& operator--()
            {
                kv_--;
                return *this;
            }

            const_iterator operator--(int)
            {
                auto cur = *this; kv_--;
                return cur;
            }

            const_reference operator*() const { return *kv_; }
            const_pointer operator->() const { return kv_; }

            bool operator == (const iterator& rhs) const { return kv_ == rhs.kv_; }
            bool operator != (const iterator& rhs) const { return kv_ != rhs.kv_; }
            bool operator == (const const_iterator& rhs) const { return kv_ == rhs.kv_; }
            bool operator != (const const_iterator& rhs) const { return kv_ != rhs.kv_; }
        public:
            const value_type* kv_;
        };

        void init(size_type bucket, float mlf = EMH_DEFAULT_LOAD_FACTOR)
        {
            _pairs = nullptr;
            _index = nullptr;
            _mask = _num_buckets = 0;
            _num_filled = 0;
            max_load_factor(mlf);
            rehash(bucket);
        }

        HashMap(size_type bucket = 2, float mlf = EMH_DEFAULT_LOAD_FACTOR)
        {
            init(bucket, mlf);
        }

        HashMap(const HashMap& rhs)
        {
            if (rhs.load_factor() > EMH_MIN_LOAD_FACTOR)
            {
                _pairs = alloc_bucket((size_type)(rhs._num_buckets * rhs.max_load_factor()) + 4);
                _index = alloc_index(rhs._num_buckets);
                clone(rhs);
            }
            else
            {
                init(rhs._num_filled + 2, EMH_DEFAULT_LOAD_FACTOR);
                for (auto it = rhs.begin(); it != rhs.end(); ++it)
                    insert_unique(it->first, it->second);
            }
        }

        HashMap(HashMap&& rhs) noexcept
        {
            init(0);
            *this = std::move(rhs);
        }

        HashMap(std::initializer_list<value_type> ilist)
        {
            init((size_type)ilist.size());
            for (auto it = ilist.begin(); it != ilist.end(); ++it)
                do_insert(*it);
        }

        template<class InputIt>
        HashMap(InputIt first, InputIt last, size_type bucket_count = 4)
        {
            init(std::distance(first, last) + bucket_count);
            for (; first != last; ++first)
                emplace(*first);
        }

        HashMap& operator=(const HashMap& rhs)
        {
            if (this == &rhs)
                return *this;

            if (rhs.load_factor() < EMH_MIN_LOAD_FACTOR)
            {
                clear(); free(_pairs); _pairs = nullptr;
                rehash(rhs._num_filled + 2);
                for (auto it = rhs.begin(); it != rhs.end(); ++it)
                    insert_unique(it->first, it->second);
                return *this;
            }

            clearkv();

            if (_num_buckets != rhs._num_buckets)
            {
                free(_pairs); free(_index);
                _index = alloc_index(rhs._num_buckets);
                _pairs = alloc_bucket((size_type)(rhs._num_buckets * rhs.max_load_factor()) + 4);
            }

            clone(rhs);
            return *this;
        }

        HashMap& operator=(HashMap&& rhs) noexcept
        {
            if (this != &rhs)
            {
                swap(rhs);
                rhs.clear();
            }
            return *this;
        }

        ~HashMap() noexcept
        {
            clearkv();
            free(_pairs);
            free(_index);
        }

    private:
        void clone(const HashMap& rhs)
        {
            _hasher = rhs._hasher;
            //        _eq          = rhs._eq;
            _num_buckets = rhs._num_buckets;
            _num_filled = rhs._num_filled;
            _mlf = rhs._mlf;
            _last = rhs._last;
            _mask = rhs._mask;
            _etail = rhs._etail;

            auto opairs = rhs._pairs;
            memcpy((char*)_index, (char*)rhs._index, (_num_buckets + EAD) * sizeof(Index));

            if (is_copy_trivially())
            {
                memcpy((char*)_pairs, (char*)opairs, _num_filled * sizeof(value_type));
            }
            else
            {
                for (size_type slot = 0; slot < _num_filled; slot++)
                    new(_pairs + slot) value_type(opairs[slot]);
            }
        }
    public:

        void swap(HashMap& rhs)
        {
            //      std::swap(_eq, rhs._eq);
            std::swap(_hasher, rhs._hasher);
            std::swap(_pairs, rhs._pairs);
            std::swap(_index, rhs._index);
            std::swap(_num_buckets, rhs._num_buckets);
            std::swap(_num_filled, rhs._num_filled);
            std::swap(_mask, rhs._mask);
            std::swap(_mlf, rhs._mlf);
            std::swap(_last, rhs._last);
            std::swap(_etail, rhs._etail);
        }

        // -------------------------------------------------------------
        inline iterator begin() { return first(); }
        inline const_iterator cbegin() const { return first(); }
        inline const_iterator begin() const { return first(); }

        inline iterator end() { return { this, _num_filled }; }
        inline const_iterator cend() const { return { this, _num_filled }; }
        inline const_iterator end() const { return cend(); }

        inline const value_type* values() const { return _pairs; }
        inline const Index* index() const { return _index; }

        inline size_type size() const { return _num_filled; }
        inline bool empty() const { return _num_filled == 0; }
        inline size_type bucket_count() const { return _num_buckets; }

        /// Returns average number of elements per bucket.
        inline float load_factor() const { return static_cast<float>(_num_filled) / (_mask + 1); }

        inline HashT& hash_function() const { return _hasher; }
        inline EqT& key_eq() const { return _eq; }

        void max_load_factor(float mlf)
        {
            if (mlf < 0.991 && mlf > EMH_MIN_LOAD_FACTOR)
            {
                _mlf = (uint32_t)((1 << 27) / mlf);
                if (_num_buckets > 0) rehash(_num_buckets);
            }
        }

        inline constexpr float max_load_factor() const { return (1 << 27) / (float)_mlf; }
        inline constexpr size_type max_size() const { return (1ull << (sizeof(size_type) * 8 - 1)); }
        inline constexpr size_type max_bucket_count() const { return max_size(); }

        // ------------------------------------------------------------
        template<typename K = KeyT>
        inline iterator find(const K& key) noexcept
        {
            return { this, find_filled_slot(key) };
        }

        template<typename K = KeyT>
        inline const_iterator find(const K& key) const noexcept
        {
            return { this, find_filled_slot(key) };
        }

        template<typename K = KeyT>
        ValueT& at(const K& key)
        {
            const auto slot = find_filled_slot(key);
            //throw
            return EMH_VAL(_pairs, slot);
        }

        template<typename K = KeyT>
        const ValueT& at(const K& key) const
        {
            const auto slot = find_filled_slot(key);
            //throw
            return EMH_VAL(_pairs, slot);
        }

        template<typename K = KeyT>
        inline bool contains(const K& key) const noexcept
        {
            return find_filled_slot(key) != _num_filled;
        }

        template<typename K = KeyT>
        inline size_type count(const K& key) const noexcept
        {
            return find_filled_slot(key) == _num_filled ? 0 : 1;
            //return find_sorted_bucket(key) == END ? 0 : 1;
            //return find_hash_bucket(key) == END ? 0 : 1;
        }

        template<typename K = KeyT>
        std::pair<iterator, iterator> equal_range(const K& key)
        {
            const auto found = find(key);
            if (found.second == _num_filled)
                return { found, found };
            else
                return { found, std::next(found) };
        }

        private:
        void merge(HashMap& rhs)
        {
            if (empty())
            {
                *this = std::move(rhs);
                return;
            }

            for (auto rit = rhs.begin(); rit != rhs.end(); )
            {
                auto fit = find(rit->first);
                if (fit == end())
                {
                    insert_unique(rit->first, std::move(rit->second));
                    rit = rhs.erase(rit);
                }
                else
                {
                    ++rit;
                }
            }
        }

        /// Returns the matching ValueT or nullptr if k isn't found.
        bool try_get(const KeyT& key, ValueT& val) const noexcept
        {
            const auto slot = find_filled_slot(key);
            const auto found = slot != _num_filled;
            if (found)
            {
                val = EMH_VAL(_pairs, slot);
            }
            return found;
        }

        /// Returns the matching ValueT or nullptr if k isn't found.
        ValueT* try_get(const KeyT& key) noexcept
        {
            const auto slot = find_filled_slot(key);
            return slot != _num_filled ? &EMH_VAL(_pairs, slot) : nullptr;
        }

        /// Const version of the above
        ValueT* try_get(const KeyT& key) const noexcept
        {
            const auto slot = find_filled_slot(key);
            return slot != _num_filled ? &EMH_VAL(_pairs, slot) : nullptr;
        }

        /// set value if key exist
        bool try_set(const KeyT& key, const ValueT& val) noexcept
        {
            const auto slot = find_filled_slot(key);
            if (slot == _num_filled)
                return false;

            EMH_VAL(_pairs, slot) = val;
            return true;
        }

        /// set value if key exist
        bool try_set(const KeyT& key, ValueT&& val) noexcept
        {
            const auto slot = find_filled_slot(key);
            if (slot == _num_filled)
                return false;

            EMH_VAL(_pairs, slot) = std::move(val);
            return true;
        }

        /// Convenience function.
        ValueT get_or_return_default(const KeyT& key) const noexcept
        {
            const auto slot = find_filled_slot(key);
            return slot == _num_filled ? ValueT() : EMH_VAL(_pairs, slot);
        }

        // -----------------------------------------------------
        std::pair<iterator, bool> do_insert(const value_type& value) noexcept
        {
            const auto key_hash = hash_key(value.first);
            const auto bucket = find_or_allocate(value.first, key_hash);
            const auto bempty = EMH_EMPTY(_index, bucket);
            if (bempty)
            {
                EMH_NEW(value.first, value.second, bucket, key_hash);
            }

            const auto slot = EMH_SLOT(_index, bucket);
            return { {this, slot}, bempty };
        }

        std::pair<iterator, bool> do_insert(value_type&& value) noexcept
        {
            const auto key_hash = hash_key(value.first);
            const auto bucket = find_or_allocate(value.first, key_hash);
            const auto bempty = EMH_EMPTY(_index, bucket);
            if (bempty)
            {
                EMH_NEW(std::move(value.first), std::move(value.second), bucket, key_hash);
            }

            const auto slot = EMH_SLOT(_index, bucket);
            return { {this, slot}, bempty };
        }

        template<typename K, typename V>
        std::pair<iterator, bool> do_insert(K&& key, V&& val) noexcept
        {
            const auto key_hash = hash_key(key);
            const auto bucket = find_or_allocate(key, key_hash);
            const auto bempty = EMH_EMPTY(_index, bucket);
            if (bempty)
            {
                EMH_NEW(std::forward<K>(key), std::forward<V>(val), bucket, key_hash);
            }

            const auto slot = EMH_SLOT(_index, bucket);
            return { {this, slot}, bempty };
        }

        template<typename K, typename V>
        std::pair<iterator, bool> do_assign(K&& key, V&& val) noexcept
        {
            check_expand_need();
            const auto key_hash = hash_key(key);
            const auto bucket = find_or_allocate(key, key_hash);
            const auto bempty = EMH_EMPTY(_index, bucket);
            if (bempty)
            {
                EMH_NEW(std::forward<K>(key), std::forward<V>(val), bucket, key_hash);
            }
            else
            {
                EMH_VAL(_pairs, EMH_SLOT(_index, bucket)) = std::move(val);
            }

            const auto slot = EMH_SLOT(_index, bucket);
            return { {this, slot}, bempty };
        }
        public:

        std::pair<iterator, bool> insert(const value_type& p)
        {
            check_expand_need();
            return do_insert(p);
        }

        std::pair<iterator, bool> insert(value_type&& p)
        {
            check_expand_need();
            return do_insert(std::move(p));
        }

        void insert(std::initializer_list<value_type> ilist)
        {
            reserve(ilist.size() + _num_filled, false);
            for (auto it = ilist.begin(); it != ilist.end(); ++it)
                do_insert(*it);
        }

        template <typename Iter>
        void insert(Iter first, Iter last)
        {
            reserve(std::distance(first, last) + _num_filled, false);
            for (; first != last; ++first)
                do_insert(first->first, first->second);
        }

        private:
        template<typename K, typename V>
        size_type insert_unique(K&& key, V&& val)
        {
            check_expand_need();
            const auto key_hash = hash_key(key);
            auto bucket = find_unique_bucket(key_hash);
            EMH_NEW(std::forward<K>(key), std::forward<V>(val), bucket, key_hash);
            return bucket;
        }

        size_type insert_unique(value_type&& value)
        {
            return insert_unique(std::move(value.first), std::move(value.second));
        }

        inline size_type insert_unique(const value_type& value)
        {
            return insert_unique(value.first, value.second);
        }

        template <class... Args>
        inline std::pair<iterator, bool> emplace(Args&&... args) noexcept
        {
            check_expand_need();
            return do_insert(std::forward<Args>(args)...);
        }

        //no any optimize for position
        template <class... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args)
        {
            (void)hint;
            check_expand_need();
            return do_insert(std::forward<Args>(args)...).first;
        }

        template<class... Args>
        std::pair<iterator, bool> try_emplace(const KeyT& k, Args&&... args)
        {
            check_expand_need();
            return do_insert(k, std::forward<Args>(args)...);
        }

        template<class... Args>
        std::pair<iterator, bool> try_emplace(KeyT&& k, Args&&... args)
        {
            check_expand_need();
            return do_insert(std::move(k), std::forward<Args>(args)...);
        }

        template <class... Args>
        inline size_type emplace_unique(Args&&... args)
        {
            return insert_unique(std::forward<Args>(args)...);
        }

        std::pair<iterator, bool> insert_or_assign(const KeyT& key, ValueT&& val) { return do_assign(key, std::forward<ValueT>(val)); }
        std::pair<iterator, bool> insert_or_assign(KeyT&& key, ValueT&& val) { return do_assign(std::move(key), std::forward<ValueT>(val)); }

        /// Return the old value or ValueT() if it didn't exist.
        ValueT set_get(const KeyT& key, const ValueT& val)
        {
            check_expand_need();
            const auto key_hash = hash_key(key);
            const auto bucket = find_or_allocate(key, key_hash);
            if (EMH_EMPTY(_index, bucket))
            {
                EMH_NEW(key, val, bucket, key_hash);
                return ValueT();
            }
            else
            {
                const auto slot = EMH_SLOT(_index, bucket);
                ValueT old_value(val);
                std::swap(EMH_VAL(_pairs, slot), old_value);
                return old_value;
            }
        }
        public:
        /// Like std::map<KeyT, ValueT>::operator[].
        ValueT& operator[](const KeyT& key) noexcept
        {
            check_expand_need();
            const auto key_hash = hash_key(key);
            const auto bucket = find_or_allocate(key, key_hash);
            if (EMH_EMPTY(_index, bucket))
            {
                /* Check if inserting a value rather than overwriting an old entry */
                EMH_NEW(key, std::move(ValueT()), bucket, key_hash);
            }

            const auto slot = EMH_SLOT(_index, bucket);
            return EMH_VAL(_pairs, slot);
        }

        ValueT& operator[](KeyT&& key) noexcept
        {
            check_expand_need();
            const auto key_hash = hash_key(key);
            const auto bucket = find_or_allocate(key, key_hash);
            if (EMH_EMPTY(_index, bucket))
            {
                EMH_NEW(std::move(key), std::move(ValueT()), bucket, key_hash);
            }

            const auto slot = EMH_SLOT(_index, bucket);
            return EMH_VAL(_pairs, slot);
        }

        /// Erase an element from the hash table.
        /// return 0 if element was not found
        size_type erase(const KeyT& key) noexcept
        {
            const auto key_hash = hash_key(key);
            const auto sbucket = find_filled_bucket(key, key_hash);
            if (sbucket == INACTIVE)
                return 0;

            const auto main_bucket = key_hash & _mask;
            erase_slot(sbucket, (size_type)main_bucket);
            return 1;
        }

        //iterator erase(const_iterator begin_it, const_iterator end_it)
        iterator erase(const const_iterator& cit) noexcept
        {
            const auto slot = (size_type)(cit.kv_ - _pairs);
            size_type main_bucket;
            const auto sbucket = find_slot_bucket(slot, main_bucket); //TODO
            erase_slot(sbucket, main_bucket);
            return { this, slot };
        }

        //only last >= first
        iterator erase(const_iterator first, const_iterator last) noexcept
        {
            auto esize = long(last.kv_ - first.kv_);
            auto tsize = long((_pairs + _num_filled) - last.kv_); //last to tail size
            auto next = first;
            while (tsize-- > 0)
            {
                if (esize-- <= 0)
                    break;
                next = ++erase(next);
            }

            //fast erase from last
            next = this->last();
            while (esize-- > 0)
                next = --erase(next);

            return { this, size_type(next.kv_ - _pairs) };
        }

        static constexpr bool is_triviall_destructable()
        {
#if __cplusplus >= 201402L || _MSC_VER > 1600
            return !(std::is_trivially_destructible<KeyT>::value && std::is_trivially_destructible<ValueT>::value);
#else
            return !(std::is_pod<KeyT>::value && std::is_pod<ValueT>::value);
#endif
        }

        static constexpr bool is_copy_trivially()
        {
#if __cplusplus >= 201103L || _MSC_VER > 1600
            return (std::is_trivially_copyable<KeyT>::value && std::is_trivially_copyable<ValueT>::value);
#else
            return (std::is_pod<KeyT>::value && std::is_pod<ValueT>::value);
#endif
        }

        void clearkv()
        {
            if (is_triviall_destructable())
            {
                while (_num_filled--)
                    _pairs[_num_filled].~value_type();
            }
        }

        /// Remove all elements, keeping full capacity.
        void clear() noexcept
        {
            clearkv();

            if (_num_filled > 0)
                memset((char*)_index, INACTIVE, sizeof(_index[0]) * _num_buckets);

            _last = _num_filled = 0;
            _etail = INACTIVE;
        }

        void shrink_to_fit(const float min_factor = EMH_DEFAULT_LOAD_FACTOR / 4)
        {
            if (load_factor() < min_factor && bucket_count() > 10) //safe guard
                rehash(_num_filled + 1);
        }

        /// Make room for this many elements
        bool reserve(uint64_t num_elems, bool force)
        {
            (void)force;
#if EMH_HIGH_LOAD == 0
            const auto required_buckets = num_elems * _mlf >> 27;
            if (EMH_LIKELY(required_buckets < _mask)) // && !force
                return false;
            //assert(required_buckets < max_size());
            rehash(required_buckets + 2);
            return true;
        }

        static value_type* alloc_bucket(size_type num_buckets)
        {
            auto new_pairs = malloc((uint64_t)num_buckets * sizeof(value_type));
            return (value_type*)(new_pairs);
        }

        static Index* alloc_index(size_type num_buckets)
        {
            auto new_index = (char*)malloc((uint64_t)(EAD + num_buckets) * sizeof(Index));
            return (Index*)(new_index);
        }

        bool reserve(size_type required_buckets) noexcept
        {
            if (_num_filled != required_buckets)
                return reserve(required_buckets, true);

            _last = 0;

            memset((char*)_index, INACTIVE, sizeof(_index[0]) * _num_buckets);
            for (size_type slot = 0; slot < _num_filled; slot++)
            {
                const auto& key = EMH_KEY(_pairs, slot);
                const auto key_hash = hash_key(key);
                const auto bucket = size_type(key_hash & _mask);
                auto& next_bucket = EMH_BUCKET(_index, bucket);
                if ((int)next_bucket < 0)
                    EMH_INDEX(_index, bucket) = { 1, slot | EMH_KEYMASK(key_hash, _mask) };
                else
                {
                    EMH_HSLOT(_index, bucket) |= EMH_KEYMASK(key_hash, _mask);
                    next_bucket++;
                }
            }
            return true;
        }

        void rebuild(size_type num_buckets) noexcept
        {
            free(_index);
            auto new_pairs = (value_type*)alloc_bucket((size_type)(num_buckets * max_load_factor()) + 4);
            if (is_copy_trivially())
            {
                if (_pairs)
                    memcpy((char*)new_pairs, (char*)_pairs, _num_filled * sizeof(value_type));
            }
            else
            {
                for (size_type slot = 0; slot < _num_filled; slot++)
                {
                    new(new_pairs + slot) value_type(std::move(_pairs[slot]));
                    if (is_triviall_destructable())
                        _pairs[slot].~value_type();
                }
            }
            free(_pairs);
            _pairs = new_pairs;
            _index = (Index*)alloc_index(num_buckets);

            memset((char*)_index, INACTIVE, sizeof(_index[0]) * num_buckets);
            memset((char*)(_index + num_buckets), 0, sizeof(_index[0]) * EAD);
        }

        void rehash(uint64_t required_buckets)
        {
            if (required_buckets < _num_filled)
                return;

            assert(required_buckets < max_size());
            auto num_buckets = _num_filled > (1u << 16) ? (1u << 16) : 4u;
            while (num_buckets < required_buckets) { num_buckets *= 2; }
            _last = _mask / 4;

            _mask = num_buckets - 1;
            _num_buckets = num_buckets;

            rebuild(num_buckets);

            _etail = INACTIVE;
            for (size_type slot = 0; slot < _num_filled; ++slot)
            {
                const auto& key = EMH_KEY(_pairs, slot);
                const auto key_hash = hash_key(key);
                const auto bucket = find_unique_bucket(key_hash);
                EMH_INDEX(_index, bucket) = { bucket, slot | EMH_KEYMASK(key_hash, _mask) };
            }
        }

    private:
        // Can we fit another element?
        inline bool check_expand_need()
        {
            return reserve(_num_filled, false);
        }

        size_type slot_to_bucket(const size_type slot) const noexcept
        {
            size_type main_bucket;
            return find_slot_bucket(slot, main_bucket); //TODO
        }

        //very slow
        void erase_slot(const size_type sbucket, const size_type main_bucket) noexcept
        {
            const auto slot = EMH_SLOT(_index, sbucket);
            const auto ebucket = erase_bucket(sbucket, main_bucket);
            const auto last_slot = --_num_filled;
            if (EMH_LIKELY(slot != last_slot))
            {
                const auto last_bucket = (_etail == INACTIVE || ebucket == _etail)
                    ? slot_to_bucket(last_slot) : _etail;

                EMH_KV(_pairs, slot) = std::move(EMH_KV(_pairs, last_slot));
                EMH_HSLOT(_index, last_bucket) = slot | (EMH_HSLOT(_index, last_bucket) & ~_mask);
            }

            if (is_triviall_destructable())
                _pairs[last_slot].~value_type();

            _etail = INACTIVE;
            EMH_INDEX(_index, ebucket) = { INACTIVE, 0 };
        }

        size_type erase_bucket(const size_type bucket, const size_type main_bucket) noexcept
        {
            const auto next_bucket = EMH_BUCKET(_index, bucket);
            if (bucket == main_bucket)
            {
                if (main_bucket != next_bucket)
                {
                    const auto nbucket = EMH_BUCKET(_index, next_bucket);
                    EMH_INDEX(_index, main_bucket) = {
                        (nbucket == next_bucket) ? main_bucket : nbucket,
                        EMH_HSLOT(_index, next_bucket)
                    };
                }
                return next_bucket;
            }

            const auto prev_bucket = find_prev_bucket(main_bucket, bucket);
            EMH_BUCKET(_index, prev_bucket) = (bucket == next_bucket) ? prev_bucket : next_bucket;
            return bucket;
        }

        // Find the slot with this key, or return bucket size
        size_type find_slot_bucket(const size_type slot, size_type& main_bucket) const
        {
            const auto key_hash = hash_key(EMH_KEY(_pairs, slot));
            const auto bucket = main_bucket = size_type(key_hash & _mask);
            if (slot == EMH_SLOT(_index, bucket))
                return bucket;

            auto next_bucket = EMH_BUCKET(_index, bucket);
            while (true)
            {
                if (EMH_LIKELY(slot == EMH_SLOT(_index, next_bucket)))
                    return next_bucket;
                next_bucket = EMH_BUCKET(_index, next_bucket);
            }

            return INACTIVE;
        }

        // Find the slot with this key, or return bucket size
        size_type find_filled_bucket(const KeyT& key, uint64_t key_hash) const noexcept
        {
            const auto bucket = size_type(key_hash & _mask);
            auto next_bucket = EMH_BUCKET(_index, bucket);
            if (EMH_UNLIKELY((int)next_bucket < 0))
                return INACTIVE;

            if (EMH_EQHASH(bucket, key_hash))
            {
                const auto slot = EMH_SLOT(_index, bucket);
                if (EMH_LIKELY(_eq(key, EMH_KEY(_pairs, slot))))
                    return bucket;
            }
            if (next_bucket == bucket)
                return INACTIVE;

            while (true)
            {
                if (EMH_EQHASH(next_bucket, key_hash))
                {
                    const auto slot = EMH_SLOT(_index, next_bucket);
                    if (EMH_LIKELY(_eq(key, EMH_KEY(_pairs, slot))))
                        return next_bucket;
                }

                const auto nbucket = EMH_BUCKET(_index, next_bucket);
                if (nbucket == next_bucket)
                    return INACTIVE;
                next_bucket = nbucket;
            }

            return INACTIVE;
        }

        // Find the slot with this key, or return bucket size
        template<typename K = KeyT>
        size_type find_filled_slot(const K& key) const noexcept
        {
            const auto key_hash = hash_key(key);
            const auto bucket = size_type(key_hash & _mask);
            auto next_bucket = EMH_BUCKET(_index, bucket);
            if ((int)next_bucket < 0)
                return _num_filled;

            if (EMH_EQHASH(bucket, key_hash))
            {
                const auto slot = EMH_SLOT(_index, bucket);
                if (EMH_LIKELY(_eq(key, EMH_KEY(_pairs, slot))))
                    return slot;
            }
            if (next_bucket == bucket)
                return _num_filled;

            while (true)
            {
                if (EMH_EQHASH(next_bucket, key_hash))
                {
                    const auto slot = EMH_SLOT(_index, next_bucket);
                    if (EMH_LIKELY(_eq(key, EMH_KEY(_pairs, slot))))
                        return slot;
                }

                const auto nbucket = EMH_BUCKET(_index, next_bucket);
                if (nbucket == next_bucket)
                    return _num_filled;
                next_bucket = nbucket;
            }

            return _num_filled;
        }
        //kick out bucket and find empty to occpuy
        //it will break the orgin link and relnik again.
        //before: main_bucket-->prev_bucket --> bucket   --> next_bucket
        //atfer : main_bucket-->prev_bucket --> (removed)--> new_bucket--> next_bucket
        size_type kickout_bucket(const size_type kmain, const size_type bucket) noexcept
        {
            const auto next_bucket = EMH_BUCKET(_index, bucket);
            const auto new_bucket = find_empty_bucket(next_bucket, 2);
            const auto prev_bucket = find_prev_bucket(kmain, bucket);

            const auto last = next_bucket == bucket ? new_bucket : next_bucket;
            EMH_INDEX(_index, new_bucket) = { last, EMH_HSLOT(_index, bucket) };

            EMH_BUCKET(_index, prev_bucket) = new_bucket;
            EMH_BUCKET(_index, bucket) = INACTIVE;

            return bucket;
        }

        /*
        ** inserts a new key into a hash table; first, check whether key's main
        ** bucket/position is free. If not, check whether colliding node/bucket is in its main
        ** position or not: if it is not, move colliding bucket to an empty place and
        ** put new key in its main position; otherwise (colliding bucket is in its main
        ** position), new key goes to an empty position.
        */
        template<typename K = KeyT>
        size_type find_or_allocate(const K& key, uint64_t key_hash) noexcept
        {
            const auto bucket = size_type(key_hash & _mask);
            auto next_bucket = EMH_BUCKET(_index, bucket);
            if ((int)next_bucket < 0)
            {
                return bucket;
            }

            const auto slot = EMH_SLOT(_index, bucket);
            if (EMH_EQHASH(bucket, key_hash))
                if (EMH_LIKELY(_eq(key, EMH_KEY(_pairs, slot))))
                    return bucket;

            //check current bucket_key is in main bucket or not
            const auto kmain = hash_bucket(EMH_KEY(_pairs, slot));
            if (kmain != bucket)
                return kickout_bucket(kmain, bucket);
            else if (next_bucket == bucket)
                return EMH_BUCKET(_index, next_bucket) = find_empty_bucket(next_bucket, 1);

            uint32_t csize = 1;
            //find next linked bucket and check key
            while (true)
            {
                const auto eslot = EMH_SLOT(_index, next_bucket);
                if (EMH_EQHASH(next_bucket, key_hash))
                {
                    if (EMH_LIKELY(_eq(key, EMH_KEY(_pairs, eslot))))
                        return next_bucket;
                }

                csize += 1;
                const auto nbucket = EMH_BUCKET(_index, next_bucket);
                if (nbucket == next_bucket)
                    break;
                next_bucket = nbucket;
            }

            //find a empty and link it to tail
            const auto new_bucket = find_empty_bucket(next_bucket, csize);
            return EMH_BUCKET(_index, next_bucket) = new_bucket;
        }

        size_type find_unique_bucket(uint64_t key_hash) noexcept
        {
            const auto bucket = size_type(key_hash & _mask);
            auto next_bucket = EMH_BUCKET(_index, bucket);
            if ((int)next_bucket < 0)
            {
                return bucket;
            }

            //check current bucket_key is in main bucket or not
            const auto kmain = hash_main(bucket);
            if (EMH_UNLIKELY(kmain != bucket))
                return kickout_bucket(kmain, bucket);
            else if (EMH_UNLIKELY(next_bucket != bucket))
                next_bucket = find_last_bucket(next_bucket);

            return EMH_BUCKET(_index, next_bucket) = find_empty_bucket(next_bucket, 2);
        }

        /***
          Different probing techniques usually provide a trade-off between memory locality and avoidance of clustering.
        Since Robin Hood hashing is relatively resilient to clustering (both primary and secondary), linear probing is the most cache friendly alternativeis typically used.

            It's the core algorithm of this hash map with highly optimization/benchmark.
        normaly linear probing is inefficient with high load factor, it use a new 3-way linear
        probing strategy to search empty slot. from benchmark even the load factor > 0.9, it's more 2-3 timer fast than
        one-way search strategy.

        1. linear or quadratic probing a few cache line for less cache miss from input slot "bucket_from".
        2. the first  search  slot from member variant "_last", init with 0
        3. the second search slot from calculated pos "(_num_filled + _last) & _mask", it's like a rand value
        */
        // key is not in this mavalue. Find a place to put it.
        size_type find_empty_bucket(const size_type bucket_from, uint32_t csize) noexcept
        {
            auto bucket = bucket_from;
            if (EMH_EMPTY(_index, ++bucket) || EMH_EMPTY(_index, ++bucket))
                return bucket;
            constexpr size_type quadratic_probe_length = 6u;
            for (size_type offset = 4u, step = 3u; step < quadratic_probe_length; )
            {
                bucket = (bucket_from + offset) & _mask;
                if (EMH_EMPTY(_index, bucket) || EMH_EMPTY(_index, ++bucket))
                    return bucket;
                offset += step++;//3.4.5
            }

            for (;;)
            {
                if (EMH_EMPTY(_index, ++_last))// || EMH_EMPTY(_index, ++_last))
                    return _last++;

                _last &= _mask;
                auto medium = (_num_buckets / 2 + _last) & _mask;
                if (EMH_EMPTY(_index, medium))// || EMH_EMPTY(_index, ++medium))
                    return _last = medium;
            }

            return 0;
        }

        size_type find_last_bucket(size_type main_bucket) const
        {
            auto next_bucket = EMH_BUCKET(_index, main_bucket);
            if (next_bucket == main_bucket)
                return main_bucket;

            while (true)
            {
                const auto nbucket = EMH_BUCKET(_index, next_bucket);
                if (nbucket == next_bucket)
                    return next_bucket;
                next_bucket = nbucket;
            }
        }

        size_type find_prev_bucket(const size_type main_bucket, const size_type bucket) const
        {
            auto next_bucket = EMH_BUCKET(_index, main_bucket);
            if (next_bucket == bucket)
                return main_bucket;

            while (true)
            {
                const auto nbucket = EMH_BUCKET(_index, next_bucket);
                if (nbucket == bucket)
                    return next_bucket;
                next_bucket = nbucket;
            }
        }

        inline size_type hash_bucket(const KeyT& key) const noexcept
        {
            return (size_type)hash_key(key) & _mask;
        }

        inline size_type hash_main(const size_type bucket) const noexcept
        {
            const auto slot = EMH_SLOT(_index, bucket);
            return (size_type)hash_key(EMH_KEY(_pairs, slot)) & _mask;
        }

    private:
        template<typename UType, typename std::enable_if<std::is_integral<UType>::value, uint32_t>::type = 0>
        inline uint64_t hash_key(const UType key) const
        {
#if EMH_INT_HASH
            return hash64(key);
#elif EMH_IDENTITY_HASH
            return key + (key >> 24);
#else
            return _hasher(key);
#endif
        }

        template<typename UType, typename std::enable_if<std::is_same<UType, std::string>::value, uint32_t>::type = 0>
        inline uint64_t hash_key(const UType& key) const
        {
#if EMH_WYHASH_HASH
            return wyhashstr(key.data(), key.size());
#else
            return _hasher(key);
#endif
        }

        template<typename UType, typename std::enable_if<!std::is_integral<UType>::value && !std::is_same<UType, std::string>::value, uint32_t>::type = 0>
        inline uint64_t hash_key(const UType& key) const
        {
            return _hasher(key);
        }

    private:
        Index* _index;
        value_type* _pairs;

        HashT     _hasher;
        EqT       _eq;
        uint32_t  _mlf;
        size_type _mask;
        size_type _num_buckets;
        size_type _num_filled;
        size_type _last;
        size_type _etail;
    };
} // namespace emhash
#undef  EMH_KEY
#undef  EMH_VAL
#undef  EMH_KV
#undef  EMH_BUCKET
#undef  EMH_NEW
#undef  EMH_EMPTY
#undef  EMH_PREVET
#undef  EMH_LIKELY
#undef  EMH_UNLIKELY
namespace std
{
	namespace __gnu_pbds
	{
		struct null_type
		{
		};

        template <typename Key, typename Value, typename Hash, typename Compare, enable_if_t<!is_same_v<Value, null_type>, int> a = 0>
        using gp_hash_table = emhash8::HashMap<Key, Value, Hash, Compare>;

        template <typename Key, typename Value, typename Hash, typename Compare, enable_if_t<is_same_v<Value, null_type>, int> a = 0>
        using gp_hash_table = unordered_set<Key, Hash, Compare>;
	}
}