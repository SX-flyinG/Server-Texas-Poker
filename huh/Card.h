#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

class Card {
public:
    string Cards[52]; 

    Card() {
        string ranks[] = { "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
                          "Nine", "Ten", "Jack", "Queen", "King", "Ace" };
        string suits[] = { "Peaks", "Worms", "Diamonds", "Cross" };
        int index = 0;

        for (int suit = 0; suit < 4; suit++) {  
            for (int rank = 0; rank < 13; rank++) {  
                Cards[index++] = ranks[rank] + " of " + suits[suit];
            }
        }
    }

};
