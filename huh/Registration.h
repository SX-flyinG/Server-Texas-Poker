class Registration {
    string username;
    string password;
    int age = 0;
    int choice = -1;

public:
    int reg(SOCKET clientSocket);
    int regIn(SOCKET clientSocket);
    int logIn(SOCKET clientSocket);
};

/* Handle the main registration or login process */
int Registration::reg(SOCKET clientSocket) {
    string menu = "Choose an action:\n0 - Exit\n1 - Register\n2 - Log In\n";
    send(clientSocket, menu.c_str(), menu.size(), 0);

    char buffer[2];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error: Failed to receive client input" << endl;
        return -1;
    }

    choice = buffer[0] - '0';

    switch (choice) {
    case 0:
        cout << "Client exited." << endl;
    case 1:
        return regIn(clientSocket);
    case 2:
        return logIn(clientSocket);
    default:
        cerr << "Invalid choice received from client." << endl;
    }
}

/* Handle user registration */
int Registration::regIn(SOCKET clientSocket) {
    string prompt = "Enter your username (max 16 characters, no spaces):\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);

    char buffer[16];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error: Failed to receive username" << endl;
        return -1;
    }
    buffer[bytesReceived] = '\0';
    username = buffer;

    prompt = "Enter your year of birth:\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);

    char yearBuffer[5];
    bytesReceived = recv(clientSocket, yearBuffer, sizeof(yearBuffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error: Failed to receive year of birth" << endl;
        return -1;
    }
    yearBuffer[bytesReceived] = '\0';
    age = stoi(yearBuffer);

    if (age < 1900 || age > 2005) {
        string error = "Invalid age. Goodbye!\n";
        send(clientSocket, error.c_str(), error.size(), 0);
        return -1;
    }

    prompt = "Enter your password (max 64 characters, no spaces):\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);

    char passBuffer[64];
    bytesReceived = recv(clientSocket, passBuffer, sizeof(passBuffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error: Failed to receive password" << endl;
        return -1;
    }
    passBuffer[bytesReceived] = '\0';
    password = passBuffer;

    // Save to file
    ofstream userFile("user_data.txt");
    userFile << username << "\n" << password << "\n" << age;
    userFile.close();

    string success = "Registration successful!\n";
    send(clientSocket, success.c_str(), success.size(), 0);
    return 0;
}

/* Handle user login */
int Registration::logIn(SOCKET clientSocket) {
    string prompt = "Enter your username:\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);

    char buffer[16];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error: Failed to receive username" << endl;
        return -1;
    }
    buffer[bytesReceived] = '\0';
    string enteredUsername = buffer;

    prompt = "Enter your password:\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);

    char passBuffer[64];
    bytesReceived = recv(clientSocket, passBuffer, sizeof(passBuffer) - 1, 0);
    if (bytesReceived <= 0) {
        cerr << "Error: Failed to receive password" << endl;
        return -1;
    }
    passBuffer[bytesReceived] = '\0';
    string enteredPassword = passBuffer;

    // Verify credentials
    ifstream userFile("user_data.txt");
    if (!userFile) {
        string error = "No registered users. Please register first.\n";
        send(clientSocket, error.c_str(), error.size(), 0);
        return -1;
    }

    string storedUsername, storedPassword;
    int storedAge;
    userFile >> storedUsername >> storedPassword >> storedAge;
    userFile.close();

    if (enteredUsername == storedUsername && enteredPassword == storedPassword) {
        string success = "Login successful! Welcome back.\n";
        send(clientSocket, success.c_str(), success.size(), 0);
    }
    else {
        string error = "Invalid credentials. Try again.\n";
        send(clientSocket, error.c_str(), error.size(), 0);
    }
    return 0;
}