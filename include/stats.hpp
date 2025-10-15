#pragma once

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
