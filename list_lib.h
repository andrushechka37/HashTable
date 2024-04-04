#pragma once



#include <cstdlib>
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

inline int graph_number = 1;
const int command_len = 1000;
const int comment_len = 100;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void create_new_graph(void) {  // TODO: temporary files

    char command1[command_len] = "dot -Tpng /root/HASH/grath.dot  -o /root/HASH/graphs/graph";
    char command2[] = ".png";
    char graph_number_str[2] = {};

    snprintf(graph_number_str, 2,  "%d", graph_number);
    strcat(command1, graph_number_str);
    strcat(command1, command2);
    system(command1);
    graph_number++;
}


void list_visualize(doubly_linked_list * list, const char * comment) {
    FILE * pfile = fopen("grath.dot", "wb");
    fprintf(pfile, "digraph structs {\n");
    fprintf(pfile, "\trankdir=LR;\n");
    fprintf(pfile, "\tgraph [bgcolor=\"#31353b\"]\n");
    fprintf(pfile, "\tnode[color=\"black\",fontsize=14];\n");
    fprintf(pfile, "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n\n\n");

    fprintf(pfile, "\t50 [shape=note,style=filled, fillcolor=\"#fdf39b\", label=\"%s\", fontcolor = \"black\", fontsize = 20];\n", comment);

    for (int i = 0; i < list->list_capacity; i++) {
        fprintf(pfile, "\t%d [shape=Mrecord,style=filled, fillcolor=\"#7293ba\", label=\" ip: %d ", i, i);
        fprintf(pfile, "| data: %s", list->data[i].value);
        fprintf(pfile, "| next: %d", list->data[i].next);
        if (list->data[i].prev == free_elem_marker) {
            fprintf(pfile, "| prev: fre\" ");
        } else {
            fprintf(pfile, "| prev: %d\" ", list->data[i].prev);
        }
        if (list->data[i].prev == free_elem_marker) {
             fprintf(pfile, ", color = lightgreen];\n");
        } else {
            fprintf(pfile, "];\n");
        }
    }

    fprintf(pfile, "\n\t");
    for (int i = 0; i < list->list_capacity - 1; i++) {
        fprintf(pfile, "%d->", i);
    }
    fprintf(pfile, "%d[weight = 100, color = \"invis\"];\n", list->list_capacity - 1);


    for(int i = 0; i < list->list_capacity - 1; i++) {
        if (list->data[i].prev == free_elem_marker) {     // draw next line
            fprintf(pfile, "\t%d->%d[color = \"#22f230\", constraint=false];\n", i, list->data[i].next);
        } else if (list->data[i].next == 0) {
            fprintf(pfile, "\t%d->%d[color = \"#8139bd\", constraint=false];\n", i, list->data[i].next);
        } else {
            fprintf(pfile, "\t%d->%d[color = \"#0ae7ff\", constraint=false];\n", i, list->data[i].next);
        }
    }


    for (int i = 0; i < list->list_capacity; i++) {
        if (list->data[i].prev != free_elem_marker && list->data[i].prev != list->data[0].prev) {   // draw prev line
            fprintf(pfile, "\t%d -> %d[color = \"#ff0a0a\", constraint=false];\n", i, list->data[i].prev);
        } else if (list->data[i].prev == list->data[0].prev) {
            fprintf(pfile, "\t%d -> %d[color = \"#8139bd\", constraint=false];\n", i, list->data[i].prev);
        }
    }

    fprintf(pfile, "\th [shape=tripleoctagon,label=\"HEAD\", color = \"yellow\", fillcolor=\"#7293ba\",style=filled  ];\n");
    fprintf(pfile, "\tt [shape=tripleoctagon,label=\"TALE\", color = \"yellow\", fillcolor=\"#7293ba\",style=filled ];\n");
    fprintf(pfile, "\tf [shape=tripleoctagon,label=\"FREE\", color = \"yellow\", fillcolor=\"#7293ba\",style=filled ];\n");
    fprintf(pfile, "\th->t->f[weight = 100, color = \"invis\"];\n");
    fprintf(pfile, "\th->%d[color = \"orange\", constraint=false];\n", list->data[0].next);
    fprintf(pfile, "\tt->%d[color = \"orange\", constraint=false];\n", list->data[0].prev);
    fprintf(pfile, "\tf->%d[color = \"orange\", constraint=false];\n", list->free_element_head);
    fprintf(pfile, "\n}");
    fclose(pfile);


    create_new_graph();   
}

void html_dump(void) {
    FILE * pfile = fopen("log.html", "w"); // 

    fprintf(pfile, "<hr/>\n");
    fprintf(pfile, "<head>\n");
    fprintf(pfile, "\t<title>90</title>\n");
    fprintf(pfile, "</head>\n");
    for (int i = 1; i < graph_number; i++) {
        fprintf(pfile, "<img src = \"/root/HASH/graphs/graph");
        fprintf(pfile, "%d", i);
        fprintf(pfile, ".png\">\n");
        fprintf(pfile,"<br><br><br><br>\n");
    }
    fclose(pfile);
    
}


static int  get_vacant_cell(doubly_linked_list * list);
static void add_vacant_cell(doubly_linked_list * list, int position);

bool verify_list(doubly_linked_list * list) {
    bool error = 0;

    if (list->data == NULL) {  
        printf("data zero ptr");
        error = 1;
        return 1;     
    } 

    for (int i = 0; i < list->list_capacity; i++) {
        if (list->data[list->data[i].prev].next != i && list->data[i].prev != free_elem_marker) {
            printf("%d %d", list->data[list->data[i].prev].next, i);
            printf("next of previous elem is %d element is %d", list->data[list->data[i].prev].next, i);
            return 1; 
        }
        if (list->data[list->data[i].next].prev != i && list->data[i].prev != free_elem_marker) {
            printf("previous of next element is %d element is %d", list->data[list->data[i].next].prev, i);
            return 1; 
        }
    }


    if(list->free_element_head < 0) { 
        printf("free below zero");
        error = 1;       
    } 

    if(list->free_element_head > list->list_capacity) { 
        printf("free is out of range    %d    \n", list->free_element_head);
        error = 1;       
    } 

    if (list->data[list->free_element_head].next == 0) {
        printf("no more free space");
        error = 1;
    }

    if (error == 1) {
        list_visualize(list, "error, check console output");
        return 1;
    }

    return 0;
}



int list_insert_after(doubly_linked_list * list, int position, char * value, int given_len) {

    int cur = get_vacant_cell(list);

    list->data[list->data[position].next].prev = cur;      // prev of the next elem is current

    list->data[cur].prev = position; 
    
    list->data[cur].value = (char *) calloc (max_len_of_word, 1);
    strcpy(list->data[cur].value, value);

    list->data[cur].next = list->data[position].next;
    list->data[cur].len_of_word = given_len;

    list->data[position].next = cur;

    list->list_size++;
    return 0;     

}



doubly_linked_list * list_ctor(void) {


    doubly_linked_list * list = (doubly_linked_list *) calloc(sizeof(doubly_linked_list), 1);

    list->data = (list_element *) calloc(max_list_capacity, sizeof(list_element));

    for (int i = 1; i < max_list_capacity; i++) {
        list->data[i].prev = free_elem_marker;
        list->data[i].next = i + 1;
        list->data[i].len_of_word = 0;
        list->data[i].value = "\0";
    }

    list->free_element_head = 1;
    list->list_size = 0;
    list->list_capacity = max_list_capacity;
    return list;
}

int list_dtor(doubly_linked_list * list) {
    for(int i = 0; i < list->list_capacity; i++) {
        list->data[i].prev  = 0; 
        list->data[i].value = 0; 
        list->data[i].next  = 0; 
    }
    list->list_capacity = 0;
    list->list_size = 0;
    list->free_element_head = 0;
    free(list->data);
    list->data = NULL;
    return 0;
}

void list_realock_up(doubly_linked_list * list) {
    list->data = (list_element *) realloc(list->data, sizeof(list_element) * list->list_capacity * 2);

    for (int i = list->free_element_head; i < list->list_capacity * 2; i++) {
        list->data[i].prev = free_elem_marker;
        list->data[i].next = i + 1;
        list->data[i].len_of_word = 0;
        list->data[i].value = "\0";
    }

    list->list_capacity *= 2;

    return;
}

static int get_vacant_cell(doubly_linked_list * list) {

    if (list->free_element_head == list->list_capacity - 1) {
        list_realock_up(list);
    }

    int position = list->free_element_head;
    list->free_element_head = list->data[list->free_element_head].next; 

    // if (free elem == size) -> realloc x2
    return position;
}

static void add_vacant_cell(doubly_linked_list * list, int position) {
    list->data[position].prev = free_elem_marker;
    list->data[position].value = 0;
    list->data[position].next  = list->free_element_head;
    list->free_element_head = position;
}



void list_cell_open(FILE * pfile, doubly_linked_list * list) {
        fprintf(pfile, "\n     ");
        for (int i = 0; i < list->list_capacity; i++) {

        if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

            for (int j = 0; j < (list->data[i]).len_of_word + 3; j++) {
                 fprintf(pfile, "_");
            }

        } else {
            fprintf(pfile, "______");
        }
    }
    fprintf(pfile, "_");

    fprintf(pfile, "\n");
    return;
}

void list_cell_close(FILE * pfile, doubly_linked_list * list) {


    fprintf(pfile, "\n     ");

        for (int i = 0; i < list->list_capacity; i++) {

        if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

            fprintf(pfile, "|");
            for (int j = 0; j < (list->data[i]).len_of_word + 2; j++) {
                fprintf(pfile, " ");
            }

        } else {
            fprintf(pfile, "|     ");
        }
    }


    fprintf(pfile, "|");
    fprintf(pfile, "\n     ");



        for (int i = 0; i < list->list_capacity; i++) {

        if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

            fprintf(pfile, "|");
            for (int j = 0; j < (list->data[i]).len_of_word + 2; j++) {
                fprintf(pfile, " ");
            }

        } else {
            fprintf(pfile, "|     ");
        }
    }

    fprintf(pfile, "|");
    fprintf(pfile, "\n     ");




        for (int i = 0; i < list->list_capacity; i++) {

        if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

            fprintf(pfile, "|");
            for (int j = 0; j < (list->data[i]).len_of_word + 2; j++) {
                fprintf(pfile, "_");
            }

        } else {
            fprintf(pfile, "|_____");
        }
    }    
    fprintf(pfile, "|");
    return;
}

void dump_list_txt(doubly_linked_list * list, FILE * pfile) {



    //list_cell_open(pfile, list);

    fprintf(pfile, "  ip:| ");
    for (int i = 0; i < list->list_capacity; i++) {

        if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {


            fprintf(pfile, "%.3d", i);
            for (int j = 0; j < (list->data[i]).len_of_word - 2; j++) {
                 fprintf(pfile, " ");
            }

            fprintf(pfile, "| ");

        } else {
            fprintf(pfile, "%.3d | ", i);
        }
    }

    // list_cell_close(pfile, list);



    fprintf(pfile, "\ndata:|");
    for (int i = 37; i < list->list_capacity; i++) {
        if ((list->data[i]).value == 0) {

            fprintf(pfile, " nul |");
        } else {

            if ((list->data[i]).len_of_word < 3) {
                fprintf(pfile, " %s", (list->data[i]).value);
                for (int j = 0; j < (4 - (list->data[i]).len_of_word); j++) {
                    fprintf(pfile, " ");
                }
                fprintf(pfile, "|");
            } else {
                fprintf(pfile, " %.20s |", (list->data[i]).value);

            }
            
        }
    }

    //list_cell_close(pfile, list);



    fprintf(pfile, "\n\nhead: [%.3d]\n", list->data[0].next);
    fprintf(pfile, "tale: [%.3d]\n", list->data[0].prev);
    fprintf(pfile, "free: [%.3d]\n\n", list->free_element_head);
    fprintf(pfile, "size: [%.3d]\n", list->list_size);
    fprintf(pfile, "capa: [%.3d]\n", list->list_capacity);

}






















// #pragma once



// #include <cstdlib>
// #define verify(list)               \
// if (verify_list(&list) != 0) {     \
//     return -1;                     \
// }


// const int max_list_capacity = 5;
// const int max_len_of_word = 20;                      

// const int free_elem_marker =  -1; 

// struct list_element {
//     char * value; 
//     int prev;
//     int next;
//     int len_of_word;
// };

// struct doubly_linked_list {
//     list_element * data;
//     int free_element_head;
//     int list_size;
//     int list_capacity;
// };


// bool verify_list(doubly_linked_list * list);

// int list_insert_after(doubly_linked_list * list, int position, char * value, int given_len);


// doubly_linked_list * list_ctor(void);
// int list_dtor(doubly_linked_list * list);

// void create_new_graph(void);
// void list_visualize(doubly_linked_list * list, const char * comment);

// void unite_visualizations(void);

// void html_dump(void);

// inline int graph_number = 1;
// const int command_len = 1000;
// const int comment_len = 100;

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>


// void create_new_graph(void) {  // TODO: temporary files

//     char command1[command_len] = "dot -Tpng /root/HASH/grath.dot  -o /root/HASH/graphs/graph";
//     char command2[] = ".png";
//     char graph_number_str[2] = {};

//     snprintf(graph_number_str, 2,  "%d", graph_number);
//     strcat(command1, graph_number_str);
//     strcat(command1, command2);
//     system(command1);
//     graph_number++;
// }


// void list_visualize(doubly_linked_list * list, const char * comment) {
//     FILE * pfile = fopen("grath.dot", "wb");
//     fprintf(pfile, "digraph structs {\n");
//     fprintf(pfile, "\trankdir=LR;\n");
//     fprintf(pfile, "\tgraph [bgcolor=\"#31353b\"]\n");
//     fprintf(pfile, "\tnode[color=\"black\",fontsize=14];\n");
//     fprintf(pfile, "\tedge[color=\"darkgreen\",fontcolor=\"blue\",fontsize=12];\n\n\n");

//     fprintf(pfile, "\t50 [shape=note,style=filled, fillcolor=\"#fdf39b\", label=\"%s\", fontcolor = \"black\", fontsize = 20];\n", comment);

//     for (int i = 0; i < list->list_capacity; i++) {
//         fprintf(pfile, "\t%d [shape=Mrecord,style=filled, fillcolor=\"#7293ba\", label=\" ip: %d ", i, i);
//         fprintf(pfile, "| data: %s", list->data[i].value);
//         fprintf(pfile, "| next: %d", list->data[i].next);
//         if (list->data[i].prev == free_elem_marker) {
//             fprintf(pfile, "| prev: fre\" ");
//         } else {
//             fprintf(pfile, "| prev: %d\" ", list->data[i].prev);
//         }
//         if (list->data[i].prev == free_elem_marker) {
//              fprintf(pfile, ", color = lightgreen];\n");
//         } else {
//             fprintf(pfile, "];\n");
//         }
//     }

//     fprintf(pfile, "\n\t");
//     for (int i = 0; i < list->list_capacity - 1; i++) {
//         fprintf(pfile, "%d->", i);
//     }
//     fprintf(pfile, "%d[weight = 100, color = \"invis\"];\n", list->list_capacity - 1);


//     for(int i = 0; i < list->list_capacity - 1; i++) {
//         if (list->data[i].prev == free_elem_marker) {     // draw next line
//             fprintf(pfile, "\t%d->%d[color = \"#22f230\", constraint=false];\n", i, list->data[i].next);
//         } else if (list->data[i].next == 0) {
//             fprintf(pfile, "\t%d->%d[color = \"#8139bd\", constraint=false];\n", i, list->data[i].next);
//         } else {
//             fprintf(pfile, "\t%d->%d[color = \"#0ae7ff\", constraint=false];\n", i, list->data[i].next);
//         }
//     }


//     for (int i = 0; i < list->list_capacity; i++) {
//         if (list->data[i].prev != free_elem_marker && list->data[i].prev != list->data[0].prev) {   // draw prev line
//             fprintf(pfile, "\t%d -> %d[color = \"#ff0a0a\", constraint=false];\n", i, list->data[i].prev);
//         } else if (list->data[i].prev == list->data[0].prev) {
//             fprintf(pfile, "\t%d -> %d[color = \"#8139bd\", constraint=false];\n", i, list->data[i].prev);
//         }
//     }

//     fprintf(pfile, "\th [shape=tripleoctagon,label=\"HEAD\", color = \"yellow\", fillcolor=\"#7293ba\",style=filled  ];\n");
//     fprintf(pfile, "\tt [shape=tripleoctagon,label=\"TALE\", color = \"yellow\", fillcolor=\"#7293ba\",style=filled ];\n");
//     fprintf(pfile, "\tf [shape=tripleoctagon,label=\"FREE\", color = \"yellow\", fillcolor=\"#7293ba\",style=filled ];\n");
//     fprintf(pfile, "\th->t->f[weight = 100, color = \"invis\"];\n");
//     fprintf(pfile, "\th->%d[color = \"orange\", constraint=false];\n", list->data[0].next);
//     fprintf(pfile, "\tt->%d[color = \"orange\", constraint=false];\n", list->data[0].prev);
//     fprintf(pfile, "\tf->%d[color = \"orange\", constraint=false];\n", list->free_element_head);
//     fprintf(pfile, "\n}");
//     fclose(pfile);


//     create_new_graph();   
// }

// void html_dump(void) {
//     FILE * pfile = fopen("log.html", "w"); // 

//     fprintf(pfile, "<hr/>\n");
//     fprintf(pfile, "<head>\n");
//     fprintf(pfile, "\t<title>90</title>\n");
//     fprintf(pfile, "</head>\n");
//     for (int i = 1; i < graph_number; i++) {
//         fprintf(pfile, "<img src = \"/root/HASH/graphs/graph");
//         fprintf(pfile, "%d", i);
//         fprintf(pfile, ".png\">\n");
//         fprintf(pfile,"<br><br><br><br>\n");
//     }
//     fclose(pfile);
    
// }


// static int  get_vacant_cell(doubly_linked_list * list);
// static void add_vacant_cell(doubly_linked_list * list, int position);

// bool verify_list(doubly_linked_list * list) {
//     bool error = 0;

//     if (list->data == NULL) {  
//         printf("data zero ptr");
//         error = 1;
//         return 1;     
//     } 

//     for (int i = 0; i < list->list_capacity; i++) {
//         if (list->data[list->data[i].prev].next != i && list->data[i].prev != free_elem_marker) {
//             printf("%d %d", list->data[list->data[i].prev].next, i);
//             printf("next of previous elem is %d element is %d", list->data[list->data[i].prev].next, i);
//             return 1; 
//         }
//         if (list->data[list->data[i].next].prev != i && list->data[i].prev != free_elem_marker) {
//             printf("previous of next element is %d element is %d", list->data[list->data[i].next].prev, i);
//             return 1; 
//         }
//     }


//     if(list->free_element_head < 0) { 
//         printf("free below zero");
//         error = 1;       
//     } 

//     if(list->free_element_head > list->list_capacity) { 
//         printf("free is out of range    %d    \n", list->free_element_head);
//         error = 1;       
//     } 

//     if (list->data[list->free_element_head].next == 0) {
//         printf("no more free space");
//         error = 1;
//     }

//     if (error == 1) {
//         list_visualize(list, "error, check console output");
//         return 1;
//     }

//     return 0;
// }



// int list_insert_after(doubly_linked_list * list, int position, char * value, int given_len) {

//     int cur = get_vacant_cell(list);

//     list->data[list->data[position].next].prev = cur;      // prev of the next elem is current

//     list->data[cur].prev = position; 
    
//     list->data[cur].value = (char *) calloc (max_len_of_word, 1);
//     strcpy(list->data[cur].value, value);

//     list->data[cur].next = list->data[position].next;
//     list->data[cur].len_of_word = given_len;

//     list->data[position].next = cur;

//     list->list_size++;
//     return 0;     

// }



// doubly_linked_list * list_ctor(void) {


//     doubly_linked_list * list = (doubly_linked_list *) calloc(sizeof(doubly_linked_list), 1);

//     list->data = (list_element *) calloc(max_list_capacity, sizeof(list_element));

//     for (int i = 1; i < max_list_capacity; i++) {
//         list->data[i].prev = free_elem_marker;
//         list->data[i].next = i + 1;
//     }

//     list->free_element_head = 1;
//     list->list_size = 0;
//     list->list_capacity = max_list_capacity;
//     return list;
// }

// int list_dtor(doubly_linked_list * list) {
//     for(int i = 0; i < list->list_capacity; i++) {
//         list->data[i].prev  = 0; 
//         list->data[i].value = 0; 
//         list->data[i].next  = 0; 
//     }
//     list->list_capacity = 0;
//     list->list_size = 0;
//     list->free_element_head = 0;
//     free(list->data);
//     list->data = NULL;
//     return 0;
// }

// void list_realock_up(doubly_linked_list * list) {
//     list->data = (list_element *) realloc(list->data, sizeof(list_element) * list->list_capacity * 2);

//     for (int i = list->free_element_head; i < list->list_capacity * 2; i++) {
//         list->data[i].prev = free_elem_marker;
//         list->data[i].next = i + 1;
//     }

//     list->list_capacity *= 2;

//     return;
// }

// static int get_vacant_cell(doubly_linked_list * list) {

//     if (list->free_element_head == list->list_capacity - 1) {
//         list_realock_up(list);
//     }

//     int position = list->free_element_head;
//     list->free_element_head = list->data[list->free_element_head].next; 

//     // if (free elem == size) -> realloc x2
//     return position;
// }

// static void add_vacant_cell(doubly_linked_list * list, int position) {
//     list->data[position].prev = free_elem_marker;
//     list->data[position].value = 0;
//     list->data[position].next  = list->free_element_head;
//     list->free_element_head = position;
// }



// void list_cell_open(FILE * pfile, doubly_linked_list * list) {
//         fprintf(pfile, "\n     ");
//         for (int i = 0; i < list->list_capacity; i++) {

//         if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

//             for (int j = 0; j < (list->data[i]).len_of_word + 3; j++) {
//                  fprintf(pfile, "_");
//             }

//         } else {
//             fprintf(pfile, "______");
//         }
//     }
//     fprintf(pfile, "_");

//     fprintf(pfile, "\n");
//     return;
// }

// void list_cell_close(FILE * pfile, doubly_linked_list * list) {


//     fprintf(pfile, "\n     ");

//         for (int i = 0; i < list->list_capacity; i++) {

//         if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

//             fprintf(pfile, "|");
//             for (int j = 0; j < (list->data[i]).len_of_word + 2; j++) {
//                 fprintf(pfile, " ");
//             }

//         } else {
//             fprintf(pfile, "|     ");
//         }
//     }


//     fprintf(pfile, "|");
//     fprintf(pfile, "\n     ");



//         for (int i = 0; i < list->list_capacity; i++) {

//         if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

//             fprintf(pfile, "|");
//             for (int j = 0; j < (list->data[i]).len_of_word + 2; j++) {
//                 fprintf(pfile, " ");
//             }

//         } else {
//             fprintf(pfile, "|     ");
//         }
//     }

//     fprintf(pfile, "|");
//     fprintf(pfile, "\n     ");




//         for (int i = 0; i < list->list_capacity; i++) {

//         if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {

//             fprintf(pfile, "|");
//             for (int j = 0; j < (list->data[i]).len_of_word + 2; j++) {
//                 fprintf(pfile, "_");
//             }

//         } else {
//             fprintf(pfile, "|_____");
//         }
//     }    
//     fprintf(pfile, "|");
//     return;
// }

// void dump_list_txt(doubly_linked_list * list, FILE * pfile) {



//     list_cell_open(pfile, list);

//     fprintf(pfile, "  ip:| ");
//     for (int i = 0; i < list->list_capacity; i++) {

//         if ((list->data[i]).value != 0 && (list->data[i]).len_of_word >= 3) {


//             fprintf(pfile, "%.3d", i);
//             for (int j = 0; j < (list->data[i]).len_of_word - 2; j++) {
//                  fprintf(pfile, " ");
//             }

//             fprintf(pfile, "| ");

//         } else {
//             fprintf(pfile, "%.3d | ", i);
//         }
//     }

//     list_cell_close(pfile, list);



//     fprintf(pfile, "\ndata:|");
//     for (int i = 0; i < list->list_capacity; i++) {
//         if ((list->data[i]).value == 0) {

//             fprintf(pfile, " nul |");
//         } else {

//             if ((list->data[i]).len_of_word < 3) {
//                 fprintf(pfile, " %s", (list->data[i]).value);
//                 for (int j = 0; j < (4 - (list->data[i]).len_of_word); j++) {
//                     fprintf(pfile, " ");
//                 }
//                 fprintf(pfile, "|");
//             } else {
//                 fprintf(pfile, " %.20s |", (list->data[i]).value);

//             }
            
//         }
//     }

//     list_cell_close(pfile, list);



//     fprintf(pfile, "\n\nhead: [%.3d]\n", list->data[0].next);
//     fprintf(pfile, "tale: [%.3d]\n", list->data[0].prev);
//     fprintf(pfile, "free: [%.3d]\n\n", list->free_element_head);
//     fprintf(pfile, "size: [%.3d]\n", list->list_size);
//     fprintf(pfile, "capa: [%.3d]\n", list->list_capacity);

// }
