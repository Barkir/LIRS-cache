#pragma once

#include <unordered_map>
#include <list>
#include <queue>

template <typename T>
class WhateverPage {
    private:
        int pageNum;
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

template <typename T, typename KeyT  = int>
class WhateverCache {
    private:
        using lstIter = typename std::list<T>::iterator;
        std::unordered_map<KeyT, lstIter> hashMap;
        std::list<std::pair<KeyT, T>> dataList;
        size_t capacity;

    public:
        WhateverCache(size_t cap = 4) : capacity(cap) {
            std::cout << "WhateverCache created with capacity " << cap << std::endl;
        }

        void insert(KeyT key, T elem) {
            dataList.push_front(elem);

            auto it = hashMap.find(key);
            if (it != hashMap.end()) {

            }

            if (hashMap.size() >= capacity) {
                auto lastElem = dataList.end();
                KeyT lruKey = lastElem->first();
                hashMap.erase(lruKey);

                // move the last recently used element to the beginning
                dataList.splice(dataList.begin(), dataList, lastElem);
            }


        }
};
