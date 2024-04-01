#pragma once
#include "list_lib.h"

const int hash_table_size = 10;

struct hash_table {
    //pointer to hash func
    doubly_linked_list ** data;
};