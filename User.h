#include "iostream"
#include "string"
#include "vector"
#pragma once

class User {
public:
    User();

    User(std::string);

    int getId();
    void setId(int);

    std::string getUsername();
    void setUsername(std::string);

    std::string getRegistrationDate();
    void setRegistrationDate(std::string);

    int getFriends();
    void setFriends(int);

    int getMessages();
    void setMessages(int);



    bool addUser();
    bool addFriend(std::string);
    bool writeMessage(User, std::string);
    bool removeFriend(User);
    std::vector<User> getAnotherUsersList();


private:
    int id;
    std::string username;
    std::string registrationDate;
    int friends;
    int messages;
};

