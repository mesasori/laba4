#include <string>
#include <fstream>
#include "chrono"
#include "ctime"
#include "User.h"
#include <vector>
#include "cmath"

User parseLine(std::string line);

User::User(std::string username) {
    User::setUsername(username);
    User::setId(0);
    User::setFriends(0);
    User::setMessages(0);

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string registrationDate = std::ctime(&end_time);
    std::string formattedDate = "";
    for (int i = 0; i < registrationDate.size(); i++) {
        if (registrationDate[i] == ' ') formattedDate += '_';
        else formattedDate += registrationDate[i];
    }
    User::setRegistrationDate(formattedDate);
}

std::vector<int> getFriendsList(int id);
bool alreadyFriend(int userId, int friendId);

bool User::addFriend(std::string username) {
    std::vector <User> list = getAnotherUsersList();
    int friendId = -1;
    for (auto & listFriend : list) {
        if (listFriend.getUsername() == username) friendId = listFriend.getId();
    }
    if (friendId == -1) {
        std::cout << "User isn't found. Try another username\n";
        return false;
    } else {
        if (alreadyFriend(User::getId(), friendId)) {
            std::cout << "He's already your friend!\n";
            return false;
        } else {
            //корректирование файла с юзерами
            std::ofstream file1("/home/mesasori/CLionProjects/laba3/users.txt");
            file1.close();
            std::ofstream file("/home/mesasori/CLionProjects/laba3/users.txt", std::ios::app);
            if (file.is_open()) {
                for (int i = 0; i < list.size(); i++) {
                    //прибавка к счетчику другу
                    if (list[i].getUsername() != username) {
                        file << list[i].getId() << " " << list[i].getUsername() << " " << list[i].getFriends() << " "
                             << list[i].getMessages() << " " << list[i].getRegistrationDate() << "\n";
                    } else {
                        file << list[i].getId() << " " << list[i].getUsername() << " " << list[i].getFriends() + 1 << " "
                             << list[i].getMessages() << " " << list[i].getRegistrationDate() << "\n";
                    }
                }
                User::setFriends(User::getFriends() + 1);
                file << User::getId() << " " << User::getUsername() << " " << User::getFriends() << " "
                     << User::getMessages() << " " << User::getRegistrationDate() << "\n";
                file.close();
                std::cout << "Friend added!\n";
            } else {
                std::cout << "Fault. Try again\n";
                return false;
            }

            //корректирование своего файла с друзьями
            std::string path = "/home/mesasori/CLionProjects/laba3/friends_" + std::to_string(User::getId()) + ".txt";
            std::ofstream fileFriends(path, std::ios::app);
            if (fileFriends.is_open()) {
                fileFriends << std::to_string(friendId) << "\n";
                fileFriends.close();
                std::cout << "damned\n";
            } else {
                std::cout << "Fault. Try again\n";
                return false;
            }

            //корректирование файла друга с друзьями
            std::string friendPath = "/home/mesasori/CLionProjects/laba3/friends_" + std::to_string(friendId) + ".txt";
            std::ofstream friendFileFriends(friendPath, std::ios::app);
            if (friendFileFriends.is_open()) {
                friendFileFriends << std::to_string(User::getId());
                friendFileFriends.close();
            } else {
                std::cout << "Fault. Try again\n";
                return false;
            }

            return true;
        }
    }
}

bool User::writeMessage(User user, std::string message) {
    std::vector <User> list = getAnotherUsersList();
    //изменяю файл юзеров
    std::ofstream file1("/home/mesasori/CLionProjects/laba3/users.txt");
    file1.close();
    std::ofstream file("/home/mesasori/CLionProjects/laba3/users.txt", std::ios::app);
    if (file.is_open()) {
        for (int i = 0; i < list.size(); i++) {
            if (list[i].getId() == user.getId()) {
                file << list[i].getId() << " " << list[i].getUsername() << " " << list[i].getFriends() << " " << list[i].getMessages() + 1 << " " << list[i].getRegistrationDate() << "\n";
            } else {
                file << list[i].getId() << " " << list[i].getUsername() << " " << list[i].getFriends() << " " << list[i].getMessages() << " " << list[i].getRegistrationDate() << "\n";
            }
        }
        User::setMessages(User::getMessages() + 1);
        file << User::getId() << " " << User::getUsername() << " " << User::getFriends() << " " << User::getMessages() << " " << User::getRegistrationDate() << "\n";
        file.close();
        std::cout << "Message sent!\n";

    }
    else {
        std::cout << "Fault. Try again\n";
        return false;
    }

    //Запись сообщения в общий файл
    std::string messagePath = "/home/mesasori/CLionProjects/laba3/messages" + std::to_string(std::min(user.getId(), User::getId())) + "_"
            + std::to_string(std::max(user.getId(), User::getId())) + ".txt";
    std::ofstream messageFile(messagePath, std::ios::app);
    if (messageFile.is_open()) {
        messageFile << std::to_string(User::getId()) + ">>" + message + "\n";
        messageFile.close();
        std::cout << "Message is sent!\n\n";
    } else {
        std::cout << "Fault. Try again\n";
        return false;
    }

    return true;
}

bool User::removeFriend(User user) {
    std::vector <User> list = getAnotherUsersList();

    //корректирование файла с юзерами
    std::ofstream file1("/home/mesasori/CLionProjects/laba3/users.txt");
    file1.close();
    std::ofstream file("/home/mesasori/CLionProjects/laba3/users.txt", std::ios::app);
    if (file.is_open()) {
        for (auto & i : list) {
            //прибавка к счетчику другу
            if (i.getUsername() != username) {
                file << i.getId() << " " << i.getUsername() << " " << i.getFriends() << " "
                     << i.getMessages() << " " << i.getRegistrationDate() << "\n";
            } else {
                file << i.getId() << " " << i.getUsername() << " " << i.getFriends() - 1 << " "
                     << i.getMessages() << " " << i.getRegistrationDate() << "\n";
            }
        }
        User::setFriends(User::getFriends() - 1);
        file << User::getId() << " " << User::getUsername() << " " << User::getFriends() << " "
             << User::getMessages() << " " << User::getRegistrationDate() << "\n";
        file.close();
        std::cout << "Friend removed!\n";
    } else {
        std::cout << "Fault. Try again\n";
        return false;
    }

    //корректирование своего файла с друзьями
    std::vector <int> myFriendsList = getFriendsList(User::getId());
    std::string path = "/home/mesasori/CLionProjects/laba3/friends_" + std::to_string(User::getId()) + ".txt";
    std::ofstream fileFriends(path);
    if (fileFriends.is_open()) {
        for (auto i : myFriendsList) {
            if (i != user.getId()) fileFriends << i << "\n";
        }
        fileFriends.close();

    } else {
        std::cout << "Fault. Try again\n";
        return false;
    }

    //корректирование файла друга с друзьями
    std::vector <int> friendFriendsList = getFriendsList(user.getId());
    std::string friendPath = "/home/mesasori/CLionProjects/laba3/friends_" + std::to_string(user.getId()) + ".txt";
    std::ofstream friendFileFriends(friendPath);
    if (friendFileFriends.is_open()) {
        for (auto i : friendFriendsList) {
            if (i != User::getId()) friendFileFriends << i << "\n";
        }
        friendFileFriends.close();
    } else {
        std::cout << "Fault. Try again\n";
        return false;
    }

    return true;
}

bool User::addUser() {
    std::ofstream fout("/home/mesasori/CLionProjects/laba3/users.txt", std::ios::app);
    if (fout.is_open()) {
        fout << User::getId() << " " << User::getUsername() << " " << User::getFriends() << " " << User::getMessages() << " " << User::getRegistrationDate();
        fout.close();
        std::cout << "Success!\n";
        return true;
    }
    else {
        std::cout << "Fault. Try again\n";
        return false;
    }
}

int User::getId() {
    return id;
}
void User::setId(int id) {
    User::id = id;
}

std::string User::getUsername() {
    return username;
}
void User::setUsername(std::string username) {
    User::username = username;
}

std::string User::getRegistrationDate() {
    return registrationDate;
}
void User::setRegistrationDate(std::string registrationDate) {
    User::registrationDate = registrationDate;
}

int User::getFriends() {
    return friends;
}
void User::setFriends(int friends) {
    User::friends = friends;
}

int User::getMessages() {
    return messages;
}
void User::setMessages(int messages) {
    User::messages = messages;
}

std::vector <User> User::getAnotherUsersList() {
    std::vector <User> list;
    std::ifstream file("/home/mesasori/CLionProjects/laba3/users.txt");
    std::string line;
    while (getline(file, line)) {
        User user = parseLine(line);
        if (user.getId() != User::getId()) {
            list.push_back(user);
        }
    }
    file.close();
    return list;
}

std::vector <int> getFriendsList(int id) {
    std::string path = "/home/mesasori/CLionProjects/laba3/friends_" + std::to_string(id) + ".txt";
    std::ifstream file(path);
    std::vector <int> friends;
    std::string line;
    while (getline(file, line)) {
        friends.push_back(std::stoi(line));
    }
    return friends;
}

bool alreadyFriend(int userId, int friendId) {
    std::string path = "/home/mesasori/CLionProjects/laba3/friends_" + std::to_string(userId) + ".txt";
    std::ifstream file(path);
    std::string id;
    while(getline(file, id)) {
        if (stoi(id) == friendId) {
            file.close();
            return true;
        }
    }
    return false;
}

User::User() {}

User parseLine(std::string line) {
    User user;
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

void inform();

