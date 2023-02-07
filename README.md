# 遗传算法对”到家“服务派单进行优化
提供了一套用遗传算法对到家服务进行派单的方案，业务背景https://juejin.cn/post/7071934813292199949。

## 说明
1. data目录下存放了task和worker相关数据；
2. Gene、Chromosome分别定义了基因和染色体的结构和操作算子，Ga中对初始化、演化等遗传操作做了进一步封装。
3. Postion、Task、Worker定义了一些元数据结构。
4. data_reader中定义了一些读取数据的方法。
