#include "cache.hpp"

WhateverCache::WhateverCache(size_t cap = DEFAULT_CACHE_CAP) : capacity(cap) {
    std::cout << "WhateverCache created with capacity " << cap << std::endl;
}

void WhateverCache::printNElem(size_t n) {
    size_t count = 0;
    for (const auto& el : dataList) {
        if (count++ >= n)
            break;
        std::cout << el << " ";
    }
    std::cout << "\n";
}

void WhateverCache::insert(KeyT key, T elem) {
    dataList.push_front(elem);

    auto it = hashMap.find(key);
    if (it != hashMap.end()) {
        if (hashMap.size() >= capacity) {
            auto lastElem = dataList.end();
            std::cout << "deleting least recently used element - " << lastElem << "\n";
            KeyT lruKey = lastElem->first();
            hashMap.erase(lruKey);

            hashMap[key] = dataList.begin();

            // move the last recently used element to the beginning
            dataList.splice(dataList.begin(), dataList, lastElem);
        }
    }
}

T* WhateverCache::find(KeyT key) {
    T found = hashMap.find(key);
    if (T != hashMap.end()) {
        std::cout << "found page in hashmap!" << "\n";
        return &T;
    }

    std::cout << "page not found in hash, finding it slowly..." << "\n";
    for (const auto& el : dataList) {
        if (el.first() == key) {
            std::cout << "found page in list!" << "\n";
            return &el;
        }
    }
    std::cout << "page not found!" << "\n";
    return nullptr;
    }
