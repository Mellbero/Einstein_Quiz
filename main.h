#ifndef MAIN
#define MAIN

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "minitrace.h"

#ifdef TRACE
#define TRACE_PRINT(x) printf("TRACE: %s\n", x)
#else
#define TRACE_PRINT(x)
#endif

#define HOUSES 5
#define OPTIONS 5
#define CATEGORIES 6

#define HOUSE 5
#define HOUSE_B 0b100000

#define FIRST 0b10000 << (HOUSES * HOUSE)
#define SECOND 0b01000 << (HOUSES * HOUSE)
#define MIDDLE 0b00100 << (HOUSES * HOUSE)
#define FOURTH 0b00010 << (HOUSES * HOUSE)
#define FIFTH 0b00001 << (HOUSES * HOUSE)

#define NATIONALITY 4
#define NATIONALITY_B 0b010000

#define NORWEGIAN 0b10000 << (HOUSES * NATIONALITY)
#define DANE 0b01000 << (HOUSES * NATIONALITY)
#define GERMAN 0b00100 << (HOUSES * NATIONALITY)
#define BRITT 0b00010 << (HOUSES * NATIONALITY)
#define SWEDE 0b00001 << (HOUSES * NATIONALITY)

#define COLOUR 3
#define COLOUR_B 0b001000

#define RED 0b10000 << (HOUSES * COLOUR)
#define GREEN 0b01000 << (HOUSES * COLOUR)
#define BLUE 0b00100 << (HOUSES * COLOUR)
#define WHITE 0b00010 << (HOUSES * COLOUR)
#define YELLOW 0b00001 << (HOUSES * COLOUR)

#define BEVERAGE 2
#define BEVERAGE_B 0b000100

#define TEA 0b10000 << (HOUSES * BEVERAGE)
#define COFFEE 0b01000 << (HOUSES * BEVERAGE)
#define BEER 0b00100 << (HOUSES * BEVERAGE)
#define MILK 0b00010 << (HOUSES * BEVERAGE)
#define WATER 0b00001 << (HOUSES * BEVERAGE)

#define CIGARETTE 1
#define CIGARETTE_B 0b000010

#define ROTHMANNS 0b10000 << (HOUSES * CIGARETTE)
#define WINFIELD 0b01000 << (HOUSES * CIGARETTE)
#define DUNHILL 0b00100 << (HOUSES * CIGARETTE)
#define MARLBORO 0b00010 << (HOUSES * CIGARETTE)
#define PALLMALL 0b00001 << (HOUSES * CIGARETTE)

#define PET 0
#define PET_B 0b000001

#define DOG 0b10000 << (HOUSES * PET)
#define BIRD 0b01000 << (HOUSES * PET)
#define CAT 0b00100 << (HOUSES * PET)
#define HORSE 0b00010 << (HOUSES * PET)
#define FISH 0b00001 << (HOUSES * PET)

#define NEIGHBOUR 0
#define LEFT 1

#define SET_BIT(x, pos) ((x) |= (pos))
#define CLEAR_BIT(x, pos) ((x) &= ~(pos))

struct Solution {
    uint_fast32_t houses [5];
    uint_fast8_t completion [5];
};

inline void swap_blocks(uint_fast32_t *a, uint_fast32_t *b, uint_fast8_t block_index);
inline uint_fast8_t get_block(uint_fast32_t house, uint_fast8_t column_index);
inline void printBinary(uint_fast8_t num);
inline uint_fast8_t find_set_bit_pos(uint_fast32_t house, uint_fast8_t column_index);
inline void print_sol(struct Solution sol);
inline bool apply_rule(struct Solution* end_sol, uint_fast32_t to_search, uint_fast32_t to_swap, uint_fast8_t to_search_column, uint_fast32_t to_search_column_b, uint_fast8_t to_swap_column, uint_fast32_t to_swap_column_b, uint_fast8_t rule);
inline bool apply_rule_neighbour(struct Solution* end_sol, uint_fast32_t to_search_1, uint_fast32_t to_search_2, uint_fast8_t to_search_1_column, uint_fast32_t to_search_1_column_b, uint_fast8_t to_search_2_column, uint_fast32_t to_search_2_column_b, uint_fast8_t neighbour, uint_fast8_t rule);
inline bool assign(struct Solution* sol, uint_fast8_t rule);


#endif