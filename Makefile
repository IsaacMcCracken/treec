debug:
	clang-cl /Zi src/main.c -o treec.exe -I src/include
	./treec test/notc.tc 

debug_build:
	clang-cl /Zi src/main.c -o treec.exe src/include


