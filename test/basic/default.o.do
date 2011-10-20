source ../../cmds.sh
test -d build || mkdir build 
s=${1#build/}
c++ --std=c++0x -c ${s%-O?}.cc -o $3 -O${s##*-O} -Dbasic=${s//-/_}
