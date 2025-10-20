<div align="center">
LIRS, LRU and Ideal cache Implementation

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Bash Script](https://img.shields.io/badge/bash_script-%23121011.svg?style=for-the-badge&logo=gnu-bash&logoColor=white)


</div>


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

5. you will get two targets - `lirs` and `lru` (hopefully).

## How to test LRU?
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



---

## LIRS tests implementation

Testing infrastructure of *LIRS* cache will differ from *LRU*.
There is a class called _LIRS Stats_ whhere we collect these data:

- `size_t hits`
- `size_t misses`

The LIRS cache will have two `get` functions - one is for regular use and one is for testing.

```cpp
T& get(int key) {
    return getFunc(key, true);
}

T& getWithStats(int key) {
    return getFunc(key, false)
}
```

### How to test LIRS?
To write a test - use this format:
```
{lir_size} {hir_size} {number of calls} {call sequence}
```

Then run program like this:
```
./lirs < test.txt
```

Then it prints you this info:
1. Total hits
2. Total misses
3. Hit Rate

That's all 4 today.


## References
[MySQL LIRS implementation source code](https://www.iskm.org/mysql56/pgman_8hpp_source.html)

[LIRS wiki](https://en.wikipedia.org/wiki/LIRS_caching_algorithm)

[LIRS article](https://ranger.uta.edu/~sjiang/pubs/papers/jiang02_LIRS.pdf)

[my attempts to figure out what's going on :| ](./lirs_tryhard.md)




