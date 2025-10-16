#include <functional>

template <typename cache_T, typename KeyT = int>
class VizCache {
    private:
        using getFuncT = std::function<T (const KeyT&)>;
        using insertFuncT = std::function<T (const KeyT&, T&)>;

        getFuncT getFunc_;
        insertFuncT insertFunc_;

    public:
        VizCache(getFuncT getFunc, insertFuncT insertFunc) : getFunc_(getFunc), insertFuncT_(insertFunc) {}
    public:
        bool get(const KeyT& key) {
            T elem = getFunc_(key);

            /* update vizualization */

        }

        bool insert(const KeyT& key, T& elem) {
            insertFunc_(key, elem);

            /* update vizualization */
        }
}
