source ../does.sh
redo-ifchange basic/all
c++ -Iadobe benchmark.cc ../src/handler.cc ../src/cflow.cc basic/build/*.o -O3 -o $3
