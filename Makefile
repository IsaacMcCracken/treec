debug:
	clang-cl /Zi src/main.c -o treec.exe
	./treec test/notc.tc 

debug_build:
	clang-cl /Zi src/main.c -o treec.exe


