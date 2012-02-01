redo-ifchange all
[ "x$(readlink flipsafe)" = x. ] || ln -s . flipsafe
tar zcf flipsafe.tar.gz $(git ls-tree HEAD --name-only -r include  test README.md cmds.sh | grep -v .gitignore | sed 's,^,flipsafe/,') 
rm flipsafe >/dev/null
