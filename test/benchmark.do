source ../does.sh
c++ -Iadobe benchmark.cc ../src/handler.cc ../src/cflow.cc -O3 -o $3
