redo-ifchange ../does.sh

function c++() {
  depfile=/tmp/does.dep.$$
  g++ -I../include -I. -Wall -Werror -Wextra -MT x -MD -MF $depfile "$@"
  result=$?
  sed '1s,.*: *,,;s,\\$,,' < $depfile | xargs redo-ifchange
  rm -- $depfile 2> /dev/null
  return $result
}

