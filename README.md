# 一种多目标调度优化的遗传算法实现
提供了一种用遗传算法解决多目标优化的样例，具体业务背景请见：https://juejin.cn/post/7071934813292199949/

## 说明
1. 运行时需要指定运行路径为项目路径，用于从data目录下读取station和worker数据，stationInfo.csv和workerInfo.csv下各提供了一条样例数据，可以参照样例数据格式造一批数进行测试；
2. constant_param下定义了一些常量与系数，io_utils下定义了从data目录下读取样例数据的方法；
3. Postion、Station、worker分别定义了用于封装位置、站点与操作员信息的类，StationNode下定义了链表节点；
4. GA中定义了用于生成初始种群、进行遗传操作以及演化的方法。
