DEPS=(test/testsihft.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
export CXXFLAGS="-Os -g3"
redo-ifchange $DEPS
g++ $CXXFLAGS -lboost_unit_test_framework -o $3 $DEPS
