class Lobby {
    int getKey;

public:
    Lobby() {
        getKey = 27; // Default exit key
    }

    // Main lobby menu
    void MainLobby(SOCKET clientSocket) {
        while(true)
        {
            string menu =
                "\t\t\t####  #       #####  #####   #  #  \n"
                "\t\t\t#  #  #       #   #  #       # #   \n"
                "\t\t\t####  #       #####  #       ##    \n"
                "\t\t\t#  #  #       #   #  #       # #   \n"
                "\t\t\t####  ######  #   #  #####   #  #  \n"
                "\n\n"
                "\t\t\t     #  #####  #####   #  #  \n"
                "\t\t\t     #  #   #  #       # #   \n"
                "\t\t\t     #  #####  #       ##    \n"
                "\t\t\t     #  #   #  #       # #   \n"
                "\t\t\t######  #   #  #####   #  #  \n"
                "\n1 - Start Game\n"
                "2 - Game Mode\n"
                "3 - Rules\n"
                "0,4 - Exit\n"
                "Enter your choice: ";

            send(clientSocket, menu.c_str(), menu.size(), 0);

            char choice;
            recv(clientSocket, &choice, 1, 0);

            switch (choice) {
            case '0':
            case '4':
                send(clientSocket, "Exiting lobby. Goodbye!\n", 24, 0);
                break;
            case '1':
                send(clientSocket, "Starting the game. Get ready!\n", 30, 0);
                break;
            case '2':
                send(clientSocket, "Game Mode is under development.\n", 33, 0);
                break;
            case '3':
                Rules(clientSocket);
                break;
            default:
                send(clientSocket, "Invalid input. Try again.\n", 26, 0);
                MainLobby(clientSocket);
                break;
            }
        }
    }

    // Display rules menu
    void Rules(SOCKET clientSocket) {
        string rulesMenu =
            "Choose rules to read:\n"
            "0 - Exit\n"
            "1 - Texas Hold'em\n"
            "2 - Default Poker\n";
        send(clientSocket, rulesMenu.c_str(), rulesMenu.size(), 0);

        char choice;
        recv(clientSocket, &choice, 1, 0);

        switch (choice) {
        case '0':
            MainLobby(clientSocket);
            break;
        case '1':
            PrintTexasHoldemRules(clientSocket);
            break;
        case '2':
            send(clientSocket, "Default Poker rules coming soon!\n", 33, 0);
            MainLobby(clientSocket);
            break;
        default:
            send(clientSocket, "Invalid choice. Returning to main menu.\n", 40, 0);
            MainLobby(clientSocket);
            break;
        }
    }

    // Print Texas Hold'em rules
    void PrintTexasHoldemRules(SOCKET clientSocket) {
        string texasRules =
            "----------------------------------------\n"
            "         Texas Hold'em Poker Rules       \n"
            "----------------------------------------\n"
            "1. Each player is dealt two private cards (hole cards).\n"
            "2. Five community cards are dealt face-up on the 'board'.\n"
            "3. Players combine their two hole cards with the community cards\n"
            "   to make the best possible five-card hand.\n"
            "4. The game has four betting rounds:\n"
            "   - Pre-Flop: After players receive hole cards.\n"
            "   - Flop: After the first three community cards are revealed.\n"
            "   - Turn: After the fourth community card is revealed.\n"
            "   - River: After the fifth and final community card is revealed.\n"
            "5. The player with the best five-card hand wins the pot.\n"
            "6. Hand Rankings (from best to worst):\n"
            "   - Royal Flush, Straight Flush, Four of a Kind,\n"
            "     Full House, Flush, Straight, Three of a Kind,\n"
            "     Two Pair, One Pair, High Card.\n"
            "7. If all players but one fold, the last player standing wins the pot.\n"
            "----------------------------------------\n";

        send(clientSocket, texasRules.c_str(), texasRules.size(), 0);
        MainLobby(clientSocket);
    }
};
