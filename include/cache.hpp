#pragma once

#include <unordered_map>
#include <list>
#include <queue>

const size_t DEFAULT_CACHE_CAP=4;

template <typename T, typename KeyT  = int>
class WhateverCache {
    private:
        using lstIter = typename std::list<T>::iterator;
        std::unordered_map<KeyT, lstIter> hashMap;
        std::list<std::pair<KeyT, T>> dataList;
        size_t capacity;

    public:
        WhateverCache(size_t cap);
        void printNElem(size_t n);
        void insert(KeyT key, T elem);
        T* find(KeyT key);
};
