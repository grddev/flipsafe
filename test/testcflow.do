source ../does.sh
c++ -std=c++0x -O0 -g3 -lboost_unit_test_framework -o $3 testcflow.cc ../src/cflow.cc ../src/handler.cc

