#include <cctype>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

const int max_len_of_word = 20;

void removePunctuation(char *word) {
    char * old_version = word;
    char * new_verion = word;
    
    while(*old_version) {
        if (ispunct((unsigned char)*old_version)) {
            old_version++;
        } else if (old_version == new_verion) {
            old_version++;
            new_verion++;
        } else {
            *new_verion++ = *old_version++;
        }
    }
    *new_verion = '\0';
}


void parse_text(FILE * original, FILE * parsed_text) {

    char word[max_len_of_word] = "";
    int len = 0;

    while (fscanf(original, "%s %n", word, &len) == 1) {
        removePunctuation(word);
        if (strlen(word) == 0) continue;
        fprintf(parsed_text, "%ld %s\n", strlen(word), word);
    }


}



int main() {
    FILE * original = fopen("text.txt", "r");
    FILE * parsed_text = fopen("data_storage.txt", "w");
    parse_text(original, parsed_text);
    int i = 5;
}