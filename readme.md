>  计75  李阳崑  2017011235  li-yk17@mails.tsinghua.edu.cn



### 实验环境

gcc version 9.3.0 (Arch Linux 9.3.0-1)



### 编译

在 `tomasulo/` 文件夹下执行

```shell
g++ main.cpp RegStat.cpp Tomasulo.cpp util.cpp -o tomasulo
```

编译得到可执行文件 `tomasulo` （提交的内容中已包含可执行文件 `tomasulo`）

执行

```shell
g++ query.cpp -o query
```

编译得到可执行文件 `query` （提交的内容中已包含可执行文件 `query`）



### 运行

可执行文件 `tomasulo` 就是Tomasulo算法模拟器，输入以下命令运行：

```shell
./tomasulo [测例名]
```

其中测例名可以取以下12个字符串：

`0.basic`, `1.basic`, `2.basic`, `3.basic`, `4.basic`, `Example`, `Fabo`, `Fact`, `Gcd`, `Mul`, `Big_test`, `my`

代表 `Testcase/` 文件夹下的12个测例文件。

运行模拟器会在命令行输出该测例的算法执行时间、所花cycle数和运行总时间，并且会将运行的log输出在 `Log/` 文件夹下。



可执行文件 `query` 是用来查询模拟器执行过程中任意一个 Cycle 的瞬时状态的，输入以下命令运行：

```shell
./query [测例名] [cycle]
```

测例名不支持`Gcd`, `Mul`, `Big_test`这3个，其他9个都支持。

cycle 表示查询第几个cycle结束时的状态。



### 关于states

每个测例每个cycle的瞬时状态我用文本文件存在 `states/` 文件夹下，文件名是`测例名_cycle` ，可以直接点开查看。

因为命令行中的制表符`\t`和记事本中的制表符宽度可能不一样，因此有可能出现 `query` 的结果对齐不太好，这个时候可以直接点开`states/`下的文件查看。


