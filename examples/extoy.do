source ../cmds.sh
c++ -std=c++0x -O0 -g3 -Itoy -o $3 toy/main.cc ../src/cflow.cc ../src/handler.cc

