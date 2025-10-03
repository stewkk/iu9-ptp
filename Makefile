test: build
	ctest --test-dir build/ -j 4 --output-on-failure

gdb: build
	gdb --args ./build/bin/unittests --gtest_break_on_failure

build:
	cmake --build build -- -j 6

dot:
	dot -T png -O /tmp/out4.dot
	loupe /tmp/out4.dot.png

.PHONY: gdb test build dot
