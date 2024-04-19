#include <cstddef>
#include <smmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include <immintrin.h>
#include <nmmintrin.h>


extern "C" int asm_strcmp_s(const char * str1, const char * str2);

static inline int inline_asm_strcmp(const char * str1, const char * str2) {
    int res = 0;

    asm (".intel_syntax noprefix\n"
         "vmovdqu ymm1, YMMWORD PTR [rdi] \n"
         "vpcmpeqd ymm0, ymm1, YMMWORD PTR [rsi]\n"
         "vmovmskps eax, ymm0\n"
         "xor eax, 0xFF\n"
         ".att_syntax prefix\n"
         : "=r" (res) :: "ymm0", "ymm1", "cc");

    return res;
}

void hash_table_ctor(hash_table * table,  size_t (*hash_table_func)(char * word)) {

    (table->data) = (doubly_linked_list **) calloc(hash_table_size, sizeof(doubly_linked_list *));
    table->hash_table_func = hash_table_func;
    return;
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

    int place_for_number = table->hash_table_func(word);

    if (CUR_LIST_PTR == NULL) {
        CUR_LIST_PTR = list_ctor();
        table->quantity_of_lists++;
    }

    int position = get_free_cell_in_list(CUR_LIST_PTR, word, len_of_word);
    if (position != -1) {
        list_insert_after(CUR_LIST_PTR, position, word, len_of_word);
    }

    return;
}

#undef CUR_LIST_PTR

#define CUR_LIST_PTR (table->data)[i]

void hash_table_dump_txt(hash_table * table) {

    // TODO: make a logging library out of this
    FILE * log = fopen("log_file.txt", "w");  // hardcode is ploho TODO: yes (also check this)

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
    if (data == NULL) {                                           // TODO: if you say it's bad it doesn't make it better (maybe a bit)!
        printf("file not found: \"useful-information-for-debugging-this.txt\","
                       "also why did fopen fail? Look in errno! (perror)\n");
                       // place holder: maybe more informative message
        return;
    }

    char word[max_len_of_word] = "";
    int len = 0;

    while (fscanf(data, "%d %s",&len, word) == 2) {
        hash_table_insert(table, word, len);
    }
    fclose(data);
}

int hash_table_search(char * word, hash_table * table) {

    size_t hash = table->hash_table_func(word);

    doubly_linked_list * list = table->data[hash];

    for (int i = 1; i <= list->list_size; i++) {
        
        if (inline_asm_strcmp(word, list->data[i].value) == 0) {
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

static int TABLE_NUMBER = 1; 
void make_csv_table(hash_table * table) {

    char table_name[20] = "";
    snprintf(table_name, 9,  "%d", TABLE_NUMBER);
    TABLE_NUMBER++;
    strcat(table_name, "table.csv");
    char folder[30] = "tables/";
    strcat(folder, table_name);


    FILE * csv = fopen(folder, "w");

    if (csv == NULL) {
        printf("NULL no \"tables\" folder found in repository\n");
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

int main(void) {

    for (size_t k = 81000; k < 1000000; k += 5000) {

    hash_table_size = k;

    hash_table table = {};
    hash_table_ctor(&table, CRC32_modified);

    read_file_to_table(&table);

    unsigned long long res = 0;
    int max_number = 10000;

    // NOTE: you can add few initial runs to load caches
    for (int n = 0; n < max_number; n++) {

        unsigned long long start = __rdtsc();        

        for (int i = 0; i < hash_table_size; i++) { // NOTE: Can you try to not lookup every element

            if (table.data[i] == NULL) continue;
            for (int j = 1; j < table.data[i]->list_size; j++) {
                hash_table_search(table.data[i]->data[j].value, &table);
            }
        }

        unsigned long long end = __rdtsc(); // NOTE: Can you calculate nanoseconds from clock (check out procps source code, it probably has a solution, also htop)

        res += (end-start);
    }

    FILE * table_perf = fopen("table_size.csv", "a");

    fprintf(table_perf, "%lu;%llu\n", k, res/max_number);

    fclose(table_perf);

    

    // make_csv_table(&table);
    hash_table_dtor(&table);
    // printf("\n\n%llu\n\n",res/max_number);

    }
}