# SanSi

> 三思而后行。

## 简介

三思（SanSi）是一个新型迭代哈希密码。

不同于传统顺序处理的 sponge 结构，三思在其基础上引入了更多的复杂性：将顺次线性处理分块的结构改变为树形结构，通过在处理顺序的维度引入更多的复杂性加强了密码，使得长度扩展攻击等攻击方法更加困难。同时，三思还在 `f` 函数处引入了不同的方法。

本算法由刘方舟、刘子君、刘泽禹、庞元喆、徐明启设计、实现并完成评估。

## 设计理念

设计的 idea 是将处理流程中加入树状的处理逻辑。每个分块对应一个启发值（位置和每个分量异或和）。

首先计算出前两个分块的 $5 \times 5 \times w$ 张量，此后每处理一个块都首先将其嵌入一个全零或全一的张量（取决于位置的奇偶性）。此时我们有三个张量，贪心地将两个启发值最大的张量进行合并。

## 算法组成

## 算法实现效率

## 评估结果

### 随机性检测

使用[**NIST**统计学工具包](https://csrc.nist.gov/Projects/Random-Bit-Generation/Documentation-and-Software)完成随机性检测。  
通过随机改动字节串中随机选取的部分比特，生成相近的多组明文，经过哈希后，获得待检测文件；之后使用**NIST**工具包对文件进行随机性分析。

上述流程可通过命令：

```bash
bash test.sh
```

完成。共生成10240组，长度为256个字符的随机相近明文，经过Sansi算法哈希并转换为ASCII编码的01串。  
将待检测文件分为8个比特流，对其进行**NIST**工具包的所有随机性检测项目，结果显示，哈希结果通过了所有的随机性检测项目，如下表所示（具体检测结果文件详见`finalAnalysisReport_test1.txt`）：

|项目|P Value| 通过率 |
|:--:|:--:|:--:|
|Frequency| 0.118170 | 1/1 |
| BlockFrequency | 0.685931 | 1/1 |
|CumulativeSums| 0.236335/0.226213 |  2/2 |
| Rank | 0.346732 | 1/1|
| FFT | 0.977122 | 1/1 |
| Runs | 0.536981| 1/1 |
| LongestRun |0.518900 | 1/1|
|NonOverlappingTemplate| - | 148/148 |
| OverlappingTemplate | 0.673786 | 1/1 |
| ApproximateEntropy | 0.439917 | 1/1 |
| RandomExcursions | - | 8/8 |
| RandomExcursionsVariant | - | 18/18 |
| Serial | 0.955901/0.971477 | 2/2 |
| LinearComplexity | 0.300616 | 1/1 |

### 碰撞攻击检测

A novel cryptographic hash function, project for Modern Cryptography, 2022 Spring THUCST.

SanSi（三思）是一个基于类 sponge 结构的哈希密码。三可以意味着每一轮都有三个张量进行决策，也可以意味着改编自 SHA3（大雾）。

目前的 demo 版非常粗糙，F 函数直接 copy 自 SHA3 的处理流程，后期如果觉得可以用的话需要大概。
