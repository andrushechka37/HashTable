#include <cstddef>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include <immintrin.h>
#include <nmmintrin.h>


extern "C" int asm_strlen(const char * str);
// read about asm insert
// inline strang things 

void hash_table_ctor(hash_table * table,  size_t (*hash_table_func)(char * word, int len_of_word)) {

    (table->data) = (doubly_linked_list **) calloc(hash_table_size, sizeof(doubly_linked_list *));
    table->hash_table_func = hash_table_func;
    return;
}

const int vector_capacity = 16;

int fast_strcmp (const char * first, int len1, const char * second, int len2) {

    if (len1 != len2) {
        return 1;
    }

    __m128i first_word =  _mm_loadu_si128((const __m128i_u *)first);
    __m128i second_word = _mm_loadu_si128((const __m128i_u *)second);

    if (len1 > vector_capacity) {
        return strcmp(first, second);
    }

    return _mm_cmpestri(first_word, len1, second_word, len2, _SIDD_UBYTE_OPS);

}

int get_free_cell_in_list(doubly_linked_list * list, char * word, int len_of_word) {

    for (int i = 1; i <= list->list_size; i++) {
        if (strcmp(word, list->data[i].value) == 0) {
            return -1;
        }
    }

    return list->list_size;
}


#define CUR_LIST_PTR (table->data)[place_for_number]

void hash_table_insert(hash_table * table, char * word, int len_of_word) {

    int place_for_number = table->hash_table_func(word, len_of_word);

    if (CUR_LIST_PTR == NULL) {
        CUR_LIST_PTR = list_ctor();
        table->quantity_of_lists++;
    }

    int position = get_free_cell_in_list(CUR_LIST_PTR, word, len_of_word);
    if (position != -1) {
        list_insert_after(CUR_LIST_PTR, position, word, len_of_word);
        table->sum_of_words++;
    }

    return;
}

#undef CUR_LIST_PTR

#define CUR_LIST_PTR (table->data)[i]

void hash_table_dump_txt(hash_table * table) {

    FILE * log = fopen("log_file.txt", "w");  // hardcode is ploho

    for (int i = 65; i < hash_table_size; i++) {
        if (CUR_LIST_PTR == NULL) continue;

        fprintf(log, "\n\n     ========================= %d_hash_array ======================\n\n", i);
        printf("%d\n", i);

        dump_list_txt(CUR_LIST_PTR, log);

        printf("%d\n", i);
    }
    
    fclose(log);
    return;
}


void read_file_to_table(hash_table * table) {
    FILE * data = fopen("data_storage.txt", "r"); // hard code of file name is very very ploho
    if (data == NULL) {
        printf("netu faila s dannumi\n");
        return;
    }

    char word[max_len_of_word] = "";
    int len = 0;

    while (fscanf(data, "%d %s",&len, word) == 2) {
        hash_table_insert(table, word, len);
    }
    fclose(data);
}

int  hash_table_search(char * word, hash_table * table) {

    int len = strlen(word);

    size_t hash = table->hash_table_func(word, len);

    doubly_linked_list * list = table->data[hash];

    for (int i = 1; i <= list->list_size; i++) {
        if (strcmp(word, list->data[i].value) == 0) {
            return i;
        }

        // if (asm_strcmp(word, list->data[i].value) == 0) {
        //     return i;
        // }
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

int TABLE_NUMBER = 1;
void make_csv_table(hash_table * table) {

    char table_name[20] = "";
    snprintf(table_name, 9,  "%d", TABLE_NUMBER);
    TABLE_NUMBER++;
    strcat(table_name, "table.csv");
    char folder[30] = "tables/";
    strcat(folder, table_name);

    FILE * csv = fopen(folder, "w");

    if (csv == NULL) {
        printf("NULL no tables folder\n");
        return;
    }

    for (int i = 1; i < hash_table_size; i++) {
        if (table->data[i] == NULL) {
            fprintf(csv, "%d;0\n", i);
            continue;
        }
        fprintf(csv, "%d;%d\n", i, table->data[i]->list_size);
    }
    fclose(csv);
}

const int quantity_of_func = 7;

int main(void) {

    size_t (*hash_table_func_array[quantity_of_func])(char * word, int len_of_word) = {
    always_zero_func,
    first_letter_func,
    word_len_func,
    ascii_sum_func,
    ascii_sum_div_len_func,
    rol_hash_func,
    CRC32_modified
    };


    // for (int i = 6; i < quantity_of_func; i++) {

    //     hash_table table = {};
    //     hash_table_ctor(&table, hash_table_func_array[6]);

    //     read_file_to_table(&table);

    //     unsigned long long res = 0;
    //     int max_number = 100000;


    //     for (int n = 0; n < max_number; n++) {

    //         unsigned long long start = __rdtsc();        

    //         for (int i = 0; i < hash_table_size; i++) {

    //             if (table.data[i] == NULL) continue;
    //             for (int j = 1; j < table.data[i]->list_size; j++) {
    //                 hash_table_search(table.data[i]->data[j].value, &table);
    //             }
    //         }

    //         unsigned long long end = __rdtsc();

    //         res += (end-start);
    //     }

    //     make_csv_table(&table);



    //     hash_table_dtor(&table);
    // // }


    // printf("\n\n%llu\n\n",res/max_number);

    const char word1[] = "g";
    printf("\nlen: %d\n", asm_strlen(word1));
    return 0;
    //asm_strcmp(word1, word2);
}











        // printf("\nload factor of %d table is: %lf\n", i + 1, (float)table.sum_of_words/table.quantity_of_lists);

        // double disp = 0;
        // double average = (double)table.sum_of_words/hash_table_size;

        // for (int i = 0; i < hash_table_size; i++) {
        //     if (table.data[i] == NULL) {
        //         disp += average * average;
        //     } else {
        //         disp += (table.data[i]->list_size - average) * (table.data[i]->list_size - average);
        //     }
        // }
        // printf("\ndisp %d table is: %lf\n", i + 1, disp/table.quantity_of_lists);