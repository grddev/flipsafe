source ../cmds.sh
c++ -std=c++0x -g3 -Os -lboost_unit_test_framework -o $3 inject.cc ../src/handler.cc

