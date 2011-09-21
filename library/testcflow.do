DEPS=(test/testcflow.cc src/cflow.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
export CXXFLAGS="-std=c++0x -O0 -g3"
redo-ifchange $DEPS
g++ $CXXFLAGS -lboost_unit_test_framework -o $3 $DEPS

