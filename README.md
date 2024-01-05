# ustc-adb-lab

中科大高级数据库系统实验-2023fall(金培权)

### 运行所有算法
在code文件夹下运行
``
./run.sh
``
或者
``
./run.sh <max_k of lru-k, default is 5>
``

### 测试单个算法
在code文件夹下运行
```
mkdir build 
cd build
cmake .. && make
./lab1 -r <replacement_algorithm> -k <value k of "lru-k"> -d <path of trace file>
```
可以用`./lab1 -h`查看各个参数的具体介绍