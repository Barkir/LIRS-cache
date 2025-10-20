#include <iostream>

#include "ideal.hpp"
#include "main.hpp"

int slow_get_elem(int key) {
    return key;
}

int main() {

    struct testingInterface testing = getTestingInfo(IDEAL_MODE);

    IdealCache<int, int> ideal(testing.cache_size, slow_get_elem, testing.sequence);
    for (auto key : testing.sequence) {
        // ideal.printCacheInfo();
        ideal.lookup_update(key);
    }
    ideal.printCacheHits();

}
