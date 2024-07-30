#!/bin/bash

compile_model=true  
debug_message=false  
m_option="" 

while getopts ":ndm:" opt; do
  case ${opt} in
    n) compile_model=false ;;
    d) debug_message=true  ;;
    m) m_option="-m ${OPTARG}"  ;;
    \?) echo "Invalid option: -${OPTARG}" ;;
    :) echo "Option -${OPTARG} requires an argument." ;;
  esac
done

if $compile_model ; then
  echo "delete Model"
  rm -rf ./model
  echo "build model"
  make buildDebug
fi

echo "run model"
if $debug_message ; then
  ./model -c ./config/4IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone ${m_option} -d temptest
else
  ./model -c ./config/4IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone ${m_option}
fi


# echo "delete Model"
# rm -rf ./model
# echo “build model”
# make buildDebug
# echo "run model"
# ./model -c ./config/4IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone -d temptest
