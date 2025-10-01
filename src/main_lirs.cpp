#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "main.hpp"
#include "lirs.hpp"

struct testingInterface getTestingInfo() {
    struct testingInterface testing = {};

    std::cin >> testing.test_name;
    std::cin >> testing.cache_size;
    std::cin >> testing.numelements;

    for (size_t i = 0; i < testing.numelements; i++) {
        int element = 0;
        std::cin >> element;
        testing.sequence.push_back(element);
    }

    std::cin >> testing.hits;
    return testing;
}

int main() {
    LIRSCache<int> lirs;
    // std::cout << lirs << "\n";

// =============================================================
//     Testing infrastructure for LRU cache (./run_tests.sh)
// =============================================================
    struct testingInterface testing = getTestingInfo();
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    WhateverCache<int> test_cache(testing.cache_size);
    for (auto it = testing.sequence.begin(); it != testing.sequence.end(); it++) {
        int key = *it;
        if (test_cache.get(key) == nullptr) {
            test_cache.insert(key, key * 100);
        }
    }
    std::cout.rdbuf(old);
    std::string result = buffer.str();
    size_t hits_count = std::count(result.begin(), result.end(), '\n');
    if (hits_count == testing.hits) {
        std::cout << testing.test_name << " PASSED! (" << hits_count << " hits)" "\n";
        return 0;
    } else {
        std::cout << testing.test_name << " FAILED! (" << hits_count <<" instead " << testing.hits << "hits)" << "\n";
        return -1;
    }

}
