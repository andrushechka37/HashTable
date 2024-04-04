

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