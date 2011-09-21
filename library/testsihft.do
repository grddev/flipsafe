DEPS=(test/testsihft.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
redo-ifchange $DEPS
g++ -g2 -Os -lboost_unit_test_framework -o $3 $DEPS

