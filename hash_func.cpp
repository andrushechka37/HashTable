const int hash_table_size = 2777;

int always_zero_func(char * word, int len_of_word) {
    return 1;
}

int first_letter_func(char * word, int len_of_word) {
    return word[0];
}

int word_len_func(char * word, int len_of_word) {
    return len_of_word;
}

int ascii_sum_func(char * word, int len_of_word) {
    int sum = 0;
    for (int i = 0; i < len_of_word; i++) {
        sum += word[i];
    }
    return sum;
}

int ascii_sum_div_len_func(char * word, int len_of_word) {
    int sum = 0;
    for (int i = 0; i < len_of_word; i++) {
        sum += word[i];
    }
    return sum / len_of_word;
}




int my_rol(int number) {
    return (number >> 1) | (number << 31);
}

int my_ror(int number) {
    return (number << 1) | (number >> 31);
}

int ror_hash_func(char * word, int len_of_word) {
    int hash = 0;
    for (int i = 0; i < len_of_word; i++) {
        hash = my_ror(hash) ^ word[i];
    }
    return hash % hash_table_size;
}

int rol_hash_func(char * word, int len_of_word) {
    int hash = 0;
    for (int i = 0; i < len_of_word; i++) {
        hash = my_rol(hash) ^ word[i];
    }
    return hash;
}



