for c in auto man; do
  for m in dup tri trump; do
    echo $m-$c.log
  done
done | xargs redo-ifchange
