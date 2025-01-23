#include "Gaming.h"

class Lobby {
public:
    void MainLobby(SOCKET clientSocket) {
        int numPlayers = 1;
        PokerGame pokerGame(numPlayers);
        char choice;
        while (true) {
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

            int bytesReceived = recv(clientSocket, &choice, 1, 0);
            if (bytesReceived <= 0) {
                cerr << "Client disconnected or error receiving data" << endl;
                closesocket(clientSocket);
                return;
            }


            switch (choice) {
            case '0':
            case '4':
                send(clientSocket, "Exiting lobby. Goodbye!\n", 24, 0);
                return;
            case '1':
                send(clientSocket, "Starting the game. Get ready!\n", 30, 0);
                pokerGame.StartGame(clientSocket);
                break;
            case '2':
                send(clientSocket, "Game Mode is under development.\n", 33, 0);
                break;
            case '3':
                Rules(clientSocket);
                break;
            default:
                send(clientSocket, "Invalid input. Try again.\n", 26, 0);
                break;
            }
        }
    }

    void Rules(SOCKET clientSocket) {
        string rules =
            "Game Rules:\n"
            "1. Play fair.\n"
            "2. Respect other players.\n"
            "3. Have fun!\n"
            "Press any key to return to the lobby.\n";

        send(clientSocket, rules.c_str(), rules.size(), 0);

        char choice;
        recv(clientSocket, &choice, 1, 0);
    }
};
