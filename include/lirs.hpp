#pragma once

#include <stack>
#include <queue>
#include <list>
#include <optional>
#include <unordered_map>
#include <iomanip>
#include <functional>

#include "stats.hpp"

const size_t DEFAULT_LIR_CAP = 16;
const size_t DEFAULT_HIR_CAP = 2;
const size_t RECENCY_THRESHOLD = 128;

// #define DEBUG_MODE
#ifdef DEBUG_MODE

#define ON_DEBUG(code) code

#else

#define ON_DEBUG(code)

#endif

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
        typename std::list<T>::iterator stackIt;
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

        using slowGetFunc = std::function<T(const KeyT &)>;
        using lst = typename std::list<std::pair<KeyT, LIRSPage<T>>>;
        using lstIter = lst::iterator;

// -----------------------------
        lst lowInterSet;
        lst highInterSet;
        lst dataList;
        std::unordered_map<KeyT, lstIter> hashMap;
// -----------------------------

        size_t lir_capacity;
        size_t hir_capacity;
        slowGetFunc slowFunc;

        CacheStats stats;

        bool atTheBottom(auto it) {
            if (lowInterSet.empty())
                return false;
            return it->second == std::prev(lowInterSet.end());
        }

        lstIter onStack(KeyT key) {
            auto result = hashMap.find(key);
            if (result != hashMap.end())
                return result->second;
            // for (auto lit = lowInterSet.begin(); lit != lowInterSet.end(); lit++) {
            //     if (lit->first == key) {
            //         return lit;
            //     }
            // }
            return lowInterSet.end();
        }

        lstIter onQueue(KeyT key) {
            auto result = hashMap.find(key);
            if (result != hashMap.end())
                return result->second;
            // for (auto hit = highInterSet.begin(); hit != highInterSet.end(); hit++) {
            //     if (hit->first == key) {
            //         return hit;
            //     }
            // }
            return highInterSet.end();
        }

    public:
        LIRSCache(size_t lirCap = DEFAULT_LIR_CAP, size_t hirCap = DEFAULT_HIR_CAP, slowGetFunc slowFuncIn = nullptr) : lir_capacity(lirCap), hir_capacity(hirCap), slowFunc(slowFuncIn) {};
        void insert(KeyT key, T elem);

        LIRSPage<T>* getFunc(KeyT key, bool state);

        LIRSPage<T>* get(KeyT key);
        LIRSPage<T>* getWithStats(KeyT key);

        void printCacheStats();
        void printCacheHits();

        void evictIfNeeded(size_t count);
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
void LIRSCache<T, KeyT>::printCacheStats() {
    std::cout << "Total hits: " << stats.getHits() << "\n";
    std::cout << "Total misses: " << stats.getMisses() << "\n";
    std::cout << "hitRate: " << stats.getHitRate() << "\n";
}

template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::printCacheHits() {
    std::cout << stats.getHits() << "\n";
}

template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::insert(KeyT key, T elem) {
    ON_DEBUG(std::cout << "===LIRSCache insert method===\n");

    std::pair<KeyT, LIRSPage<T>> toInsert(key, elem); // data to insert

    ON_DEBUG(std::cout << "Inserting data: {" << key << elem << "}\n");

    if (lowInterSet.size() <= lir_capacity) { // empty hash case

        ON_DEBUG(std::cout << "# empty hash case\n");

        toInsert.second.setHot(true);
        toInsert.second.setResident(true);

        ON_DEBUG(std::cout << "\t set to hot, set to resident\n");

        lowInterSet.push_front(toInsert);

        ON_DEBUG(std::cout << "\t pushed it to the front of the stack S\n");

        hashMap[key] = lowInterSet.begin();

        ON_DEBUG(std::cout << "\t hashMap[key] = lowInterSet.begin()\n");
    } else if (highInterSet.size() <= hir_capacity) {
        ON_DEBUG(std::cout << "# pushing to highInterSetCase case\n");
        highInterSet.push_front(toInsert);
        ON_DEBUG(std::cout << "\t highInterSet.push_front(toInsert)\n");
    } else {
        ON_DEBUG(std::cout << "# no place in highInterSet case\n");
        highInterSet.pop_back();
        highInterSet.push_front(toInsert);
        ON_DEBUG(std::cout << "\t popped back of the list Q, highIntetSet.push_front(toInsert)\n");

    }
}

template<typename T, typename KeyT>
LIRSPage<T>* LIRSCache<T, KeyT>::get(KeyT key){
    return getFunc(key, false);
}

template<typename T, typename KeyT>
LIRSPage<T>* LIRSCache<T, KeyT>::getWithStats(KeyT key){
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
LIRSPage<T>* LIRSCache<T, KeyT>::getFunc(KeyT key, bool testState) {
    ON_DEBUG(std::cout << "===LIRSCache getFunc method (testState =" << testState << ")===\n");
    auto it = hashMap.find(key);
    if (it != hashMap.end()) { // page can be hot or cold resident
        ON_DEBUG(std::cout << "# hot or cold resident case (found elem in hashmap)\n");
        if (it->second->second.getHot()) { // hot page case
            ON_DEBUG(std::cout << "\t # hot page case\n");
            if (atTheBottom(it)) { // if at the stack bottom
                ON_DEBUG(std::cout << "\t\t # at the bottom of the stack case\n");
                lowInterSet.splice(lowInterSet.begin(), lowInterSet, it->second);
                ON_DEBUG(std::cout << "\t\t\t moving it to the beginning of lowInterSet\n");
                prune();
                ON_DEBUG(std::cout << "\t\t\t pruning... \n");
                if (testState)
                    stats.recordHit();
                return &it->second->second;
            } else {
                ON_DEBUG(std::cout << "\t\t # not at the bottom of the stack case\n");
                lowInterSet.splice(lowInterSet.begin(), lowInterSet, it->second);
                ON_DEBUG(std::cout << "\t\t\t moving it to the beginning of lowInterSet\n");
                if (testState)
                    stats.recordHit();
                return &it->second->second;
            }
        } else { // cold resident page case
            ON_DEBUG(std::cout << "\t # cold resident page case\n");
            if (auto new_it = onStack(key); new_it != lowInterSet.end()) {
                ON_DEBUG(std::cout << "\t\t # cold resident page on stack case\n");
                new_it->second.setHot(true);
                ON_DEBUG(std::cout << "\t\t\t setting it to hot page\n");
                highInterSet.erase(new_it);
                ON_DEBUG(std::cout << "\t\t\t erasing from Q\n");
                lowInterSet.back().second.setHot(false);
                ON_DEBUG(std::cout << "\t\t\t setting back of lowInterSet to cold\n");
                lowInterSet.splice(highInterSet.end(), lowInterSet, lowInterSet.end()); // moving from lowInterSet end to highInterSet end
                ON_DEBUG(std::cout << "\t\t\t moving back of lowInterSet to highInterSet\n");
                prune();
                ON_DEBUG(std::cout << "\t\t\t pruning...\n");
                if (testState)
                    stats.recordMiss();
                return &new_it->second;

            } else {
                ON_DEBUG(std::cout << "\t\t # cold resident page not on stack case\n");
                highInterSet.splice(highInterSet.end(), highInterSet, new_it); // leaving it cold and moving to the end of the queue
                ON_DEBUG(std::cout << "\t\t\t moving page to the end of the queue\n");
                if (testState)
                    stats.recordMiss();
                return &new_it->second;
            }

        }
    } else { // page can be cold non-resident
        ON_DEBUG(std::cout << "# cold non-resident page case\n");
        if (auto new_it = onStack(key); new_it != lowInterSet.end()) {
            ON_DEBUG(std::cout << "\t\t # cold non-resident page on stack case\n");
            new_it->second.setHot(true);
            ON_DEBUG(std::cout << "\t\t\t setting it to hot page\n");
            highInterSet.erase(new_it);
            ON_DEBUG(std::cout << "\t\t\t erasing from Q\n");
            lowInterSet.back().second.setHot(false);
            ON_DEBUG(std::cout << "\t\t\t setting back of lowInterSet to cold\n");
            highInterSet.splice(highInterSet.end(), lowInterSet, lowInterSet.end());
            ON_DEBUG(std::cout << "\t\t\t moving back of lowInterSet to highInterSet\n");
            prune();
            ON_DEBUG(std::cout << "\t\t\t pruning...\n");
            if (testState)
                stats.recordMiss();
            return &new_it->second;
        } else {
            ON_DEBUG(std::cout << "\t\t # cold non-resident page not on stack case\n");
            if (auto new_it = onQueue(key); new_it != highInterSet.end()) {
                highInterSet.splice(highInterSet.end(), lowInterSet, new_it);
                ON_DEBUG(std::cout << "\t\t\t moving page to the end of the queue\n");
                if (testState)
                    stats.recordMiss();
                return &new_it->second;
            }
        }
    }
    return nullptr;
}



// this function finds HIR resident blocks in LIR stack until there is an LIR block
template<typename T, typename KeyT>
void LIRSCache<T, KeyT>::prune() {
    for (auto it = lowInterSet.end(); it != lowInterSet.begin(); it--) {
        if (auto smth = onQueue(it->first); smth != highInterSet.end()) {
            if (it != lowInterSet.end())
                lowInterSet.erase(it);
            break;
        } else {
            break;
        }
    }
}
