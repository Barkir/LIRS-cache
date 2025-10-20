#pragma once

#include <unordered_map>
#include <queue>
#include <functional>
#include <vector>
#include <iostream>

template <typename T, typename KeyT = int>
class IdealCache {
private:
    using keyVec = std::vector<KeyT>;
    using slowGetFunc = std::function<T(const KeyT &)>;
    using cacheIter = typename std::unordered_map<KeyT, T>::iterator;

    size_t cap;
    size_t ind = 0;
    std::unordered_map<KeyT, T> cache;
    std::unordered_map<KeyT, std::queue<size_t>> keyPos;

    struct CacheStats {
        size_t hits = 0;
        size_t misses = 0;

        void recordHit() { hits++; }
        void recordMiss() { misses++; }
        size_t getHits() const { return hits; }
        size_t getMisses() const { return misses; }
    };

    CacheStats stats;
    slowGetFunc slow_get_elem;

public:
    IdealCache(size_t capIn, slowGetFunc slow_get_elemIn, keyVec &elems)
        : cap(capIn), slow_get_elem(slow_get_elemIn) {
        for (size_t i = 0; i < elems.size(); i++) {
            keyPos[elems[i]].push(i);
        }
    }

    bool lookup_update(const KeyT &key) {
        auto &key_queue = keyPos[key];

        if (!key_queue.empty() && key_queue.front() == ind) {
            key_queue.pop();
        }

        if (cache.find(key) != cache.end()) {
            stats.recordHit();
            ind++;
            return true;
        }

        stats.recordMiss();

        if (key_queue.empty()) {
            ind++;
            return false;
        }

        if (cache.size() < cap) {
            cache[key] = slow_get_elem(key);
            ind++;
            return false;
        }

        if (eraseElem(key)) {
            cache[key] = slow_get_elem(key);
        }

        ind++;
        return false;
    }

    void printCacheHits() {
        std::cout << stats.getHits() << "\n";
    }

    void printCacheInfo() {
        std::cout << "Capacity: " << cap << "\n";
        std::cout << "Index: " << ind << "\n";
        std::cout << "Hits: " << stats.getHits() << ", Misses: " << stats.getMisses() << "\n";
    }

private:
    bool eraseElem(const KeyT &newKey) {
        if (cache.empty()) return false;

        auto victim = cache.end();
        size_t maxDist = 0;
        bool found = false;

        for (auto it = cache.begin(); it != cache.end(); ++it) {
            auto &key_queue = keyPos[it->first];

            if (key_queue.empty()) {
                victim = it;
                found = true;
                break;
            }

            // Calculate distance to next use
            size_t dist = key_queue.front() - ind;
            if (dist > maxDist) {
                maxDist = dist;
                victim = it;
                found = true;
            }
        }

        auto &newKey_queue = keyPos[newKey];
        if (!newKey_queue.empty()) {
            size_t newKeyDist = newKey_queue.front() - ind;

            if (found && newKeyDist < maxDist) {
                cache.erase(victim);
                return true;
            }
        }

        if (!found) {
            return false;
        }

        cache.erase(victim);
        return true;
    }
};
