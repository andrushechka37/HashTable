#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

// dispersion
// wtf with inline

void hash_table_ctor(hash_table * table,  size_t (*hash_table_func)(char * word, int len_of_word)) {

    (table->data) = (doubly_linked_list **) calloc(hash_table_size, sizeof(doubly_linked_list *));
    table->hash_table_func = hash_table_func;
    return;
}


int get_free_cell_in_list(doubly_linked_list * list, char * word) {

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

    int position = get_free_cell_in_list(CUR_LIST_PTR, word);
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
        printf("NULL\n");
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
    CRC32
    };


    for (int i = 0; i < quantity_of_func; i++) {

        hash_table table = {};
        hash_table_ctor(&table, hash_table_func_array[i]);

        FILE * data = fopen("data_storage.txt", "r"); // hard code of file name is very very ploho
        read_file_to_table(data, &table);
        fclose(data);

        printf("\nload factor of %d table is: %lf\n", i + 1, (float)table.sum_of_words/table.quantity_of_lists);

        double disp = 0;
        double average = (double)table.sum_of_words/hash_table_size;

        for (int i = 0; i < hash_table_size; i++) {
            if (table.data[i] == NULL) {
                disp += average * average;
            } else {
                disp += (table.data[i]->list_size - average) * (table.data[i]->list_size - average);
            }
        }
        printf("\ndisp %d table is: %lf\n", i + 1, disp/table.quantity_of_lists);

        make_csv_table(&table);
        hash_table_dtor(&table);
    }
}




//   43.89%  list_test  libc.so.6             [.] __strcmp_sse2_unaligned                                                                                                                                        ◆
//   19.08%  list_test  list_test             [.] get_free_cell_in_list                                                                                                                                          ▒
//   12.60%  list_test  libc.so.6             [.] __vfscanf_internal                                                                                                                                             ▒
//    9.92%  list_test  list_test             [.] strcmp@plt                                                                                                                                                     ▒
//    0.76%  list_test  [kernel.kallsyms]     [k] __add_to_page_cache_locked                                                                                                                                     ▒
//    0.76%  list_test  [kernel.kallsyms]     [k] __softirqentry_text_start                                                                                                                                      ▒
//    0.76%  list_test  [kernel.kallsyms]     [k] clear_page_erms                                                                                                                                                ▒
//    0.76%  list_test  ld-linux-x86-64.so.2  [.] _dl_relocate_object                                                                                                                                            ▒
//    0.76%  list_test  libc.so.6             [.] _IO_file_xsputn@@GLIBC_2.2.5                                                                                                                                   ▒
//    0.76%  list_test  libc.so.6             [.] _IO_sputbackc                                                                                                                                                  ▒
//    0.76%  list_test  libc.so.6             [.] __GI_____strtoll_l_internal                                                                                                                                    ▒
//    0.76%  list_test  libc.so.6             [.] __isoc99_fscanf                                                                                                                                                ▒
//    0.76%  list_test  libc.so.6             [.] __vfprintf_internal                                                                                                                                            ▒
//    0.76%  list_test  list_test             [.] hash_table_insert                                                                                                                                              ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] __mod_node_page_state                                                                                                                                          ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] __x64_sys_openat                                                                                                                                               ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] apparmor_inode_getattr                                                                                                                                         ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] delete_from_page_cache_batch                                                                                                                                   ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] do_user_addr_fault                                                                                                                                             ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] iowrite16                                                                                                                                                      ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] rcu_core                                                                                                                                                       ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] rcu_do_batch                                                                                                                                                   ▒
//    0.38%  list_test  [kernel.kallsyms]     [k] zap_pte_range                                                                                                                                                  ▒
//    0.38%  list_test  ld-linux-x86-64.so.2  [.] mmap64                                                                                                                                                         ▒
//    0.38%  list_test  libc.so.6             [.] __libc_calloc  