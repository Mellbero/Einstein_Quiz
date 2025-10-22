#include "main.h"

void swap_blocks(uint_fast32_t *a, uint_fast32_t *b, uint_fast8_t block_index){
    uint_fast32_t block = 0b11111 << ((CATEGORIES - 1) * block_index);
    uint_fast32_t x = (*a ^ *b) & block;
    *a ^= x;
    *b ^= x;
}

uint_fast8_t get_block(uint_fast32_t house, uint_fast8_t column_index){
    uint_fast32_t block = 0b11111 << ((CATEGORIES - 1) * column_index);
    return (house & block) >> ((CATEGORIES - 1) * column_index);
}

void printBinary(uint_fast8_t num) {
    for (int i = CATEGORIES -1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

uint_fast8_t find_set_bit_pos(uint_fast32_t house, uint_fast8_t column_index) {
    uint_fast8_t block = get_block(house, column_index);
    for (uint_fast8_t house_i = 0; house_i < HOUSES; ++house_i) {
        if (block & (1u << house_i))
            return house_i;
    }
    return -1;
}

void print_sol(struct Solution sol) {
    const char *words[][5] = {
        {"FIFTH", "FOURTH", "MIDDLE", "SECOND", "FIRST"},
        {"SWEDE", "BRITT", "GERMAN", "DANE", "NORWEGIAN"},
        {"YELLOW", "WHITE", "BLUE", "GREEN", "RED"},
        {"WATER", "MILK", "BEER", "COFFEE", "TEA"},
        {"PALLMALL", "MARLBORO", "DUNHILL", "WINFIELD", "ROTHMANNS"},
        {"FISH", "HORSE", "CAT", "BIRD", "DOG"}
    };

    printf("%-8s %-12s %-8s %-8s %-12s %-6s\n", 
           "House", "Nationality", "Color", "Drink", "Cigarette", "Pet");
    printf("---------------------------------------------------------------\n");

    for(uint_fast8_t house_i = 0; house_i < HOUSES; ++house_i) {
        const char *columns[CATEGORIES];

        for(uint_fast8_t categorie_i = 0; categorie_i < CATEGORIES; ++categorie_i) {
            uint_fast8_t p = find_set_bit_pos(sol.houses[house_i], (CATEGORIES-1)-categorie_i);
            columns[categorie_i] = words[categorie_i][p];
        }

        printf("%-8s %-12s %-8s %-8s %-12s %-6s\n",
               columns[0], columns[1], columns[2], columns[3], columns[4], columns[5]);
    }

    printf("\n");

    printf("completion a: "); printBinary(sol.completion[0]);
    printf("completion b: "); printBinary(sol.completion[1]);
    printf("completion c: "); printBinary(sol.completion[2]);
    printf("completion d: "); printBinary(sol.completion[3]);
    printf("completion e: "); printBinary(sol.completion[4]);
    printf("\n");
}


bool apply_rule(struct Solution* end_sol, uint_fast32_t to_search, uint_fast32_t to_swap, uint_fast8_t to_search_column, uint_fast32_t to_search_column_b, uint_fast8_t to_swap_column, uint_fast32_t to_swap_column_b, uint_fast8_t rule){
    #ifdef TRACE
        MTR_SCOPE("c++", "apply_rule");
    #endif
    struct Solution sol = *end_sol;
    uint_fast8_t to_search_init_i = 0;
    for(uint_fast8_t to_search_f_i = 0; to_search_f_i < HOUSES; ++to_search_f_i){
        if((sol.houses[to_search_f_i] & to_search) != 0){
            to_search_init_i = to_search_f_i;
        }
    }
    for(uint_fast8_t to_search_i = 0; to_search_i < HOUSES; ++to_search_i){
        if(to_search_init_i != to_search_i){
            //Check if to_search_i block has already beed checked.
            if((sol.completion[to_search_i] & to_search_column_b) != 0) continue;
            //If the to_search_init_i block has already been checked it cant be swapped. Inner loop will thus be run only once
            if((sol.completion[to_search_init_i] & to_search_column_b) == 0){
                swap_blocks(&sol.houses[to_search_init_i], &sol.houses[to_search_i], to_search_column);
            }else{
                to_search_i -= 1;
            }
        }
        for(uint_fast8_t to_swap_i = 0; to_swap_i < HOUSES; ++to_swap_i){
            if(to_search_i != to_swap_i){
                //Check if to_swap_i block has already beed checked.
                if((sol.completion[to_swap_i] & to_swap_column_b) != 0) continue;
                //If the to_search_i block has already been checked it cant be swapped. Inner loop will thus be run only once
                if((sol.completion[to_search_i] & to_swap_column_b) == 0){
                    swap_blocks(&sol.houses[to_search_i], &sol.houses[to_swap_i], to_swap_column);
                }else{
                    to_swap_i -= 1;
                }
            }
            //check
            if((sol.houses[to_search_i] & to_swap) != 0){
                bool clear_swap = true;
                bool clear_search = true;
                if((sol.completion[to_search_i] & to_swap_column_b) != 0){
                    clear_swap = false;
                }
                if((sol.completion[to_search_i] & to_search_column_b) != 0){
                    clear_search = false;
                }
                SET_BIT(sol.completion[to_search_i], to_swap_column_b);
                SET_BIT(sol.completion[to_search_i], to_search_column_b); 
                rule += 1;
                *end_sol = sol;

                if(assign(end_sol, rule) == true) return true;
                
                rule -= 1;
                if(clear_swap == true){
                    CLEAR_BIT(sol.completion[to_search_i], to_swap_column_b);
                }
                if(clear_search == true){
                    CLEAR_BIT(sol.completion[to_search_i], to_search_column_b);
                }
            }
            if((sol.completion[to_search_i] & to_swap_column_b) != 0) break;
            swap_blocks(&sol.houses[to_search_i], &sol.houses[to_swap_i], to_swap_column);
        }
        if((sol.completion[to_search_init_i] & to_search_column_b) != 0) break;
        swap_blocks(&sol.houses[to_search_init_i], &sol.houses[to_search_i], to_search_column);
    }
    return false;
}

bool apply_rule_neighbour(struct Solution* end_sol, uint_fast32_t to_search_1, uint_fast32_t to_search_2, uint_fast8_t to_search_1_column, uint_fast32_t to_search_1_column_b, uint_fast8_t to_search_2_column, uint_fast32_t to_search_2_column_b, uint_fast8_t neighbour, uint_fast8_t rule){
    #ifdef TRACE
        MTR_SCOPE("c++", "apply_rule_neighbour");
    #endif
    struct Solution sol = *end_sol;
    uint_fast8_t to_search_1_init_i = 0;
    uint_fast8_t to_search_2_init_i = 0;
    for(uint_fast8_t to_search_f_i = 0; to_search_f_i < HOUSES; ++to_search_f_i){
        if((sol.houses[to_search_f_i] & to_search_1) != 0){
            to_search_1_init_i = to_search_f_i;
        }
        if((sol.houses[to_search_f_i] & to_search_2) != 0){
            to_search_2_init_i = to_search_f_i;
        }
    }
    for(uint_fast8_t to_search_1_i = 0; to_search_1_i < HOUSES; ++to_search_1_i){
        if(to_search_1_init_i != to_search_1_i){
            if((sol.completion[to_search_1_i] & to_search_1_column_b) != 0) continue;
            if(to_search_1_i == to_search_2_init_i) continue;
            if((sol.completion[to_search_1_init_i] & to_search_1_column_b) == 0){
                swap_blocks(&sol.houses[to_search_1_init_i], &sol.houses[to_search_1_i], to_search_1_column);
            }else{
                to_search_1_i -= 1;
            }
        }
        for(uint_fast8_t to_search_1_hn_i = 0; to_search_1_hn_i < HOUSES; ++to_search_1_hn_i){

            if(to_search_1_i != to_search_1_hn_i){
                if((sol.completion[to_search_1_hn_i] & HOUSE_B) != 0) continue;
                if((OPTIONS -1 - find_set_bit_pos(sol.houses[to_search_1_hn_i], HOUSE)) == 0 && neighbour == LEFT) continue;
                if(to_search_1_hn_i == to_search_2_init_i) continue;
                if((sol.completion[to_search_1_i] & HOUSE_B) == 0){
                    swap_blocks(&sol.houses[to_search_1_i], &sol.houses[to_search_1_hn_i], HOUSE);
                }else{
                    to_search_1_hn_i -= 1;
                }
            }
            for(uint_fast8_t to_search_2_i = 0; to_search_2_i < HOUSES; ++to_search_2_i){
    
                if(to_search_2_init_i != to_search_2_i){
                    if((sol.completion[to_search_2_i] & to_search_2_column_b) != 0) continue;
                    if(to_search_2_i == to_search_1_i || to_search_2_i == to_search_1_init_i) continue;
                    if((sol.completion[to_search_2_init_i] & to_search_2_column_b) == 0){
                        swap_blocks(&sol.houses[to_search_2_init_i], &sol.houses[to_search_2_i], to_search_2_column);
                    }else{
                        to_search_2_i -= 1;
                    }
                }

                for(uint_fast8_t to_search_2_hn_i = 0; to_search_2_hn_i < HOUSES; ++to_search_2_hn_i){
        
                    if(to_search_2_i != to_search_2_hn_i){
                        if((sol.completion[to_search_2_hn_i] & HOUSE_B) != 0) continue;
                        if(to_search_2_hn_i == to_search_1_hn_i || to_search_2_hn_i == to_search_1_init_i) continue;
                        if((sol.completion[to_search_2_i] & HOUSE_B) == 0){
                            swap_blocks(&sol.houses[to_search_2_i], &sol.houses[to_search_2_hn_i], HOUSE);
                        }else{
                            to_search_2_hn_i -= 1;
                        }
                    }
                    //check
                    uint_fast8_t house_number_1 = (OPTIONS -1 - find_set_bit_pos(sol.houses[to_search_1_i], HOUSE));
                    uint_fast8_t house_number_2 = (OPTIONS -1 - find_set_bit_pos(sol.houses[to_search_2_i], HOUSE));

                    bool check = false;

                    //Check left
                    if(house_number_2 == house_number_1 - 1){
                        check = true;
                    }
                    if(neighbour == NEIGHBOUR){
                        if(house_number_1 != FOURTH){
                            //Check right
                            if(house_number_2 == house_number_1 + 1){
                                check = true;
                            }
                        }
                    }
        
                    if(check){
                        bool clear_to_search_1 = true;
                        bool clear_to_search_2 = true;
                        bool clear_house_1 = true;
                        bool clear_house_2 = true;
                        if((sol.completion[to_search_1_i] & to_search_1_column_b) != 0){
                            clear_to_search_1 = false;
                        }
                        if((sol.completion[to_search_2_i] & to_search_2_column_b) != 0){
                            clear_to_search_2 = false;
                        }
                        if((sol.completion[to_search_1_i] & HOUSE_B) != 0){
                            clear_house_1 = false;
                        }
                        if((sol.completion[to_search_2_i] & HOUSE_B) != 0){
                            clear_house_2 = false;
                        }
                        SET_BIT(sol.completion[to_search_1_i], to_search_1_column_b);
                        SET_BIT(sol.completion[to_search_2_i], to_search_2_column_b);
                        SET_BIT(sol.completion[to_search_1_i], HOUSE_B); 
                        SET_BIT(sol.completion[to_search_2_i], HOUSE_B);
                        rule += 1;
                        *end_sol = sol;

                        if(assign(end_sol, rule) == true) return true;
                        
                        rule -= 1;
                        if(clear_to_search_1 == true){
                            CLEAR_BIT(sol.completion[to_search_1_i], to_search_1_column_b);
                        }
                        if(clear_to_search_2 == true){
                            CLEAR_BIT(sol.completion[to_search_2_i], to_search_2_column_b);
                        }
                        if(clear_house_1 == true){
                            CLEAR_BIT(sol.completion[to_search_1_i], HOUSE_B);
                        }
                        if(clear_house_2 == true){
                            CLEAR_BIT(sol.completion[to_search_2_i], HOUSE_B);
                        }
                    }
                    if((sol.completion[to_search_2_i] & HOUSE_B) != 0) break;
                    swap_blocks(&sol.houses[to_search_2_i], &sol.houses[to_search_2_hn_i], HOUSE);
                }
            
                if((sol.completion[to_search_2_init_i] & to_search_2_column_b) != 0) break;
                swap_blocks(&sol.houses[to_search_2_init_i], &sol.houses[to_search_2_i], to_search_2_column);
            }
            if((sol.completion[to_search_1_i] & HOUSE_B) != 0) break;
            swap_blocks(&sol.houses[to_search_1_i], &sol.houses[to_search_1_hn_i], HOUSE);
        }
        if((sol.completion[to_search_1_init_i] & to_search_1_column_b) != 0) break;
        swap_blocks(&sol.houses[to_search_1_init_i], &sol.houses[to_search_1_i], to_search_1_column);
    }
    return false;
}

bool assign(struct Solution* sol, uint_fast8_t rule){   
    #ifdef TRACE
        MTR_SCOPE("c++", "assign");
    #endif
    switch (rule)
    {
    case 0: //Rule 11
        return apply_rule(sol, NORWEGIAN, FIRST, NATIONALITY, NATIONALITY_B, HOUSE, HOUSE_B, rule);
        break;
    case 1: //Rule 5
        return apply_rule(sol, BRITT, RED, NATIONALITY, NATIONALITY_B, COLOUR, COLOUR_B, rule);
        break;
    case 2: //Rule 6
        return apply_rule(sol, SWEDE, DOG, NATIONALITY, NATIONALITY_B, PET, PET_B, rule);
        break;
    case 3: //Rule 7
        return apply_rule(sol, DANE, TEA, NATIONALITY, NATIONALITY_B, BEVERAGE, BEVERAGE_B, rule);
        break;
    case 4: //Rule 8
        return apply_rule(sol, GERMAN, ROTHMANNS, NATIONALITY, NATIONALITY_B, CIGARETTE, CIGARETTE_B, rule);
        break;
    case 5: //Rule 9
        return apply_rule(sol, GREEN, COFFEE, COLOUR, COLOUR_B, BEVERAGE, BEVERAGE_B, rule);
        break;
    case 6: //Rule 10
        return apply_rule(sol, WINFIELD, BEER, CIGARETTE, CIGARETTE_B, BEVERAGE, BEVERAGE_B, rule);
        break;
    case 7: //Rule 13
        return apply_rule(sol, YELLOW, DUNHILL, COLOUR, COLOUR_B, CIGARETTE, CIGARETTE_B, rule);
        break;
    case 8: //Rule 14
        return apply_rule(sol, PALLMALL, BIRD, CIGARETTE, CIGARETTE_B, PET, PET_B, rule);
        break;
    case 9: //Rule 15
        return apply_rule(sol, MIDDLE, MILK, HOUSE, HOUSE_B, BEVERAGE, BEVERAGE_B, rule);
        break;
    case 10: //Rule 17
        return apply_rule_neighbour(sol, CAT, MARLBORO, PET, PET_B, CIGARETTE, CIGARETTE_B, NEIGHBOUR, rule);
        break;
    case 11: //Rule 12
        return apply_rule_neighbour(sol, BLUE, NORWEGIAN, COLOUR, COLOUR_B, NATIONALITY, NATIONALITY_B, NEIGHBOUR, rule);
        break;
    case 12: //Rule 18
        return apply_rule_neighbour(sol, WATER, MARLBORO, BEVERAGE, BEVERAGE_B, CIGARETTE, CIGARETTE_B, NEIGHBOUR, rule);
        break;
    case 13: //Rule 19
        return apply_rule_neighbour(sol, DUNHILL, HORSE, CIGARETTE, CIGARETTE_B, PET, PET_B, NEIGHBOUR, rule);
        break;
    case 14: //Rule 16
        return apply_rule_neighbour(sol, WHITE, GREEN, COLOUR, COLOUR_B, COLOUR, COLOUR_B, LEFT, rule);
        break;
    default:
        break;
    }
    return true;
}

int main() {

    #ifdef TRACE
        mtr_init("trace.json");
        mtr_start();
        MTR_META_PROCESS_NAME("MyProgram");
        MTR_META_THREAD_NAME("MainThread");

        MTR_BEGIN("main", "Initialization");
    #endif

    clock_t start = clock();
    
    struct Solution sol = {{0b100001000010000100001000010000,
                            0b010000100001000010000100001000,
                            0b001000010000100001000010000100,
                            0b000100001000010000100001000010,
                            0b000010000100001000010000100001}, {0}};
        
        
    bool state = assign(&sol, 0);

    clock_t end = clock();

    double elapsed = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    printf("Calculation time: %.6f milliseconds\n\n", elapsed);

    start = clock();
    
    if(state == true){
        printf("True\n\n");
    }else{
        printf("False\n\n");
    }
    
    print_sol(sol);
    
    end = clock();
    
    elapsed = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    printf("Print time: %.6f milliseconds\n\n", elapsed);
    
    #ifdef TRACE
    MTR_END("main", "Initialization");
    mtr_flush();
    mtr_shutdown();
    #endif

    return 0;
}