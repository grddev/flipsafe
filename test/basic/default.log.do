# Copyright (C) 2012 Gustav Munkby
redo-ifchange $1-O3.o
objdump -d $1-O3.o | grep '^ ' > $3
