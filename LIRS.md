Low Inter-reference Recency Set (_низкий уровень межссылочной давности (что?)_)

Работает быстрее чем **LRU**.
Достигается использованием _reuse distance_,  как основной метрики.

В то время как все алгоритмы полагаются на существование _[locality of reference](https://en.wikipedia.org/wiki/Locality_of_reference)_
_Reuse distance of a page_ - the number of distinct pages accessed between two consecutive references of the page.

Короче по-русски. Мы используем количество страниц, к которым мы обратились между двумя последовательными обращениями к странице.
Если мы обратились к странице всего один раз, то ее _reuse distance_ бесконечен.

_LIRS_ uses _last_ and _second-to-last_ references.

If a page is accessed for the first time, its reuse distance is infinite.

## Illustration :)
```
 . . . B . . . B . . . . . . . . . . B . . . . .
               ^----Reuse Distance---^--Recency--^
               T1                    T2          T3
```


## How to select the replacement victim (≖_≖ )


1. Cache is divided into _Low Inter-reference Recency (LIR)_ and _High Inter-reference Recency (HIR)_ partition.
2. LIR partition holds the majority of cache. All LIR pages are resident in page
3. All recently accessed pages are placed in a _FIFO_ queue called the LIRS stack (stack S). And HIR pages are also placed in another FIFO queue (stack Q).
4. An accesed page is moved to the top of Stack S and any HIR pages at the stack's bottom are removed.




![[LIRS_replacement_operations.png]]
