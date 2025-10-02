#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "main.hpp"
#include "lirs.hpp"

int main() {

    struct testingInterface testing = getTestingInfo();

    LIRSCache<int> lirs;
    for (auto it = testing.sequence.begin(); it != testing.sequence.end(); it++) {
        int key = *it;
        if (lirs.getWithStats(key) == nullptr) {
            lirs.insert(key, key * 100);
        }
    }
    std::cout << "wtf" << "\n";
}
