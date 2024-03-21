echo "delete Model"
rm -rf ./model
echo “build model ”
make buildDebug
rm -rf ./build_debug
echo "run model"
./model -c ./config/8IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone 