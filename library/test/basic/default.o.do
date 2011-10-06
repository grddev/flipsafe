craft=${${1#*-}%.o}
method=${1%-*}
for h in  $craft.hh $method-$craft.hh; do
  test -f $h && echo $h
done | xargs redo-ifchange
g++ -I../../include -I. -Dmethod=$method -D$craft -c main.cc -o $3 -O3 -fno-align-labels -fno-align-functions
