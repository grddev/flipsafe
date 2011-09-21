DEPS=(test/testtime.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
export CXXFLAGS=-std="c++0x -Os -g3"
redo-ifchange $DEPS
g++ $CXXFLAGS -g2 -Os -lboost_unit_test_framework -o $3 $DEPS

