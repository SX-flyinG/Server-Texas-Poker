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

	// Раздача карт игрокам
	void DealInitialCards() {
		for (int i = 0; i < 5; ++i) {  // Раздаём по 5 карт каждому игроку
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
	PokerGame(int playersCount) : numPlayers(playersCount) {
		players.resize(numPlayers);
		playerChips.resize(numPlayers, 100);	
	}

	// Запуск игры
	void StartGame(SOCKET clientSocket) {
		string continueGame = "yes";
		while (continueGame == "yes") {
			deck.ShuffleDeck();         // Перемешивание колоды
			DealInitialCards();         // Раздача карт
			ShowAllHands(clientSocket); // Показать руки игроков
			PlayerBets(clientSocket);   // Ставки игроков
			DetermineWinner(clientSocket); // Определить победителя

			// Очистить руки игроков для следующего раунда
			for (int i = 0; i < numPlayers; ++i) {
				players[i].clear();
			}

			// Спросить клиента, хочет ли он продолжить
			string message = "Do you want to play another round? (yes/no)\n";
			send(clientSocket, message.c_str(), message.size(), 0);

			char buffer[16] = { 0 };
			int received = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
			if (received > 0) {
				buffer[received] = '\0';
				continueGame = string(buffer);
			}
			else {
				cerr << "Error: Failed to receive response from client.\n";
				break;
			}
		}
	}
};
