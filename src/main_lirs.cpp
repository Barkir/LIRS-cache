#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

#include "main.hpp"
#include "lirs.hpp"

int main() {

    struct testingInterface testing = getTestingInfo();

    LIRSCache<int> lirs;
    for (auto key : testing.sequence) {
        std::cout << "wtf" << "\n";
        if (lirs.getWithStats(key) == nullptr) {
            lirs.insert(key, key * 100);
        }
    }
    std::cout << "wtf" << "\n";
}
