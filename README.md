## 运行方式：
进入文件夹，在终端中输入 如下命令运行
```
./model -c ./config/2IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone 

```

## 交叉验证：
1.修改./CMakeLists.txt内的verilator/include路径，改成自己的文件verilator/include安装路径（只需第一次运行时修改成自己的即可）
2.将chisel代码转成的verilog代码复制到./*.v文件里（不需要管文件名，这个.v文件在根目录下，只需要把代码复制进去，后面运行./convert.sh时会自动根据你的输入来修改v文件的文件名，减少手动改名的次数）
3.然后修改./convert.sh文件里的第21行代码，改成需要交叉测试的模块所在的同级目录
4.在终端中输入./convert.sh 运行，并输入当前模块的名称，此时程序会将verilog代码转成C++，生成的文件会被自动复制到对应的模块所在的目录
5.在需要交叉验证的模块所在的文件中添加#include "./obj_dir/V模块名.h"
6.将原来交叉验证的模块的代码删除/注释，并连接输入输出信号，运行eval函数。
7.修改obj_dir/V模块名.h文件中的eval函数，使其能够输出多个变量，具体操作见示例。
8.在终端中输入./run.sh 运行重新编译，若能成功输出dhrystone的 结果，则表示模块没有问题。


### eg alu 模块
1.将alu的chisel代码复制到当前根目录的.v文件里。
2.修改./convert.sh文件里的第21行代码，改成./src/Processor/Component/FuncUnit（即ALU模块所在的目录）
3.在终端中输入./convert.sh，然后再输入文件名alu，就会自动将生成的C++文件放在ALU所在的目录里
4.在Func_ALU.hh中添加头文件#include "./obj_dir/Valu.h"
5.将ALU的原来的代码void Compute(InsnPtr_t& insn)函数内的东西注释，添加如下代码,其中输入信号的连接与输出信号有略微差异，输入信号直接连接，而输出信号需要传递**指针**到eval函数中，然后再返回。
````
        Valu *alu;
        alu=new Valu;//创建对象
        //连接输入
        alu->io_Insn_operand1=insn->Operand1;
        alu->io_Insn_operand2=insn->Operand2;
        alu->io_Sub_OP_out=insn->SubOp;
        //连接输出
        alu->eval(insn->RdResult);

        delete alu;//删除创建的对象
````
6.修改obj_dir/Valu.h文件中的eval函数，在参数里添加需要输出的变量，类型与model的原来的类型保持一致。
然后在eval函数里将输出的信号连接到对应的输入参数上，即可实现数据的传回。（多个数据也是一样的，多加几个输入参数，然后对其进行赋值）
**特别注意：不要忘记参数里的 “&” ！！！！，否则计算的结果无法传回给model**
````
    void eval(uint64_t& Rdresult) { 
      eval_step();
      Rdresult=io_Insn_rdresult; 
    }
````
7.在终端中输入./run.sh 运行重新编译，若能成功输出dhrystone的 结果，则表示模块没有问题。

### 调试方式
1.若出现运行失败，可以不删除需要原始代码，然后把需要交叉验证的代码加载原始代码的前面，再加上DPRINTF，然后对比两者的输入输出，就可以知道是哪里出现了问题。具体例子见Func_ALU.hh中被注释掉的compute函数。
通过如下方式将输入输出打印
    ````
        //交叉验证的模块的代码
        DPRINTF(temptest,"Verilog data1 {:#x}，data2 {:#x} data3 {:#x}",data1,data2,data3);//输出交叉验证得到的数据
        //原始模块的代码
        DPRINTF(temptest,"Origin data1 {:#x}，data2 {:#x} data3 {:#x}",data11,data22,data33);//输出原始模块的数据
        //对比两者的输出数据，如果出现数据不相等，则退出程序
        if(xxx)exit(1);
    ````
2.在做交叉验证时，请注意针对两者差异做出的兼容性修改

## 架构的配置文件都在config里，我们用的是2issuewidth.yaml

## 各个模块的源码在processor/pipeline里，观看顺序fetch1->decode->dispatch->IEW->commit
## 需要实现的模块（有的还有二级模块还有三级模块，待添加）
1.fetch1包含的子模块：
--GenNextFetchAddress
--SendFetchReq
--SendReq
--ReceiveReq
--memory（在sendfetchreq里面）
2.decode包含的子模块：
--SendReq
--DecodeInsn
--Predecode
--BranchRedirect
3.dispatch包含的子模块：
--rcu->TryAllocate
--rcu->Allocate
--lsq->TryAllocate
--lsq->Allocate
--TryDispatch
--DispatchInsn

4.IEW模块包含的子模块
--ReadOperand();
--Issue();
--Execute();
--Forwarding();
--WriteBack();

5.commit里包含的子模块
--SendCommitReq
--CommitInsnPkg

## 注意：
1.代码完全看懂比较困难，但是每个模块只需要关注其实现的功能以及输入输出信号即可，有的可能是以结构体的形式输入的，可以在vscode中通过选定变量，然后右键转到定义就可以看到具体是哪些信号
2.模块内部实现的都是具体的功能，并不包含时序

