# Copyright (C) 2012 Gustav Munkby, Hamburg University of Technology (TUHH)
tests="dup tri trump cflow timer"
crafts="hand auto"
olevels="O3 O2 Os O0"
plevels="nrp xrp"

for p in $plevels; do
  for t in $tests; do
    for c in $crafts; do
      for o in $olevels; do
        echo build/basic-$p-$o.o
        echo build/$t-$c-$p-$o.o
      done
      echo build/$t-$c-$p.log
    done
  done | sort -u | xargs redo-ifchange
done
for p in $plevels; do
  for o in $olevels; do
    echo "extern int basic_${p}_${o}(int);"
  done

  for t in $tests; do
    for c in $crafts; do
      for o in $olevels; do
        echo "extern int ${t}_${c}_${p}_${o}(int);"
      done
    done
  done
done

echo
echo "void all_tests() {"
for p in $plevels; do
  for o in $olevels; do
    echo "  run_test(start, basic_${p}_${o}, \"${o}\");"
  done
  echo "  summarize(\"basic:${p}\", 1, iterations, true, false);"

  for t in $tests; do
    for o in $olevels; do
      for c in $crafts; do
        echo "  run_test(start, ${t}_${c}_${p}_${o}, \"${c} ${o}\");"
      done
    done
    echo "  summarize(\"${t}:${p}\", 1, iterations, true, false);" 
    echo
  done
done
echo "}"

