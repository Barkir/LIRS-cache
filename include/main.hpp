#include <vector>

struct testingInterface {

    std::string test_name;
    size_t cache_size;
    size_t numelements;
    std::vector<int> sequence;
    size_t hits;

};

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
