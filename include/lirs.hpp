#pragma once

#include <stack>
#include <queue>
#include <list>

const size_t DEFAULT_LIR_CAP = 16;
const size_t DEFAULT_HIR_CAP = 2;

template <typename T>
class LIRSPage {
    public:
        T data;
        size_t recency;
        size_t irr;
        bool isResident;
};

template <typename T, typename KeyT = int>
class LIRSCache {
    private:
        std::list<std::pair<KeyT, T>> lowInterSet;
        std::list<std::pair<KeyT, T>> highInterSet;
        std::list<std::pair<KeyT, T>> dataList;
        size_t capacity;

    public:
        LIRSCache(size_t lirCap = DEFAULT_LIR_CAP, size_t hirCap = DEFAULT_HIR_CAP);
        void insert(KeyT key, T& elem);
        T& get(KeyT key);
        void prune();
};


// template<typename T, typename KeyT>
// void LIRSCache<T, KeyT>::insert(KeyT key, T elem) {
//     if ()
// }


// this function finds HIR resident blocks in LIR stack until
template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::prune() {
    for (auto it = lowInterSet.rbegin(); it != lowInterSet.rend(); it++) {
        if (it->isResident) {
            lowInterSet.erase(it);
        } else {
            break;
        }
    }
}
