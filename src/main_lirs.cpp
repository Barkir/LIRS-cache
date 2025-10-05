#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "main.hpp"
#include "lirs.hpp"

int main() {

    struct testingInterface testing = getTestingInfo(LIRS_MODE);

    LIRSCache<int> lirs(testing.lir_size, testing.hir_size);
    for (auto key : testing.sequence) {
        if (lirs.getWithStats(key) == nullptr) {
            lirs.insert(key, key * 100);
        }
    }
    lirs.printCacheStats();
}
