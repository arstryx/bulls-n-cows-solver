// Bulls and Cows Solution Minimax Algortihm
// Created by Arsenii Zakharenko


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LEN 4 // Basic game is played with 4 digit numbers
#define MAX_NUMS 5040 // Amount of numbers with unique digits for given NUM_LEN


// Each number is represented as a string
// Algorithm will work with their arrays (sets)
typedef struct {
    char numbers[MAX_NUMS][NUM_LEN + 1];
    int count;
} num_set;

// Guess result - how many bulls and cows
typedef struct 
{
    int bulls, cows;
} guess_res;



// Used for some calculations
int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}


// Checks if given number consists only of unique digits
int is_unique(char* num){
    for (int i = 0; i < NUM_LEN; i++){
        for (int j = i + 1; j < NUM_LEN; j++){
            if (num[i] == num[j] && i != j){
                return 0;
            }
        }
    }
    return 1;
}




// Generates set of all possible numbers with unique digits
num_set all_unique_nums() {
    num_set set;
    set.count = MAX_NUMS;
    int index = 0;
    const char digits[] = "0123456789";
    char num[NUM_LEN + 1];  
    num[NUM_LEN] = '\0';  

    for (long i = 0; i < power(10, NUM_LEN); i++) {
        for (int j = 0; j < NUM_LEN; j++) {
            num[j] = '0' + (i / power(10, NUM_LEN - 1 - j)) % 10;
        }
        if (is_unique(num)){
            strcpy(set.numbers[index++], num);
        }
    }
    return set;
}


// Compares "secret" number and guess and returns attempt result
guess_res res(const char* secret, const char* guess){
    guess_res res = {0, 0};
    for (int i = 0; i < NUM_LEN; i++) {
        for (int j = 0; j < NUM_LEN; j++)
        {
            if (secret[i] == guess[j]) {
                if (i == j) {
                    res.bulls++;
                }
                else {
                    res.cows++;
                }
            }
        }
    }
    return res;
}



// Minimax strategy
// TODO detailed comments
char* best_num(num_set set){
    char* best_num = NULL;
    int min_largest_group = MAX_NUMS;
    for (int i = 0; i < set.count; i++) {
        char *candidate = set.numbers[i];
        int groups[NUM_LEN + 1][NUM_LEN + 1] = {0};
        for (int j = 0; j < set.count; j++) {
            if (i == j) continue;
            guess_res result = res(candidate, set.numbers[j]);
            groups[result.bulls][result.cows]++;       
        }
        int largest_group_size = 0;
        for (int b = 0; b <= NUM_LEN; b++) {
            for (int c = 0; c <= NUM_LEN; c++) {
                if (groups[b][c] > largest_group_size) {
                    largest_group_size = groups[b][c];
                }
            }
        }
        if (largest_group_size < min_largest_group) {
            min_largest_group = largest_group_size;
            best_num = candidate;
        }
    }
    return best_num;
}


int main(){
    // Starting with all possible numbers
    num_set set = all_unique_nums();
    int attempts = 0;
    while (set.count != 0) {
        printf ("Possible numbers left: %d\n", set.count);
        
        // Try to guess number using minimax algorithm
        // More simple version -> just take random
        char *guess = best_num(set);
        printf("Attmept %d: %s\n", ++attempts, guess);
        
        // Get feedback and check if finished
        guess_res result;
        printf("How many bulls: ");
        scanf("%d", &result.bulls); 
        printf("How many cows: ");
        scanf("%d", &result.cows);
        if (result.bulls == 4) {
            printf("Number guessed in %d attempts!", attempts);
            break;
        }

        // Delete numbers that are obviously not right
        num_set new_set;
        new_set.count = 0;
        for (int i = 0; i < set.count; i++)
        {
            guess_res new_res = res(set.numbers[i], guess);
            if (new_res.bulls == result.bulls && new_res.cows == result.cows) {
                strcpy(new_set.numbers[new_set.count++], set.numbers[i]);
            }
        }
        set = new_set;    
        printf("*************************************************\n");    
    }



}