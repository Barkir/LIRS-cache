#include <vector>

enum testingModes {
    LRU_MODE,
    LIRS_MODE
};

struct testingInterface {

    std::string test_name;
    size_t cache_size;
    size_t numelements;
    std::vector<int> sequence;
    size_t hits;


    // some place for LIRS cache
    size_t lir_size;
    size_t hir_size;

};

struct testingInterface getTestingInfo(enum testingModes mode) {

    struct testingInterface testing = {};
    switch (mode) {
        case LRU_MODE:  std::cin >> testing.test_name;
                        std::cin >> testing.cache_size;
                        std::cin >> testing.numelements;

                        for (size_t i = 0; i < testing.numelements; i++) {
                            int element = 0;
                            std::cin >> element;
                            testing.sequence.push_back(element);
                        }

                        std::cin >> testing.hits;
                        return testing;

        case LIRS_MODE: std::cin >> testing.lir_size;
                        std::cin >> testing.hir_size;
                        std::cin >> testing.numelements;

                        for (size_t i = 0; i < testing.numelements; i++) {
                            int element = 0;
                            std::cin >> element;
                            testing.sequence.push_back(element);
                        }
                        return testing;
    }
}
