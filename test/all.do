# Copyright (C) 2012 Gustav Munkby
exec >&2
redo-ifchange testsihft testtime testcflow benchmark testbitcount testbool
./testsihft
./testtime
./testcflow
./testbitcount
./testbool
