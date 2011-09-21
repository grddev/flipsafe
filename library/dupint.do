DEPS=(examples/dupint/main.cc src/handler.cc)
# put ofiles in build directory
DEPS=${${DEPS[@]/#/build/}/%.cc/.o}
export CXXFLAGS="-O0 -g3 -Iexamples/dupint"
redo-ifchange $DEPS
g++ $CXXFLAGS -o $3 $DEPS

