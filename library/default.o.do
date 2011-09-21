src=${1#build/}.cc
test -d ${1%/*} || mkdir -p ${1%/*}
g++ $CXXFLAGS -g2 -Os -Iinclude -Wall -Werror -Wextra -MT x -MD -MF $1.d -c -o $3 $src
sed '1s,.*: *,,;s,\\$,,' < $1.d | xargs redo-ifchange $src

