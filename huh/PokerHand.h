class PokerHand {
private:
    vector<string> hand;

    // Helper to extract rank and suit from a card string
    pair<string, string> GetCardDetails(const string& card) {
        size_t pos = card.find(" of ");
        string rank = card.substr(0, pos);
        string suit = card.substr(pos + 4);
        return {rank, suit};
    }

    // Helper to get rank value
    int GetRankValue(const string& rank) {
        map<string, int> rankMap = {
            {"Two", 2}, {"Three", 3}, {"Four", 4}, {"Five", 5}, {"Six", 6}, {"Seven", 7},
            {"Eight", 8}, {"Nine", 9}, {"Ten", 10}, {"Jack", 11}, {"Queen", 12}, {"King", 13}, {"Ace", 14}
        };
        return rankMap[rank];
    }

public:
    PokerHand(const vector<string>& cards) : hand(cards) {}

    string EvaluateHand() {
        map<string, int> rankCount;
        map<string, int> suitCount;
        vector<int> ranks;

        for (const auto& card : hand) {
            auto [rank, suit] = GetCardDetails(card);
            rankCount[rank]++;
            suitCount[suit]++;
            ranks.push_back(GetRankValue(rank));
        }

        sort(ranks.begin(), ranks.end());

        // Check for flush
        bool isFlush = any_of(suitCount.begin(), suitCount.end(), [](auto& pair) { return pair.second >= 5; });

        // Check for straight
        bool isStraight = false;
        int consecutive = 1;
        for (size_t i = 1; i < ranks.size(); ++i) {
            if (ranks[i] == ranks[i - 1] + 1) {
                consecutive++;
                if (consecutive == 5) {
                    isStraight = true;
                    break;
                }
            } else if (ranks[i] != ranks[i - 1]) {
                consecutive = 1;
            }
        }

        // Check for four of a kind, full house, three of a kind, two pair, pair
        int fourOfAKind = 0, threeOfAKind = 0, pairs = 0;

        for (auto it = rankCount.begin(); it != rankCount.end(); ++it) {
            const string& rank = it->first;  // Ключ (ранг карты)
            int count = it->second;         // Значение (количество карт данного ранга)

            if (count == 4) {
                fourOfAKind++;
            }
            if (count == 3) {
                threeOfAKind++;
            }
            if (count == 2) {
                pairs++;
            }
        }


        if (isFlush && isStraight && ranks.back() == 14) {
            return "Royal Flush";
        }
        if (isFlush && isStraight) {
            return "Straight Flush";
        }    
        if (fourOfAKind) {
            return "Four of a Kind";
        }
        if (threeOfAKind && pairs) {
            return "Full House";
        }        
        if (isFlush) {
            return "Flush";
        }
        if (isStraight) {
            return "Straight";
        }
        if (threeOfAKind) {
            return "Three of a Kind";
        }
        if (pairs == 2) {
            return "Two Pair";
        }
        if (pairs == 1) {
            return "One Pair";
        }

        return "High Card";
    }
};


    PokerHand handEvaluator(playerHand);

    return 0;
}
