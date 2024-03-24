echo "delete Model"
rm -rf ./model
echo “build model”
make > /dev/null 2>&1 buildDebug
rm -rf ./build_debug
echo "run model"
./model -c ./config/2IssueWidth.yaml -e ./thirdParty/benchmark/rv64i_dhrystone 