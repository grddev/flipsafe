# Copyright (C) 2012 Gustav Munkby
source ../cmds.sh
redo-ifchange basic/all
c++ -Iadobe benchmark.cc ../src/handler.cc ../src/cflow.cc basic/build/*.o -O3 -o $3
