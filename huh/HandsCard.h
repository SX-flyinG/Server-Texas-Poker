class PokerHand {
private:
    vector<string> hand;

    // Карты в покере по порядку
    const vector<string> allRanks = {
        "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
        "Nine", "Ten", "Jack", "Queen", "King", "Ace"
    };

    const vector<string> allSuits = { "Peaks", "Worms", "Diamonds", "Cross" };

    // Получение числового значения ранга из строки карты
    int GetRankValueFromCard(const string& card) {
        for (size_t i = 0; i < allRanks.size(); ++i) {
            if (card.find(allRanks[i]) != string::npos) {
                return i + 2; // Ранги начинаются с 2
            }
        }
        return 0; // Если карта некорректная
    }

    // Получение масти из строки карты
    string GetSuitFromCard(const string& card) {
        for (const auto& suit : allSuits) {
            if (card.find(suit) != string::npos) {
                return suit;
            }
        }
        return ""; // Если масть некорректная
    }

    // Проверка на наличие стрита
    bool IsStraight(vector<int>& ranks) {
        sort(ranks.begin(), ranks.end());
        int consecutive = 1;
        for (size_t i = 1; i < ranks.size(); ++i) {
            if (ranks[i] == ranks[i - 1] + 1) {
                consecutive++;
                if (consecutive == 5) return true;
            }
            else if (ranks[i] != ranks[i - 1]) {
                consecutive = 1;
            }
        }
        // Проверка на стрит с тузом (A, 2, 3, 4, 5)
        if (ranks.back() == 14) { // Если есть туз
            ranks.insert(ranks.begin(), 1); // Добавляем туз как 1
            return IsStraight(ranks);
        }
        return false;
    }

    // Проверка на наличие флеша
    bool IsFlush(const map<string, int>& suitCount) {
        for (const auto& pair : suitCount) {
            if (pair.second >= 5) return true;
        }
        return false;
    }

public:
    PokerHand(const vector<string>& cards) : hand(cards) {}

    string EvaluateHand() {
        map<string, int> rankCount; // Количество каждой ранга
        map<string, int> suitCount; // Количество каждой масти
        vector<int> ranks;          // Числовые значения рангов

        for (const auto& card : hand) {
            int rankValue = GetRankValueFromCard(card);
            string suit = GetSuitFromCard(card);

            if (rankValue > 0) {
                rankCount[card]++;
                ranks.push_back(rankValue);
            }

            if (!suit.empty()) {
                suitCount[suit]++;
            }
        }

        bool flush = IsFlush(suitCount);
        bool straight = IsStraight(ranks);

        // Проверка на Royal Flush
        if (flush && straight && *max_element(ranks.begin(), ranks.end()) == 14) {
            return "Royal Flush";
        }

        // Проверка на Straight Flush
        if (flush && straight) {
            return "Straight Flush";
        }

        // Проверка на Four of a Kind, Full House и т.д.
        int fourOfAKind = 0, threeOfAKind = 0, pairs = 0;

        for (const auto& pair : rankCount) {
            if (pair.second == 4) fourOfAKind++;
            if (pair.second == 3) threeOfAKind++;
            if (pair.second == 2) pairs++;
        }

        if (fourOfAKind) return "Four of a Kind";
        if (threeOfAKind && pairs) return "Full House";
        if (flush) return "Flush";
        if (straight) return "Straight";
        if (threeOfAKind) return "Three of a Kind";
        if (pairs == 2) return "Two Pair";
        if (pairs == 1) return "One Pair";

        return "High Card";
    }
};
