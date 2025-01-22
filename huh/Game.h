class Player {
private:
    string name;  // Имя игрока
    int money;    // Количество денег у игрока
    vector<Card> hand;  // Рука игрока (карты)

public:
    // Конструктор игрока
    Player(string n) {
        name = n;
        money = 1000;
    }

    // Добавление карты в руку игрока
    void addCardToHand(const Card& card) {
        hand.push_back(card);
    }

    // Сбрасывание карт руки игрока
    void clearHand() {
        hand.clear();
    }

    // Получение имени игрока
    string getName() const {
        return name;
    }

    // Получение денег игрока
    int getMoney() const {
        return money;
    }

    // Уменьшение денег игрока (ставка)
    void betMoney(int amount) {
        money -= amount;
    }

    // Получение карт игрока
    vector<Card>& getHand() {
        return hand;
    }
};

class PokerGame {
private:
    vector<Player> players;  // Игроки
    Deck deck;               // Колода
    int pot;                 // Пот (банк)
    
public:
    // Конструктор игры
    PokerGame(SOCKET clientSocket) {
        pot = 0;    
    }
            
    // Добавление игрока в игру
    void addPlayer(const Player& player) {
        players.push_back(player);
    }

    // Раздача карт игрокам
    void dealCards() {
        for (int i = 0; i < players.size(); i++) {
            players[i].addCardToHand(deck.dealCard());
            players[i].addCardToHand(deck.dealCard());
        }
    }

    // Ставка игрока
    void playerBet(int playerIndex, int amount) {
        if (amount > 0) {
            players[playerIndex].betMoney(amount);
            pot += amount;
        }
    }

    // Печать состояния игры
    void printGameState() {
        for (int i = 0; i < players.size(); i++) {
            string stats = players[i].getName() + " has $" + players[i].getMoney() + " and holds ";
            send(clientSocket , stats , sizeof(stats) , 0) ;
            for (int j = 0; j < players[i].getHand().size(); j++) {
                string hand =  players[i].getHand()[j].getCard()  +  " \n";
                send(clientSocket , hand , sizeof(hand) , 0) ;
            }
        }
        string bankPot =  "Current pot: $" + pot + "\n";
        send(clientSocket , bankPot , sizeof(bankPot) , 0) ;
    }

    // Начало игры
    void startGame() {
        // Раздача карт
        dealCards();
        printGameState();
    }
};
