#pragma once

#include <unordered_map>
#include <queue>

template <typename T, typename KeyT = int>
class IdealCache {


    private:
        size_t cap;
        std::unordered_map<KeyT, T> cache;
        std::unordered_map<KeyT, std::queue<size_t>> keyPos;

    public:
        IdealCache(size_t capIn) cap(capIn) {
            for (size_t i = 0; i < ca)
        }

}
