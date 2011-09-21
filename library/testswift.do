DEPS=(test/testswift.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
redo-ifchange $DEPS
g++ -g2 -Os -lboost_test_exec_monitor -o $3 $DEPS

