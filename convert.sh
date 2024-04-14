#!/bin/bash
files=$(ls *.v 2>/dev/null)
# 检查是否存在.v文件并且是否只有一个
num_files=$(echo "$files" | wc -l)
if [ -z "$files" ] || [ $num_files -ne 1 ]; then
  if [ -z "$files" ]; then
    echo "当前目录下不存在.v文件"
  else
    echo "当前目录下有多个.v文件，请确保只有一个"
  fi
  exit 1
fi
#若只有一个.v文件，则进行后续操作
file_name=$(basename "${files%.v}") #提取文件名
read -p "请输入数据名称： " input_name
mv "$file_name.v" "${input_name}.v" #重命名
echo "文件已重命名为 $input_name.v"
verilator --cc $input_name.v 
rm -rf ./src/Processor/Component/FuncUnit/obj_dir

mv ./obj_dir ./src/Processor/Component/FuncUnit #改成需要交叉测试的模块所在的目录
echo "已生成的C++文件，并复制到了目标路径"
