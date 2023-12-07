#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#define MAX_HANDS 1200
#define CARDS_PER_HAND 5

typedef struct hand {
    int cards[CARDS_PER_HAND];
    unsigned int val;
    int power;
} hand_t;

typedef struct set {
    hand_t hands[MAX_HANDS];
    int bids[MAX_HANDS];
    int n;
} set_t;

#endif