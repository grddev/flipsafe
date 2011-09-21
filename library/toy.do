DEPS=(examples/toy/main.cc src/cflow.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
export CXXFLAGS="-std=c++0x -O0 -g3 -Iexamples/toy"
redo-ifchange $DEPS
g++ $CXXFLAGS -o $3 $DEPS

