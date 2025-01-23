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
#include "HandsCard.h"
#include "Registration.h"
#include "Lobby.h"
#include "PokerGame.h"


int main() {
    const int SERVER_PORT = 55805;
    Registration registration;
    Lobby lobby;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock" << endl;
        return 1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error: Failed to create socket" << endl;
        WSACleanup();
        return 1;
    }

    // Configure server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error: Failed to bind socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Start listening for connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error: Failed to listen on socket" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server running on port " << SERVER_PORT << endl;

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Accept a client connection
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error: Failed to accept client connection" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Client connected" << endl;

    // Handle client registration and logiт
    registration.reg(clientSocket);
        
    lobby.MainLobby(clientSocket);

    // Start Poker Game
    // Assuming a game with 1 players for simplicity
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
