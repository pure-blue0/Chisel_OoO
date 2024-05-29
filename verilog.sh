#!/bin/bash
files=$(ls *.scala 2>/dev/null)
# 检查是否存在.scala文件并且是否只有一个
num_files=$(echo "$files" | wc -l)
if [ -z "$files" ] || [ $num_files -ne 1 ]; then
  if [ -z "$files" ]; then
    echo "当前目录下不存在.scala文件"
  else
    echo "当前目录下有多个.scala文件，请确保只有一个"
  fi
  exit 1
fi
#1.修改并移动文件到chisel project项目中
#若只有一个.v文件，则进行后续操作
file_name=$(basename "${files%.scala}") #提取文件名
rm -f ../chisel_test/src/main/scala/*.scala # 删除目标目录下的所有 .scala 文件
read -p "请输入文件名称（或直接回车使用原始文件名）：" input_name
if [ -z "$input_name" ]; then
  input_name=$file_name
fi
cp "$file_name.scala" "../chisel_test/src/main/scala/${input_name}.scala" # 移动并重命名文件
mv "$file_name.scala" "${input_name}.scala" #重命名
rm -rf $file_name.v
echo "文件已移动并重命名为 $input_name.scala文件"
#2.运行chisel生成verilog并将文件复制回model里
cd ../chisel_test
sbt run
read -p "按回车键继续..." continue #如果正确生成则按回车
cd ../modelAchieve
cp "../chisel_test/${input_name}.v" ./
echo "已根据scala文件生成对应的v文件，并复制到当前目录中"

#3.运行verilator生成C++文件，移动C++文件到目标目录中
verilator --cc $input_name.v 
rm -rf ./src/Processor/Pipeline/obj_dir
#mv ./obj_dir ./src/Processor/Component #改成需要交叉测试的模块所在的目录
mv ./obj_dir ./src/Processor/Pipeline #改成需要交叉测试的模块所在的目录
echo "已生成的C++文件，并复制到了目标路径"
echo "请在需要交叉验证的模块的文件前添加#include \"./obj_dir/V${input_name}.h\""