#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "main.hpp"
#include "lirs.hpp"

int main() {

    struct testingInterface testing = getTestingInfo(LIRS_MODE);

    LIRSCache<int> lirs(testing.cache_size, testing.cache_size / 100);
    for (auto key : testing.sequence) {
        if (lirs.getWithStats(key) == nullptr) {
            lirs.insert(key, key * 100);
        }
    }
    lirs.printCacheHits();
}
