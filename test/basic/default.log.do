redo-ifchange $1.o
objdump -d $1.o | grep '^ ' > $3
