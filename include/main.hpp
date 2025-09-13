#include <vector>

struct testingInterface {

    std::string test_name;
    size_t cache_size;
    size_t numelements;
    std::vector<int> sequence;
    size_t hits;
};
