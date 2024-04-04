#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "list_lib.h"
#include "hash_table.h"

// sscanf and fread to читалка
int table_number_global = 1;


int hash_table_func(char * word, int len_of_word) {

    return 1;
}

void hash_table_ctor(hash_table * table) {

    (table->data) = (doubly_linked_list **) calloc(hash_table_size, sizeof(doubly_linked_list *));
    return;
}


#define CUR_LIST_PTR (table->data)[place_for_number]

int get_position_in_list(doubly_linked_list * list, char * word) {

    for (int i = 1; i <= list->list_size; i++) {
        if (strcmp(word, list->data[i].value) == 0) {
            return -1;
        }
    }

    return list->list_size;
}

void hash_table_insert(hash_table * table, char * word, int len_of_word) {

    int place_for_number = hash_table_func(word,  len_of_word); // dont like name

    if (CUR_LIST_PTR == NULL) {
        CUR_LIST_PTR = list_ctor();
    }

    int position = get_position_in_list(CUR_LIST_PTR, word);
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

    while (fscanf(data, "%d %s",&len, word) == 2) {
        hash_table_insert(table, word, len);
    }
}

int  hash_table_search(int hash, char * word, hash_table * table) {
    doubly_linked_list * list = table->data[hash];

    for (int i = 1; i <= list->list_size; i++) {
        if (strcmp(word, list->data[i].value) == 0) {
            return i;
        }
    }

    return 0;
}

void hash_table_dtor(hash_table * table) {
    for (int i = 0; i < hash_table_size; i++) {
        if (table->data[i] != NULL) {
            list_dtor(table->data[i]);
        }
    }

    free(table->data);
    table->data = NULL;
}


void make_csv_table(hash_table * table) {

    char table_name[20] = "";
    snprintf(table_name, 3,  "%d", table_number_global);
    table_number_global++;

    strcat(table_name, "table.csv");

    FILE * csv = fopen(table_name, "w");
    for (int i = 1; i < hash_table_size; i++) {
        if (table->data[i] == NULL) {
            fprintf(csv, "%d;0\n", i);
            continue;
        }
        fprintf(csv, "%d;%d\n", i, table->data[i]->list_size);
    }
    fclose(csv);
}

int main(void) {
    hash_table table = {};
    hash_table_ctor(&table);

    FILE * data = fopen("data_storage.txt", "r");
    read_file_to_table(data, &table);
    fclose(data);


    hash_table_dump_txt(&table);

    make_csv_table(&table);
    hash_table_dtor(&table);
}
