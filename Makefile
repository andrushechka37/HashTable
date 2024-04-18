CXXFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
   -Wc++14-compat -Wmissing-declarations -Wcast-qual -Wchar-subscripts                             \
   -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal           \
   -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline                   \
   -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked                     \
   -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo           \
   -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn                         \
   -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default               \
   -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast                    \
   -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing            \
   -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation             \
   -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192                  \
   -Wstack-usage=8192 -fsanitize=address -fsanitize=undefined -fPIE -Werror=vla

ASAN = -O3 -mavx

.PHONY: start

start: hash_table.o hash_func.o list_func.o strlen.o
	clang++ hash_table.o hash_func.o list_func.o strlen.o $(ASAN) -o list_test && ./list_test

hash_table.o: hash_table.h hash_table.cpp strlen.s
	clang++  $(ASAN) -g -v -c hash_table.cpp

hash_func.o: hash_func.cpp hash_table.h
	clang++ -mcrc32 $(ASAN) -g -c hash_func.cpp

list_func.o: list_func.cpp list_func.h
	clang++ -g $(ASAN) -c list_func.cpp

strlen.o: strlen.s 
	nasm -f elf64 -g strlen.s 

clean:
	rm -rf *.o list_test get_data_storage tables/*.csv perf.data

#$(CXXFLAGS)

