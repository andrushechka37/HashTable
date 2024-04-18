#pragma once
#include "list_func.h"
#include <cstddef>

const int hash_table_size = 2003; // NOTE: 2003 was a good year (choose numbers more carefully)

struct hash_table {
    size_t (*hash_table_func)(char * word); // TODO: rename func?
    doubly_linked_list ** data; // TODO: a pointer?
    size_t quantity_of_lists;
    size_t sum_of_words; // TODO: what is this
};


// TODO: Why is this in this header (make a separate one)
size_t always_zero_func(char * word, int len_of_word);
size_t first_letter_func(char * word, int len_of_word);
size_t word_len_func(char * word, int len_of_word);
size_t ascii_sum_func(char * word, int len_of_word);
size_t ascii_sum_div_len_func(char * word, int len_of_word);
size_t ror_hash_func(char * word, int len_of_word);
size_t rol_hash_func(char * word, int len_of_word);
size_t CRC32(char * word, int len_of_word); // TODO: naming?

size_t CRC32_modified(char * word);


int asm_strcmp(const char * word1, const char * word2);
