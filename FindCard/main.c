//
//  main.c
//  FindCard
//
//  Created by JianZhang on 2017/3/2.
//  Copyright © 2017年 NUT. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

typedef enum Suits {
    Spades = 0,
    Hearts,
    Clubs,
    Diamonds,
} Suits;

typedef struct Card
{
    Suits suit;
    int value;
} Card;

Card * CreatePackOfCards(void);
Card RandomDrawCardFromCards(Card *cards, int count);
Card FindDrewCardFromCards_bucket(Card *cards, int count);
Card FindDrewCardFromCards_calculate(Card *cards, int count);

void RandomPackOfCards(Card *cards, int count);
void PrintCard(Card card);
void PrintCards(const Card *cards, int count);

int main(int argc, const char *argv[])
{
    Card *cards, drewCard, foundCard;
    clock_t start, end;
    
    cards = CreatePackOfCards();
    
    RandomPackOfCards(cards, 53);
    
    drewCard = RandomDrawCardFromCards(cards, 54);
    printf("Drew Card:\n\t");
    PrintCard(drewCard);
    putchar('\n');
    
    start = clock();
    foundCard = FindDrewCardFromCards_bucket(cards, 53);
    end = clock();
    printf("Bucket Card duration: %ld\n\t", end - start);
    PrintCard(foundCard);
    putchar('\n');
    
    start = clock();
    foundCard = FindDrewCardFromCards_calculate(cards, 53);
    end = clock();
    printf("Calculate Card duration: %ld\n\t", end - start);
    PrintCard(foundCard);
    putchar('\n');
    
    PrintCards(cards, 53);
    free(cards);
    putchar('\n');
    return 0;
}

Card * CreatePackOfCards(void)
{
    Card *cards, *p;
    int index;
    Suits suit;
    
    cards = malloc(sizeof(Card) * 54);
    assert(cards != NULL);
    
    p = cards;
    
    for (index = 0; index < 13; index++) {
        for (suit = Spades; suit <= Diamonds; suit++) {
            p->suit = suit;
            p->value = index + 1;
            p++;
        }
    }
    
    (cards + 52)->suit = 0;
    (cards + 52)->value = 14;
    
    (cards + 53)->suit = 0;
    (cards + 53)->value = 15;
    
    return cards;
}

void RandomPackOfCards(Card *cards, int count)
{
    int index, randomIndex;
    Card tempCell;
    
    for (index = 0; index < count; index++) {
        randomIndex = arc4random() % 54;
        tempCell = cards[randomIndex];
        cards[randomIndex] = cards[index];
        cards[index] = tempCell;
    }
}

Card RandomDrawCardFromCards(Card *cards, int count)
{
    Card drewCard;
    int index, randomIndex;
    
    randomIndex = arc4random() % 54;
    drewCard.suit = cards[randomIndex].suit;
    drewCard.value = cards[randomIndex].value;
    
    for (index = randomIndex; index < 53; index++)
        cards[index] = cards[index + 1];
    
    cards[53].suit = 0;
    cards[53].value = 0;
    return drewCard;
}

Card FindDrewCardFromCards_bucket(Card *cards, int count)
{
    Card bucket[15][4] = {0}, drewCard;
    int index, jndex;
    
    drewCard.value = 0;
    
    for (index = 0; index < count; index++) {
        bucket[cards[index].value - 1][cards[index].suit].suit = cards[index].suit;
        bucket[cards[index].value - 1][cards[index].suit].value = cards[index].value;
    }
    
    for (index = 0; index < 13; index++) {
        for (jndex = 0; jndex < 4; jndex++) {
            if (bucket[index][jndex].value == 0) {
                drewCard.value = index + 1;
                drewCard.suit = jndex;
                break;
            }
        }
    }
    
    if (drewCard.value != 0)
        return drewCard;
    
    for (index = 13; index < 15; index++) {
        if (bucket[index][0].value == 0) {
            drewCard.value = index + 1;
            break;
        }
    }
    
    return drewCard;
}

void PrintCard(Card card)
{
    printf("Suit: %d, Value: %d\n", card.suit, card.value);
}

void PrintCards(const Card *cards, int count)
{
    int index;
    
    for (index = 0; index < count; index++)
        PrintCard(cards[index]);
}

Card FindDrewCardFromCards_calculate(Card *cards, int count)
{
    Card drewCard;
    int index, sum;
    
    sum = 0;
    for (index = 0; index < count; index++) {
        sum += cards[index].value * 10 + cards[index].suit;
    }
    
    drewCard.suit = (4008 - sum) % 10;
    drewCard.value = (4008 - sum) / 10;
    
    return drewCard;
}
