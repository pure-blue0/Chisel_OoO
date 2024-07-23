# Chisel_OoO
an out of order RISC-V processor in Chisel

update architecture on `reference_model` branch
<!-- # pineline
![](./docs/image/pipeline.drawio.png)
## Fetch
- 更新PC,优先级为 backend redirect > predicted PC > PC + 4
- 从`I$`中取fetch package, 存入fetch buffer

## Decode
- 从Fetch buffer 取指令，解码并生成micro-op，每周期最多取两条指令
- 如果指令为跳转指令，使用BTB预测是否跳转和跳转地址
- 使用Fetch Target Queue(FTQ)维护所以inflight branch prediction 和 PC 信息的记录，当预测错误时，会根据FTQ中的记录恢复处理器状态。当指令提交时，会把FTQ中相应的条目删除。

## Rename
- 使用ROB实现寄存器重命名，将指令中的逻辑地址转化为物理地址(即在ROB中的编号)。消除WAW和WAR相关
- register renaming table记录AR(Architecture Register)和PR(Physical Register)的对应关系
- 此时ROB充当物理寄存器，暂存执行完毕但不能提交的指令数据，供后续指令读取
- 同时ROB充当dispatch buffer，暂存未分配的指令

## Dispatch
- 按指令类型将dispatch buffer中的指令发送到reservation station

## Execute
- 发射到reservation station等待其源操作数都准备好后开始执行。
- 对与LSU,首先计算指令地址，然后指令被送入load/store buffer当`D$`空闲时，执行buffer中最早的指令
- 对应BPU,分支指令会判断指令是否跳转并向fetch阶段发送指令重定向信息


## Writeback
- 指令执行结果被写回ROB中的目标寄存器
- 同时结果还可能前递到reservation station中

## Commit
- 当ROB中最早的指令执行完毕后，更新AR和register renaming table，删除该条目 -->
