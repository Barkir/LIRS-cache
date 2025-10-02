#pragma once

#include <stack>
#include <queue>
#include <list>
#include <optional>
#include <unordered_map>
#include <iomanip>

const size_t DEFAULT_LIR_CAP = 16;
const size_t DEFAULT_HIR_CAP = 2;
const size_t RECENCY_THRESHOLD = 128;



class CacheStats {
    private:
        size_t hits = 0;
        size_t misses = 0;

    // record functions
    public:
        void recordHit() {
            hits++;
        }
        void recordMiss() {
            misses++;
        }

    // get functions
    public:
        size_t getHits() {
            return hits;
        }

        size_t getMisses() {
            return misses;
        }

    // analyzing
    public:
        double getHitRate() const {
        return (hits + misses) > 0 ? static_cast<double>(hits) / (hits + misses) : 0.0;
    }
};

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
        bool getHot() const {
            return isHot;
        }

        bool getResident() const {
            return isResident;
        }

        T getData() const {
            return data;
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
        size_t lir_capacity;
        size_t hir_capacity;

        CacheStats stats;

        bool atTheBottom(auto it) {
            if (lowInterSet.empty())
                return false;
            return it == std::prev(lowInterSet.end());
        }

        T* onStack(KeyT key) {
            for (auto lit = lowInterSet.begin(); lit != lowInterSet.end(); lit++) {
                if (lit->first == key) {
                    return lit;
                }
            }
        }

    public:
        LIRSCache(size_t lirCap = DEFAULT_LIR_CAP, size_t hirCap = DEFAULT_HIR_CAP) : lir_capacity(lirCap), hir_capacity(hirCap) {};
        void insert(KeyT key, T elem);

        T* getFunc(KeyT key, bool state);

        T* get(KeyT key);
        T* getWithStats(KeyT key);

        void printCache();
        void prune();

    public:
    friend std::ostream& operator<<(std::ostream& os, const LIRSCache<T, KeyT>& cache) {
        os << "\n┌─────────────────────────────────┐" << std::endl;
        os << "│          LIRS CACHE STATE       │" << std::endl;
        os << "└─────────────────────────────────┘" << std::endl;

        os << "Capacity: LIR=" << cache.lir_capacity
           << ", HIR=" << cache.hir_capacity
           << ", Used: " << cache.hashMap.size() << "/" << cache.lir_capacity << std::endl;

        os << "\nLIR STACK (Hot pages, most recent first):" << std::endl;
        os << "┌─────┬──────┬───────┬──────────┐" << std::endl;
        os << "│ Pos │ Key  │ Value │   Type   │" << std::endl;
        os << "├─────┼──────┼───────┼──────────┤" << std::endl;

        int pos = 0;
        for (const auto& item : cache.lowInterSet) {
            os << "│ " << std::setw(3) << pos++ << " │ "
               << std::setw(4) << item.first << " │ "
               << std::setw(5) << item.second.getData() << " │ "
               << (item.second.getHot() ? "  LIR    " : "  HIR    ") << "│" << std::endl;
        }
        os << "└─────┴──────┴───────┴──────────┘" << std::endl;

        os << "\nHIR QUEUE (Cold residents):" << std::endl;
        os << "┌─────┬──────┬───────┐" << std::endl;
        os << "│ Pos │ Key  │ Value │" << std::endl;
        os << "├─────┼──────┼───────┤" << std::endl;

        pos = 0;
        for (const auto& item : cache.highInterSet) {
            os << "│ " << std::setw(3) << pos++ << " │ "
               << std::setw(4) << item.first << " │ "
               << std::setw(5) << item.second.getData() << " │" << std::endl;
        }
        os << "└─────┴──────┴───────┘" << std::endl;

        return os;
    }
};


template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::insert(KeyT key, T elem) {
    std::pair<KeyT, LIRSPage<T>> toInsert(key, elem); // data to insert
    if (lowInterSet.size() <= lir_capacity) { // empty hash case

        toInsert.second.setHot(true);
        toInsert.second.setResident(true);

        lowInterSet.push_front(toInsert);
        hashMap[key] = lowInterSet.begin();
    } else if (highInterSet.size() <= hir_capacity) {
        highInterSet.push_front(toInsert);
    } else {
        highInterSet.pop_back();
        highInterSet.push_front(toInsert);
    }
}

template<typename T, typename KeyT>
T* LIRSCache<T, KeyT>::get(KeyT key){
    return getFunc(key, false);
}

template<typename T, typename KeyT>
T* LIRSCache<T, KeyT>::getWithStats(KeyT key){
    return getFunc(key, true);
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
T* LIRSCache<T, KeyT>::getFunc(KeyT key, bool testState) {
    auto it = hashMap.find(key);
    if (it != hashMap.end()) { // page can be hot or cold resident
        if ((it->second)->second.getHot()) { // hot page case
            if (atTheBottom(it)) { // if at the stack bottom
                lowInterSet.splice(lowInterSet.front(), lowInterSet, it->second->second);
                prune();
                if (testState) stats.recordHit();
                return it;
            } else {
                lowInterSet.splice(lowInterSet.front(), lowInterSet, it->second->second);
                if (testState) stats.recordHit();
                return it;
            }
        } else { // cold resident page case
            if (auto new_it = onStack(key)) {
                new_it->second.setHot(true);
                highInterSet.erase(*new_it);
                lowInterSet.back().second.setHot(false);
                lowInterSet.splice(highInterSet.end(), lowInterSet, lowInterSet.back()); // moving from lowInterSet end to highInterSet end
                prune();
                if (testState) stats.recordMiss();
                return new_it;

            } else {
                highInterSet.splice(highInterSet.end(), highInterSet, *new_it); // leaving it cold and moving to the end of the queue
                if (testState) stats.recordMiss();
                return new_it;
            }

        }
    } else { // page can be cold non-resident
        if (auto new_it = onStack(key)) {
            new_it->second.setHot(true);
            lowInterSet.back().second.setHot(false);
            highInterSet.splice(highInterSet.end(), lowInterSet, lowInterSet.end());
            prune();
            if (testState) stats.recordMiss();
        } else {
            highInterSet.splice(highInterSet.end(), lowInterSet, *new_it);
            if (testState) stats.recordMiss();
            return new_it;
        }
    }

    return nullptr;
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
