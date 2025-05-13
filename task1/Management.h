#include <iostream>
#include <set>
#include <cstdint>

#ifndef MANAGEMENT
#define MANAGEMENT
class User
{
    uint32_t userId;
    std::string name;
    std::string email;
    std::set<uint32_t> groups;

    friend void deleteUser(uint32_t);
    friend void deleteGroup(uint32_t);
    friend void addUserToGroup(uint32_t, uint32_t);

public:
    User(uint32_t userId, std::string name, std::string email)
        : userId(userId), name(name), email(email) {}
    User() {}

    void getInfo()
    {
        std::cout << "User Info" << std::endl;
        std::cout << "userId: " << userId << std::endl;
        std::cout << "name: " << name << std::endl;
        std::cout << "email: " << email << std::endl;
        std::cout << "Groups: ";
        for (auto g : groups)
            std::cout << g << " ";
        std::cout << std::endl;
    }
};

class Group
{
    uint32_t groupId;
    std::set<uint32_t> userIds;

    friend void deleteGroup(uint32_t);
    friend void getGroup(uint32_t);
    friend void addUserToGroup(uint32_t, uint32_t);
    friend void deleteUser(uint32_t userId);
    friend void allGroups();

public:
    Group(uint32_t groupId) : groupId(groupId) {}
    Group() {}

    void getInfo()
    {
        std::cout << "Group Info" << std::endl;
        std::cout << "groupId: " << groupId << std::endl;
    }
};
#endif