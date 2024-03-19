echo "delete Model"
rm -rf ./model
echo “build model ”
make buildDebug
echo "run model"
./model -c ./config/8IssueWidth.yaml -e ./thirdParty/benchmark/dhrystone.riscv 