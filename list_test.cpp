#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "list_lib.h"
#include "hash_table.h"

// sscanf and fread to читалка
// аллокация 



int hash_table_func(char * word) {
    return word[0] % 10;
}

void hash_table_ctor(hash_table * table) {
    (table->data) = (doubly_linked_list **) calloc(sizeof(doubly_linked_list *), hash_table_size);
    return;
}

#define CUR_LIST_PTR (table->data)[place_for_number]


int get_position(doubly_linked_list * list, char * word) {

    for (int i = 1; i <= list->len_of_list; i++) {
        if (strcmp(word, list->data[i].value) == 0) {
            return -1;
        }
    }

    return list->len_of_list;
}

void hash_table_insert(hash_table * table, char * word, int len_of_word) {

    int place_for_number = hash_table_func(word);

    if (CUR_LIST_PTR == NULL) {
        CUR_LIST_PTR = list_ctor();
    }

    int position = get_position(CUR_LIST_PTR, word);
    if (position != -1) {
        list_insert_after(CUR_LIST_PTR, position, word, len_of_word);
    }

    return;
}

#undef CUR_LIST_PTR

#define CUR_LIST_PTR (table->data)[i]

void hash_table_dump_txt(hash_table * table) {
    FILE * log = fopen("log_file.txt", "w");
    for (int i = 0; i < hash_table_size; i++) {
        if (CUR_LIST_PTR == NULL) continue;

        fprintf(log, "\n\n     ========================= %d_hash_array ======================\n\n", i);

        dump_list_txt(CUR_LIST_PTR, log);
    }
    return;
}


void read_file_to_table(FILE * data, hash_table * table) {

    char word[max_len_of_word] = "";
    int len = 0;
    //int i = fscanf(data, "%d %s",&len, word);
    while (fscanf(data, "%d %s",&len, word) == 2) {
        hash_table_insert(table, word, len);
    }
}

int  hash_table_search(int hash, char * word, hash_table * table) {
    doubly_linked_list * list = table->data[hash];

    for (int i = 1; i <= list->len_of_list; i++) {
        if (strcmp(word, list->data[i].value) == 0) {
            return i;
        }
    }

    return 0;
}


int main(void) {

    int x = 6;
    hash_table table = {};
    hash_table_ctor(&table);

    // hash_table_insert(&table, "dkhkj", strlen("dkhkj"));
    // hash_table_insert(&table, "ejfvj", strlen("ejfvj"));
    // hash_table_insert(&table, "fuck", strlen("fuck"));
    // hash_table_insert(&table, "dkhj", strlen("dkhj"));
    // hash_table_insert(&table, "d", strlen("d"));
    // hash_table_insert(&table, "da", strlen("da"));

    FILE * data = fopen("data_storage.txt", "r");

    read_file_to_table(data, &table);


    hash_table_dump_txt(&table);

}
