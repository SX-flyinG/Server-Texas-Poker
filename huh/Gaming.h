#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <conio.h>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <random>
#include <set>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#include "Card.h"
#include "Deck.h"

class PokerGame {
private:
    CardDeck deck;                      // Колода карт
    vector<vector<string>> players;     // Руки игроков
    vector<int> playerChips;            // Количество фишек у каждого игрока
    int numPlayers;                     // Количество игроков
    int currentRound;                   // Номер текущего раунда

    // Раздача карт игрокам
    void DealInitialCards() {
        for (int i = 0; i < 2; ++i) {  // Раздаём по 2 карты каждому игроку
            for (int j = 0; j < numPlayers; ++j) {
                players[j].push_back(deck.DealCard());
            }
        }
    }

    // Показать руки игроков
    void ShowAllHands(SOCKET clientSocket) const {
        for (int i = 0; i < numPlayers; ++i) {
            string message = "Player " + to_string(i + 1) + "'s hand: ";
            send(clientSocket, message.c_str(), message.size(), 0);
            for (const auto& card : players[i]) {
                message = card + " | ";
                send(clientSocket, message.c_str(), message.size(), 0);
            }
            message = "\n";
            send(clientSocket, message.c_str(), message.size(), 0);
        }
    }

    // Ставки игрока
    void PlayerBets(SOCKET clientSocket) {
        for (int i = 0; i < numPlayers; ++i) {
            string message = "Player " + to_string(i + 1) + ", your chips: " + to_string(playerChips[i]) + "\nPlace your bet (or 0 to fold): ";
            send(clientSocket, message.c_str(), message.size(), 0);

            int bet = 0;
            recv(clientSocket, reinterpret_cast<char*>(&bet), sizeof(bet), 0);

            if (bet == 0) {
                playerChips[i] = 0;  // Игрок сдался
                players[i].clear();   // Убираем его карты
                send(clientSocket, "You folded\n", 12, 0);
            }
            else {
                playerChips[i] -= bet;  // Вычитаем ставку из фишек
                send(clientSocket, ("You bet: " + to_string(bet) + "\n").c_str(), 20, 0);
            }
        }
    }

    // Определение победителя
    void DetermineWinner(SOCKET clientSocket) {
        string bestHandType = "High Card";
        int winnerIndex = 0;

        for (int i = 0; i < numPlayers; ++i) {
            PokerHand handEvaluator(players[i]);
            string handType = handEvaluator.EvaluateHand();
            string message = "Player " + to_string(i + 1) + "'s hand type: " + handType + "\n";
            send(clientSocket, message.c_str(), message.size(), 0);

            if (CompareHands(handType, bestHandType)) {
                bestHandType = handType;
                winnerIndex = i;
            }
        }

        string message = "The winner is Player " + to_string(winnerIndex + 1) + " with a " + bestHandType + "!\n";
        send(clientSocket, message.c_str(), message.size(), 0);

        // Победитель забирает фишки
        playerChips[winnerIndex] += 100;  // Пример: Победитель получает 100 фишек
    }

    // Сравнение комбинаций
    bool CompareHands(const string& hand1, const string& hand2) {
        map<string, int> handRanks = {
            {"High Card", 1}, {"One Pair", 2}, {"Two Pair", 3},
            {"Three of a Kind", 4}, {"Straight", 5}, {"Flush", 6},
            {"Full House", 7}, {"Four of a Kind", 8},
            {"Straight Flush", 9}, {"Royal Flush", 10}
        };
        return handRanks[hand1] > handRanks[hand2];
    }

public:
    PokerGame(int playersCount) : numPlayers(playersCount), currentRound(1) {
        players.resize(numPlayers);
        playerChips.resize(numPlayers, 100); // Начальные фишки для каждого игрока
        srand(static_cast<unsigned>(time(0))); // Для перемешивания
    }

    // Запуск игры
    void StartGame(SOCKET clientSocket) {
        while (true) {
            // Перемешивание колоды и начало нового раунда
            deck.ShuffleDeck();
            DealInitialCards();
            ShowAllHands(clientSocket);
            PlayerBets(clientSocket);  // Игроки делают ставки

            DetermineWinner(clientSocket);

            // Переход к следующему раунду
            currentRound++;

            // Прекращение игры или продолжение
            char choice;
            int bytesReceived = recv(clientSocket, &choice, 1, 0);
            if (bytesReceived <= 0) {
                cerr << "Client disconnected or error receiving data" << endl;
                closesocket(clientSocket);
                return;
            }

            switch (choice) {
            case 27:  // Игрок может выйти
                send(clientSocket, "Game over\n", 10, 0);
                return;
            default:
                send(clientSocket, "Next round starting...\n", 24, 0);
                break;
            }
        }
    }
};
