#pragma once
#include "list_func.h"
#include <cstddef>

const int hash_table_size = 2003;

struct hash_table {
    size_t (*hash_table_func)(char * word, int len_of_word);
    doubly_linked_list ** data;
    size_t quantity_of_lists;
    size_t sum_of_words;
};


size_t always_zero_func(char * word, int len_of_word);
size_t first_letter_func(char * word, int len_of_word);
size_t word_len_func(char * word, int len_of_word);
size_t ascii_sum_func(char * word, int len_of_word);
size_t ascii_sum_div_len_func(char * word, int len_of_word);
size_t ror_hash_func(char * word, int len_of_word);
size_t rol_hash_func(char * word, int len_of_word);
size_t CRC32(char * word, int len_of_word);

size_t CRC32_modified(char * word, int len_of_word);


int asm_strcmp(const char * word1, const char * word2);
