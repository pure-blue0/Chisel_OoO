## 运行方式：
进入文件夹，在终端中输入 如下命令运行
```
./model -c ./config/2IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone 

```

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

