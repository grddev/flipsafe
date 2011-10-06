# Get root dir... assumes that the file was sourced from a
# script inside the source tree (this file should be in root)
root=$(readlink -f ${BASH_SOURCE:-$0})
while [ \! -f "$root/does.sh" ]; do
  root=${root%/*}
done
redo-ifchange "$root/does.sh"

function c++() {
  depfile=/tmp/does.dep.$$
  g++ -I"$root/include" -I. -Wall -Werror -Wextra -MT x -MD -MF $depfile "$@"
  result=$?
  sed '1s,.*: *,,;s,\\$,,' < $depfile | xargs redo-ifchange
  rm -- $depfile 2> /dev/null
  return $result
}

