#include <crc32intrin.h>
#include <cstddef>
#include <cstdint>
#include "hash_table.h"
#include <immintrin.h>
#include <stddef.h>


size_t always_zero_func(char * word, int len_of_word) {
    return 50;
}

size_t first_letter_func(char * word, int len_of_word) {
    return word[0];
}

size_t word_len_func(char * word, int len_of_word) {
    return len_of_word;
}

size_t ascii_sum_func(char * word, int len_of_word) {
    size_t sum = 0;
    for (int i = 0; i < len_of_word; i++) {
        sum += word[i];
    }
    return sum;
}

size_t ascii_sum_div_len_func(char * word, int len_of_word) {
    size_t sum = 0;
    for (int i = 0; i < len_of_word; i++) {
        sum += word[i];
    }
    return sum / len_of_word;
}

inline size_t my_rol(size_t x) {
    return (x << 1) | (x >> 63);
}

inline size_t my_ror(size_t x) {
    return (x >> 1) | (x << 63);
}

size_t ror_hash_func(char * word, int len_of_word) {
    size_t hash = 0;
    for (int i = 0; i < len_of_word; i++) {
        hash = my_ror(hash) ^ word[i];
    }
    return hash % hash_table_size; // TODO: hash table should do this... (or add assert and docs to hash function)
}

size_t rol_hash_func(char * word, int len_of_word) {
    size_t hash = 0;
    for (int i = 0; i < len_of_word; i++) {
        hash = my_rol(hash) ^ word[i];
    }
    return hash % hash_table_size;
}

size_t CRC32(char * word, int len_of_word) {
    uint32_t crc = 0xffffffff;

    unsigned char* str = (unsigned char*)word;

    while (len_of_word-- != 0) {
        crc ^= *str++;
        for (int bit = 0; bit < 8; bit++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xedb88320;
            } else {
                crc = crc >> 1;
            }
        }
    }
    return ~crc % hash_table_size;
}

size_t CRC32_modified(char * word) {

	size_t hash = 0;
	uint32_t crc = 0x407EF1CA;

	for (size_t i = 0; word[i] != '\0'; i++) {
		hash = _mm_crc32_u8 (hash, word[i]);
	}

	return hash % hash_table_size;
}

