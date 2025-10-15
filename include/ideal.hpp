#pragma once

#include <unordered_map>
#include <queue>
#include <functional>

#include "stats.hpp"

template <typename T, typename KeyT = int>
class IdealCache {
    private:
        using keyVec = std::vector<KeyT>;
        using slowGetFunc = std::function<T(const KeyT &)>;
        using cacheIter = std::pair<KeyT, T>;

        size_t cap;
        size_t ind;
        std::unordered_map<KeyT, T> cache;
        std::unordered_map<KeyT, std::queue<size_t>> keyPos;

        CacheStats stats;


        slowGetFunc slow_get_elem;


    public:
        IdealCache(size_t capIn, slowGetFunc slow_get_elemIn, keyVec &elems) : cap(capIn), slow_get_elem(slow_get_elemIn) {
            for (size_t i = 0; i < elems.size(); i++) {
                keyPos[elems[i]].push(i);
            }
        }

        bool lookup_update(const KeyT &key) {
            ind++;
            auto &key_queue = keyPos[key];
            key_queue.pop();

            if (cache.contains(key))
                stats.recordHit();
                return true;

            if (key_queue.empty())
                stats.recordMiss();
                return false;

            if (cache.size() == cap) {
                if (eraseElem(key)) {
                    cache.emplace(key, slow_get_elem(key));

                    stats.recordMiss();
                    return false;
                }
            }
        }

        void printCacheHits() {
            std::cout << stats.getHits() << "\n";
        }

        private:
            bool eraseElem(const KeyT &key) {
                size_t maxDist = keyPos[key].front() - ind;
                bool found = false;
                auto eraseIt = cache.end();

                for (auto it = cache.begin(); it != cache.end(); it++) {
                    auto &key_queue = keyPos[it->first];

                    if (key_queue.empty()) {
                        eraseIt = it;
                        found = true;
                        break;
                    }

                    size_t dist = key_queue.front() - ind;

                    if (dist > maxDist) {
                        maxDist = dist;
                        eraseIt = it;
                        found = true;
                    }

                    if (found) {
                        cache.erase(eraseIt);
                    }

                    return found;
                }
            }
};
