#include <iostream>
#include "User.h"
#include "fstream"
#include "vector"
#include "algorithm"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

std::vector <User> getUsersList();
User parseString(std::string line);
bool userExistence(int id, std::string username, User *user);
void printCommands();
std::vector<User> showFriendList(int currentId);
void printAdminCommands();



void server() {
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "172.16.88.95", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }

    // Close listening socket
    close(listening);

    // While loop: accept and echo message back to client
    char buf[4096];

    while (true)
    {
        memset(buf, 0, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            std::cerr << "Error in recv(). Quitting" << "\n";
            break;
        }

        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected " << "\n";
            break;
        }

        std::string line;

        line = std::string(buf, 0, bytesReceived);
        std::cout << line << std::endl;
        std::string command = "", buffer = "", operation;
        int count = 0, currentId, anotherId;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '-') {
                count++;
                if (count == 1) command = buffer;
                if (count == 2) operation = buffer;
                if (count == 3) currentId = std::stoi(buffer);
                buffer = "";
            } else buffer += line[i];
            if (i == line.size() - 1) {
                anotherId = std::stoi(buffer);
            }
        }
        std::vector <User> usersList = getUsersList();
        User currentUser, anotherUser;
        if (command == "user") {
            currentUser = parseString(operation);
            currentUser.addUser();
        } else {
            for (auto user: usersList) {
                if (user.getId() == currentId) currentUser = user;
                if (user.getId() == anotherId) anotherUser = user;
            }
            if (command == "friend") {
                if (operation == "add") currentUser.addFriend(anotherUser.getUsername());
                if (operation == "remove") currentUser.removeFriend(anotherUser);
            }

            if (command == "message") {
                std::string mess = operation;
                currentUser.writeMessage(anotherUser, mess);
            }
        }
        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);
    }

    // Close the socket
    close(clientSocket);
}

void client() {
    bool isAdmin = false;

    std::cout << "Hi. Login or Sign up? (l/s)\n";
    User currentUser;
    std::vector <User> allUsersList = getUsersList();
    char inputLS;
    std::cin >> inputLS;
    if (inputLS == 'l' || inputLS == 'L') {
        std::cout << "Enter your id and username in format: 0 nickname\n";
        int inputId;
        std::string inputUsername;
        std::cin >> inputId >> inputUsername;
        if (inputId == 0 && inputUsername == "admin") isAdmin = true;
        if (userExistence(inputId, inputUsername, &currentUser)) {
            std::cout << "Login completed!\n";
            std::cout << currentUser.getId() << " " << currentUser.getUsername() << " " << currentUser.getFriends() << " " << currentUser.getMessages() << " " << currentUser.getRegistrationDate() << std::endl;

        } else std::cout << "User doesn't exist\n";
    }
    if (inputLS == 's' || inputLS == 'S') {
        std::cout << "Enter your username\n";
        std::string inputUsername;
        std::cin >> inputUsername;
        currentUser = User(inputUsername);
        int maxId = 0;
        bool usernameMatched = false;
        for (auto user: allUsersList) {
            maxId = std::max(maxId, user.getId());
            if (inputUsername == user.getUsername()) usernameMatched = true;
        }
        if (!usernameMatched) {
            currentUser.setId(maxId + 1);
            std::cout << "remember your id: " << currentUser.getId() << " and your username: " << currentUser.getUsername() << std::endl;
            currentUser.addUser();
        } else std::cout << "This username is already taken. Try again";
    }

    if (isAdmin) {
        bool infinityLoop = true;
        while (infinityLoop) {
            int command;
            printAdminCommands();
            std::cin >> command;
            switch (command) {
                case 0:
                    std::cout << "Logged out!";
                    infinityLoop = false;
                    break;
                case 1: {
                    std::cout << "Users list:\n";
                    std::vector<User> vec = getUsersList();
                    for (int i = 0; i < vec.size(); i++) {
                        std::cout << i + 1 << "." << vec[i].getUsername() << std::endl;
                    }
                    break;
                }
                case 2: {
                    std::cout << "Users list:\n";
                    std::vector<User> vec = getUsersList();
                    for (int i = 0; i < vec.size(); i++) {
                        std::cout << i + 1 << "." << vec[i].getUsername() << std::endl;
                    }
                    std::cout << "Enter his number in this list:\n";
                    int num;
                    std::cin >> num;
                    std::cout << "id: " << vec[num - 1].getId();
                }
                    break;
            }
        }
    }
    else {
        bool infinityLoop = true;
        while (infinityLoop) {
            int command;
            printCommands();
            std::cin >> command;
            switch (command) {
                case 0:
                    std::cout << "Logged out!";
                    infinityLoop = false;
                    break;
                case 1: {
                    std::cout << "Friends list:\n";
                    std::vector<User> vec = showFriendList(currentUser.getId());
                }
                    break;
                case 2: {
                    std::cout << "Enter his username:\n";
                    std::string friendUsername;
                    std::cin >> friendUsername;
                    currentUser.addFriend(friendUsername);
                }
                    break;
                case 3: {
                    std::cout << "Enter his number in this list:\n";
                    std::vector<User> vec = showFriendList(currentUser.getId());
                    int choice;
                    std::cin >> choice;
                    currentUser.removeFriend(vec[choice - 1]);
                }
                    break;
                case 4: {
                    std::cout << "Enter his number in this list:\n";
                    std::vector<User> vec = showFriendList(currentUser.getId());
                    int choice;
                    std::cin >> choice;
                    std::cout << "Input your message\n";
                    std::string message;
                    std::cin.ignore();
                    getline(std::cin, message);
                    currentUser.writeMessage(vec[choice - 1], message);
                }
                    break;
                default:
                    std::cout << "You entered wrong command. Try again\n";
            }

        }
    }

}

int main() {
    server();
}

void printAdminCommands() {
    std::cout << "1.Show users\n2. Check more info about user\n0. Log out\n";
}

std::vector<User> showFriendList(int currentId) {
    std::vector<User> users = getUsersList();
    std::vector<User> vec;
    std::string path = "/home/dzhavid/CLionProjects/laba/friends_" + std::to_string(currentId) + ".txt";
    std::ifstream file(path);
    std::string line;
    while(getline(file, line)) {
        int id = std::stoi(line);
        for (auto i : users) {
            if (i.getId() == id) vec.push_back(i);
        }
    }
    for (int i = 0; i < vec.size(); i++) {
        std::cout << i + 1 << "." << vec[i].getUsername() << " ";
    }
    std::cout << "\n\n";
    return vec;
}

void printCommands() {
    std::cout << "1. Show friends\n2. Add friend\n3. Delete friend\n4. Write message\n\n0. Log out\n";
}

void printCommandsAdmin() {
    std::cout << "1. Show users with info\n\n 0.Log out\n";
}

std::vector <User> getUsersList() {
    std::vector <User> list;
    std::ifstream file("/home/dzhavid/CLionProjects/laba/users.txt");
    std::string line;
    while (getline(file, line)) {
        User user = parseString(line);
        list.push_back(user);
    }
    file.close();
    return list;
}

User parseString(std::string line) {
    User user = User("name");
    int spaces = 0;
    std::string data = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
            spaces++;
            if (spaces == 1) user.setId(stoi(data));
            if (spaces == 2) user.setUsername(data);
            if (spaces == 3) user.setFriends(std::stoi(data));
            if (spaces == 4) user.setMessages(std::stoi(data));
            data = "";
        }
        else data += line[i];

        if (i == line.size() - 1) user.setRegistrationDate(data);
    }
    //std::cout << "parsing:: \n" << user.getId() << " " << user.getUsername() << " " << user.getFriends() << " " << user.getMessages() << " " << user.getRegistrationDate() << std::endl;
    //std::cout << "end of parsing\n";
    return user;
}

bool userExistence(int id, std::string username, User *user) {
    std::vector <User> list = getUsersList();
    for (auto & i : list) {
        if (i.getId() == id && i.getUsername() == username) {
            *user = i;
            return true;
        }
    }
    return false;
}

