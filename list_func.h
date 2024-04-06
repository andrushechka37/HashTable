#pragma once

#include <cstddef>
#include <cstdio>
#define verify(list)               \
if (verify_list(&list) != 0) {     \
    return -1;                     \
}


const int max_list_capacity = 5;
const int max_len_of_word = 20;                      

const int free_elem_marker =  -1; 

struct list_element {
    char * value; 
    int prev;
    int next;
    int len_of_word;
};

struct doubly_linked_list {
    list_element * data;
    int free_element_head;
    int list_size;
    int list_capacity;
};


bool verify_list(doubly_linked_list * list);

int list_insert_after(doubly_linked_list * list, int position, char * value, int given_len);


doubly_linked_list * list_ctor(void);
int list_dtor(doubly_linked_list * list);

void create_new_graph(void);
void list_visualize(doubly_linked_list * list, const char * comment);

void unite_visualizations(void);

void html_dump(void);
void dump_list_txt(doubly_linked_list * list, FILE * pfile);

inline int graph_number = 1;
const int command_len = 1000;
const int comment_len = 100;
