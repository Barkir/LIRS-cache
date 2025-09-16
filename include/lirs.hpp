#pragma once

#include <stack>
#include <queue>
#include <list>

const size_t DEFAULT_LIR_CAP = 4;
const size_t DEFAULT_HIR_CAP = 4;

template <typename T, typename KeyT = int>
class LIRSCache {
    private:
        std::stack<std::pair<KeyT, T>> lowInterSet;
        std::queue<std::pair<KeyT, T>> highInterSet;
        std::list<std::pair<KeyT, T>> dataList;
        size_t capacity;

    public:
        LIRSCache(size_t lirCap = DEFAULT_LIR_CAP, size_t hirCap = DEFAULT_HIR_CAP);
        void insert(KeyT key, T& elem);
        T& get(KeyT key);
}
