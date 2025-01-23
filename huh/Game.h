class PokerGame {
private:
    CardDeck deck;                       // Колода карт
    vector<vector<string>> players;      // Руки игроков
    int numPlayers;                      // Количество игроков
    string message

    // Функция для раздачи карт игрокам
    void DealInitialCards() {
        for (int i = 0; i < 2; ++i) { // Раздаём по 2 карты каждому игроку
            for (int j = 0; j < numPlayers; ++j) {
                players[j].push_back(deck.DealCard());
            }
        }
    }

    // Отображение рук игроков
    void ShowAllHands(SOCKET clientSocket) const {
        for (int i = 0; i < numPlayers; ++i) {
            message =  "Player " + i + 1 + "'s hand: ";
            send(clientSocket , message , sizeof(message) , 0);
            for (const auto& card : players[i]) {
                mesasge =  card + " | ";
                send(clientSocket , message , sizeof(message) , 0);
            }
            message =  "\n;
            send(clientSocket , message , sizeof(message) , 0);
        }
    }

    // Выбор победителя
    void DetermineWinner(SOCKET clientSocket) {
        string bestHandType = "High Card";
        int winnerIndex = 0;

        for (int i = 0; i < numPlayers; ++i) {
            PokerHand handEvaluator(players[i]);
            string handType = handEvaluator.EvaluateHand();
            message =  "Player " + (i + 1) + "'s hand type: " + handType + "\n";
            send(clientSocket , message , sizeof(message) , 0);
            // Сравниваем силу комбинации
            if (CompareHands(handType, bestHandType)) {
                bestHandType = handType;
                winnerIndex = i;
            }
        }

        message =  "The winner is Player " + (winnerIndex + 1) + " with a " + bestHandType + "!\n";
        send(clientSocket , message , sizeof(message) , 0);
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
        srand(static_cast<unsigned>(time(0))); // Для перемешивания
    }

    // Запуск игры
    void StartGame() {
        deck.ShuffleDeck();
        DealInitialCards();
        ShowAllHands();
        DetermineWinner();
    }
};
