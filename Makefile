

header-test: bin/header_to_binary
	mkdir -p bin sample include
	src/nesrom.py rand -o sample/sample.bin -s 32
	src/nesrom.py header -o include/sampleHeader.h --var sample_rom sample/sample.bin
	gcc -Wall -Wextra -Iinclude/ -o bin/header_to_binary src/header_to_binary.c
	bin/header_to_binary sample/sample-remake.bin
	md5sum sample/sample.bin sample/sample-remake.bin
