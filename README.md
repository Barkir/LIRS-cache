## Caches

1. clone this repository

```
git clone https://github.com/Barkir/VladimirovTasks
```
2. create build directory

```
mkdir build
```

3. run cmake from build dir

```
cd build
cmake ..
```

4. compile from build dir
```
cmake --build .
```

## How to test?
Write your tests in `tests` directory in this fmt

```
1 : TEST_NAME (no spaces!!!)
2 : cache_size
3 : number of page calls
4 : call seqeunce
5 : number of hits you wait for
```

Then make `run_tests.sh` executable
```
chmod +x run_tests.sh
```

and run it

```
./run_tests.sh
```


## LIRS tests implementation

Testing infrastructure of *LIRS* cache will differ from *LRU*.
There is a class called _LIRS Stats_ whhere we collect these data:

- `size_t hits`
- `size_t misses`

The LIRS cache will have two `get` functions - one is for regular use and one is for testing.

```cpp
T& get(int key) {
    return getImpl(key, true);
}

T& getWithStats(int key) {
    return getImpl(key, false)
}
```

## References
[MySQL LIRS implementation source code](https://www.iskm.org/mysql56/pgman_8hpp_source.html)

[LIRS wiki](https://en.wikipedia.org/wiki/LIRS_caching_algorithm)

[LIRS article](https://ranger.uta.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)

[my attempts to figure out what's going on :| ](./lirs_tryhard.md)




