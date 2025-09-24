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
        bool isHot;
        bool isResident;

    // constructors
    public:
        LIRSPage(const T& initialData) : data(initialData), recency(std::nullopt), irr(std::nullopt),
            isHot(false), isResident(false) {}

    // get functions
    public:
        bool getHot() {
            return isHot;
        }

        bool getResident() {
            return isResident;
        }

    // set functions
    public:
        void setHot(bool state) {
            isHot = state;
        }

        void setResident(bool state) {
            isResident = state;
        }
};

template <typename T, typename KeyT = int>
class LIRSCache {
    private:
        using lstIter = typename std::list<std::pair<KeyT, LIRSPage<T>>>::iterator;
        std::list<std::pair<KeyT, LIRSPage<T>>> lowInterSet;
        std::list<std::pair<KeyT, LIRSPage<T>>> highInterSet;
        std::list<std::pair<KeyT, LIRSPage<T>>> dataList;
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
    if (lowInterSet.size() <= capacity) { // empty hash case
        lowInterSet.push_front(toInsert);
        hashMap[key] = lowInterSet.begin();
    } else if (!T.getRecency().has_value() || (T.getRecency() >= RECENCY_THRESHOLD) {
        T.setHIR(true);
        T.setLIR(false);
        T.setRecency(1);
    }
}


// stack contains hot pages (always hit) and referenced cold entries (resident or not)
// most recent recency is on top
// entries which are less recent than the least recent hot page are removed ("stack pruning"). So the bottom page is always hot.

// cold entries on the stack are undergoing a "trial period". if they are referenced soon again, they become hot.
// otherwise they fall off the bottom of the stack.

// secondary data structure is the "queue". It contains all resident cold pages (on stack or not).
// when a hot page is removed from the stack it is added to the end of the queue.
// when page replacement needs a page it removes it from the front of the queue.
template<typename T, typename KeyT>
T& LIRSCache<T, KeyT>::get(KeyT key) {
    auto it = hashMap.find(key);
    if (it != hashMap.end()) { // page can be hot or cold resident
        if (it->second.getHot()) { // hot page case
            if (atTheBottom(it)) { // if at the stack bottom
                moveToTheTop(it);
                prune()
            } else {
                moveToTheTop(it);
            }
        } else { // cold resident page case
            if (onStack(it)) {
                it->second.setHot(true);
                removeFromQueue(it);
                lowInterSet.back().second.setHot(false);
                moveFromTo(lowInterSet.back(), highInterSet.end());
                prune();
            } else {
                moveFromTo(it, highInterSet.end()) // leaving it cold and moving to the end of the queue
            }

        }
    } else { // page can be cold non-resident
        if (onStack(it)) {
            it->second.setHot(true);
            lowInterSet.back().second.setHot(false);
            moveFromTo(lowInterSet.back(), highInterSet.end());
            prune();
        } else {
            moveFromTo(it, highInterSet.end());
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
