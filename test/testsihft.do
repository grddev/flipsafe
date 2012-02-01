# Copyright (C) 2012 Gustav Munkby
source ../cmds.sh
c++ -Os -g3 -lboost_unit_test_framework -o $3 testsihft.cc ../src/handler.cc

