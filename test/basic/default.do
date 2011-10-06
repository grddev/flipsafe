for c in auto hand; do
  for m in dup tri trump cflow; do
    echo build/$m-$c.log
  done
done | xargs redo-ifchange
