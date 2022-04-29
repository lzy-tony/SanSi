# SanSi
A novel cryptographic hash function, project for Modern Cryptography, 2022 Sprint THUCST.

SanSi（三思）是一个基于类 sponge 结构的哈希密码。三可以意味着每一轮都有三个张量进行决策，也可以意味着改编自 SHA3（大雾）。

目前的 demo 版非常粗糙，F 函数直接 copy 自 SHA3 的处理流程，后期如果觉得可以用的话需要大概。

设计的 idea 是将处理流程中加入树状的处理逻辑。每个分块对应一个启发值（位置和每个分量异或和）。

首先计算出前两个分块的 $5 \times 5 \times w$ 张量，此后每处理一个块都首先将其嵌入一个全零或全一的张量（取决于位置的奇偶性）。此时我们有三个张量，贪心地将两个启发值最大的张量进行合并。

