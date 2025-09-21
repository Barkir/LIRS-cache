#pragma once

#include <stack>
#include <queue>
#include <list>
#include <optional>
#include <unordered_map>

const size_t DEFAULT_LIR_CAP = 16;
const size_t DEFAULT_HIR_CAP = 2;
const size_t RECENCY_THRESHOLD = 128;

template <typename T>
class LIRSPage {
    private:
        T data;
        std::optional<size_t> recency;
        std::optional<size_t> irr;
        bool isResident;
        bool isHIR;
        bool isLIR;
    public:
        size_t getRecency() {
            return recency;
        }
        void setRecency(size_t new_recency) {
            recency = new_recency;
        }

    public:
        size_t getIRR() {
            return irr;
        }
        void setIRR(size_t new_irr) {
            irr = new_irr;
        }

    public:
        bool getResidency() {
            return isResident;
        }
        void setResidency(bool val) {
            isResident = val;
        }

    public:
        bool getHIR() {
            return isHIR;
        }
        void setHIR(bool val) {
            isHIR = val;
        }

    public:
        bool getLIR() {
            return isLIR;
        }
        void setLIR(bool val) {
            isLIR = val;
        }

};

template <typename T = LIRSPage, typename KeyT = int>
class LIRSCache {
    private:
        using lstIter = typename std::list<std::pair<KeyT, T>>::iterator;
        std::list<std::pair<KeyT, T>> lowInterSet;
        std::list<std::pair<KeyT, T>> highInterSet;
        std::list<std::pair<KeyT, T>> dataList;
        std::unordered_map<KeyT, lstIter> hashMap;
        size_t capacity;

    public:
        LIRSCache(size_t lirCap = DEFAULT_LIR_CAP, size_t hirCap = DEFAULT_HIR_CAP);
        void insert(KeyT key, T& elem);
        T& get(KeyT key);
        void prune();
};


template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::insert(KeyT key, T elem) {
    std::pair<KeyT, T> toInsert(key, elem); // data to insert
    if (lowInterSet.size() <= capacity) {
        lowInterSet.push_front(toInsert);
        hashMap[key] = lowInterSet.begin();
    } else if (!T.getRecency().has_value() || (T.getRecency() >= RECENCY_THRESHOLD) {
        T.setHIR(true);
        T.setRecency(1);
    }
}

template<typename T, typename KeyT>
T& LIRSCache<T, KeyT>::get(KeyT key) {
    auto it = hashMap.find(key);
    if (it != hashMap.end()) {
        if (it->second.getLIR()) {    // LIR meet case
            lowInterSet.splice(lowInterSet.begin(), lowInterSet, it);
            hashMap[key] = lowInterSet.begin();
            prune();
        } else if (it->second.getHIR()) {
            if isInLIR(it) {

                // erase it from HIR and swap statuses
                highInterSet.erase(it->second);
                it->second.setLIR(true);
                it->second.setHIR(false);

                // erase last LIR block and move it to the end of HIR, then prune
                auto lastIt = std::prev(lowInterSet.end());

                lastIt.setHIR(true);
                lastIt.setLIR(false);
                lowInterSet.splice(highInterSet.end(), lowInterSet, lastIt);
                prune();
            } else {
                // leaving it as an hir-block and moving to the end of HIR
                highInterSet.splice(highInterSet.end(), highInterSet, it);
            }
        }
    }
}


// this function finds HIR resident blocks in LIR stack until there is an LIR block
template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::prune() {
    for (auto it = lowInterSet.rbegin(); it != lowInterSet.rend(); it++) {
        if (it->second.getHIR()) {
            lowInterSet.erase(it);
        } else {
            break;
        }
    }
}
