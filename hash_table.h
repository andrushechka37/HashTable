#pragma once
#include "list_lib.h"

const int hash_table_size = 2777;

struct hash_table {
    //pointer to hash func
    doubly_linked_list ** data;
};

int always_zero_func(char * word, int len_of_word);
int first_letter_func(char * word, int len_of_word);
int word_len_func(char * word, int len_of_word);
int ascii_sum_func(char * word, int len_of_word);
int ascii_sum_div_len_func(char * word, int len_of_word);