- there is a simple implementation of a page here

```cpp
struct page {
    int index;  // page index
    int sz;     // page size
    char *data; // page data
}

void slow_get_page(int index, struct page *p);
```

- solution to boost it - cache

we want to save the pages we actually want to save (the ones we use more frequently)

for example we have a place for 4 pages and we have such requests

```
1, 2, 3, 4, 1, 2, ...
```

_what's the best strategy to choose for caching???_

## LRU (Least Recently Used)
- if the element is found, it moves to the head
- if not - it moves

