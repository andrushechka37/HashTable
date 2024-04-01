#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "list_lib.h"
#include "hash_table.h"

// обработка коллиции, сам поиск
// добавить слова
// парсем
// читалка
// поиск по ключу



int hash_table_func(int number) {
    return number % 10;
}

void hash_table_ctor(hash_table * table) {
    (table->data) = (doubly_linked_list **) calloc(sizeof(doubly_linked_list *), hash_table_size);
    return;
}

#define CUR_LIST_PTR (table->data)[place_for_number]
#define TALE_IP CUR_LIST_PTR->data->prev

void hash_table_insert(hash_table * table, int number) {

    int place_for_number = hash_table_func(number);

    if (CUR_LIST_PTR == NULL) {
        CUR_LIST_PTR = list_ctor();
    }

    list_insert_after(CUR_LIST_PTR, TALE_IP, number);
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


int main(void) {

    int x = 6;
    hash_table table = {};
    hash_table_ctor(&table);

    hash_table_insert(&table, 10);
    hash_table_insert(&table, 11);
    hash_table_insert(&table, 12);
    hash_table_insert(&table, 13);
    hash_table_insert(&table, 20);

    // for (int i = 0; i < 3; i++) {
    //     list_visualize(table.data[i], "-");
    // }
    hash_table_dump_txt(&table);

}
