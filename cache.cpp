#include <unordered_map>
#include <list>

template <typename T, typename KeyT = int>
struct cache_t {
    size_t sz;
    std::list<T> cache_;

    using ListIt = typename std::list<T>::iterator;
    std::unordered_map<KeyT, ListIt> hash_;

    bool full() const;

    template <typename F>
    bool lookup_update(KeyT key, F slow_get_page)
};

class WhateverPage {
    private:
        size_t pageNum;
        T pageData;

    public:
        WhateverPage(size_t number, const T& data) : pageNum(number), pageData(data) {}

        int getPageNum() const {
            return pageNum;
        }

        T getPageData() const {
            return pageData;
        }

        void setData(const T& data) {
            pageData = data;
        }

        void dumpPage () const {
            std::cout << "#" << pageNum << " : " << pageData << std::endl;
        }
};

class WhateverCache {
    private:
        using lstIter = typename std::list<T>::iterator;
        std::unordered_map<KeyT, lstIter> hashMap;
        std::list<T> dataList;

    public:

};
