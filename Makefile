.PHONY: start
start: list_test.cpp  hash_table.h list_lib.h
	clang++ -g -c list_test.cpp && clang++ list_test.o -g -o list_test && ./list_test

