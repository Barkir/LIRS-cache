        #pragma once

#include <unordered_map>
#include <list>
#include <queue>

// #define DEBUG_MODE
#ifdef DEBUG_MODE

#define ON_DEBUG(code) code

#else

#define ON_DEBUG(code)

#endif

const size_t DEFAULT_CACHE_CAP=4;

template <typename T, typename KeyT  = int>
class WhateverCache {
    private:
        using lstIter = typename std::list<std::pair<KeyT, T>>::iterator;
        std::unordered_map<KeyT, lstIter> hashMap;
        std::list<std::pair<KeyT, T>> dataList;
        size_t capacity;

    public:
        WhateverCache(size_t cap = DEFAULT_CACHE_CAP);
        void printNElem(size_t n);
        void insert(KeyT key, T elem);
        T* get(KeyT key);
        T* slow_get_page(KeyT key);
};


template<typename T, typename KeyT>
WhateverCache<T, KeyT>::WhateverCache(size_t cap) : capacity(cap) {
    ON_DEBUG(std::cout << "WhateverCache created with capacity " << cap << std::endl);
}


template<typename T, typename KeyT>
void WhateverCache<T, KeyT>::printNElem(size_t n) {
    size_t count = 0;
    for (const auto& el : dataList) {
        if (count++ >= n)
            break;
        std::cout << el << " ";
    }
    std::cout << "\n";
}


template<typename T, typename KeyT>
void WhateverCache<T, KeyT>::insert(KeyT key, T elem) {

    auto it = hashMap.find(key);
    if (it != hashMap.end()) {
        ON_DEBUG(std::cout << "case: key is already in use. replacing..." << "\n");

        it->second->second = elem;
        dataList.splice(dataList.begin(), dataList, it->second);
        hashMap[key] = dataList.begin();
        return;
    }
    if (hashMap.size() >= capacity) {
        ON_DEBUG(std::cout << "case: hashmap is full. finding LRU element and deleting it from hashmap" << "\n");
        auto lastIt = std::prev(dataList.end());
        KeyT lastKey = lastIt->first;

        ON_DEBUG(std::cout << "deleting least recently used element - " << lastIt->second << "\n");
        ON_DEBUG(if (hashMap.find(lastKey) != hashMap.end()))
            ON_DEBUG(std::cout << "element is really in a hashmap!" << "\n");
        hashMap.erase(lastKey);
        dataList.pop_back();

        ON_DEBUG(std::cout << "pushing new pair to the list..." << "\n");
        dataList.push_front(std::make_pair(key, elem));

        ON_DEBUG(std::cout << "moving it to the hashmap..." << "\n");
        hashMap[key] = dataList.begin();
    } else {
        ON_DEBUG(std::cout << "case hashMap is not full yet : inserting element..." << "\n");
        dataList.push_front(std::make_pair(key, elem));
        hashMap[key] = dataList.begin();
    }
}


template<typename T, typename KeyT>
T* WhateverCache<T, KeyT>::get(KeyT key) {
    auto found = hashMap.find(key);
    if (found != hashMap.end()) {
        std::cout << "hit!" << "\n";

        // moving the element to the start of the dataList
        dataList.splice(dataList.begin(), dataList, found->second);

        // updating hashMap
        hashMap[key] = dataList.begin();
        return (&found->second->second);
    }

    ON_DEBUG(std::cout << "page not found in hash, finding it slowly..." << "\n");
    return slow_get_page(key);
}

template<typename T, typename KeyT>
T* WhateverCache<T, KeyT>::slow_get_page(KeyT key) {
    for (auto it = dataList.begin(); it != dataList.end(); it++) {
        if (it->first == key) {
            // Перемещаем найденный элемент в начало
            dataList.splice(dataList.begin(), dataList, it);
            // Обновляем хэш-таблицу
            hashMap[key] = dataList.begin();
            return &(dataList.front().second);
        }
    }
    return nullptr;
}

