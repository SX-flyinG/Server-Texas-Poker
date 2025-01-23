#pragma once

#include <iostream>
#include <cstdlib>


#pragma comment(lib, "ws2_32.lib")

using namespace std;


class CardDeck {
    Card card;
    int currentCardIndex;

public:
    CardDeck() {
        card = Card();
        currentCardIndex = 0;
    }

    void ShuffleDeck() {

        for (int i = 51; i > 0; --i) {
            int j = rand() % (i + 1);
            swap(card.Cards[i], card.Cards[j]);
        }

        currentCardIndex = 0;
    }

    string DealCard() {
        if (currentCardIndex >= 52) {
            return "No cards left in the deck!";
        }

        return card.Cards[currentCardIndex++];
    }

};