source ../../does.sh
test -d build || mkdir build 
c++ -c ${1#build/}.cc -o $3 -O3 -fno-align-labels -fno-align-functions
