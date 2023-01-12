debug:
	clang-cl /Zi src/main.c src/frontend/*.c -o treec.exe -I src/include
	./treec test/notc.tc 

debug_build_run:
	clang-cl -g src/main.c src/frontend/*.c -o treec.exe -I src/include
	./treec test/notc.tc

