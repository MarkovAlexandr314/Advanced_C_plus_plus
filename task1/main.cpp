#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <map>
#include <set>
#include "Management.h"

using namespace std;

map<uint32_t, User> users;
map<uint32_t, Group> groups;

void createUser(uint32_t userId, string name, string email)
{
    if (users.count(userId))
    {
        cout << "User with ID " << userId << " already exists. Replace? (Y/N): ";
        char answer;
        cin >> answer;
        if (answer != 'Y' && answer != 'y')
            return;
        cin.ignore();
    }
    users[userId] = User(userId, name, email);
    cout << "User created." << endl;
}

void deleteUser(uint32_t userId)
{
    if (!users.erase(userId))
    {
        cout << "User not found." << endl;
        return;
    }
    for (auto &[gid, group] : groups)
    {
        group.userIds.erase(userId);
    }
    cout << "User deleted." << endl;
}

void createGroup(uint32_t groupId)
{
    if (groups.count(groupId))
    {
        cout << "Group with ID " << groupId << " already exists. Replace? (Y/N): ";
        char answer;
        cin >> answer;
        if (answer != 'Y' && answer != 'y')
            return;
        cin.ignore();
    }
    groups[groupId] = Group(groupId);
    cout << "Group created." << endl;
}

void deleteGroup(uint32_t groupId)
{
    if (!groups.erase(groupId))
    {
        cout << "Group not found." << endl;
        return;
    }
    for (auto &[uid, user] : users)
    {
        user.groups.erase(groupId);
    }
    cout << "Group deleted." << endl;
}

void allUsers()
{
    if (users.empty())
    {
        cout << "No users found." << endl;
        return;
    }
    for (auto &[id, user] : users)
    {
        user.getInfo();
        cout << endl;
    }
}

void getUser(uint32_t userId)
{
    auto it = users.find(userId);
    if (it == users.end())
    {
        cout << "User not found." << endl;
        return;
    }
    it->second.getInfo();
}

void getGroup(uint32_t groupId)
{
    auto it = groups.find(groupId);
    if (it == groups.end())
    {
        cout << "Group not found." << endl;
        return;
    }
    it->second.getInfo();
    cout << "Members: ";
    for (auto uid : it->second.userIds)
    {
        cout << uid << " ";
    }
    cout << endl;
}

void allGroups()
{
    if (groups.empty())
    {
        cout << "No groups found." << endl;
        return;
    }
    for (auto &[id, group] : groups)
    {
        group.getInfo();
        cout << "Members: ";
        for (auto uid : group.userIds)
        {
            cout << uid << " ";
        }
        cout << endl
             << endl;
    }
}

vector<string> split(const string &s)
{
    vector<string> tokens;
    string token;
    istringstream iss(s);
    while (iss >> token)
    {
        tokens.push_back(token);
    }
    return tokens;
}

void addUserToGroup(uint32_t userId, uint32_t groupId)
{
    if (!users.count(userId))
    {
        cout << "User not found!" << endl;
        return;
    }
    if (!groups.count(groupId))
    {
        cout << "Group not found!" << endl;
        return;
    }

    users[userId].groups.insert(groupId);
    groups[groupId].userIds.insert(userId);
    cout << "User " << userId << " added to group " << groupId << endl;
}

void printHelp()
{
    cout << "Available commands:\n"
         << "  createUser <userId> <username> <email>\n"
         << "  deleteUser <userId>\n"
         << "  allUsers\n"
         << "  getUser <userId>\n"
         << "  createGroup <groupId>\n"
         << "  deleteGroup <groupId>\n"
         << "  allGroups\n"
         << "  getGroup <groupId>\n"
         << "  addUserToGroup <userId> <groupId>\n"
         << "  help - Show this help\n"
         << "  exit - Exit program\n";
}

int main()
{
    string line;
    while (true)
    {
        cout << "> ";
        getline(cin, line);
        if (line.empty())
            continue;

        auto tokens = split(line);
        if (tokens.empty())
            continue;

        string cmd = tokens[0];
        if (cmd == "exit")
        {
            break;
        }
        else if (cmd == "createUser")
        {
            if (tokens.size() < 4)
            {
                cout << "Usage: createUser {userId} {username} {email}" << endl;
                continue;
            }
            try
            {
                uint32_t id = stoul(tokens[1]);
                createUser(id, tokens[2], tokens[3]);
            }
            catch (...)
            {
                cout << "Invalid user ID" << endl;
            }
        }
        else if (cmd == "deleteUser")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: deleteUser {userId}" << endl;
                continue;
            }
            try
            {
                uint32_t id = stoul(tokens[1]);
                deleteUser(id);
            }
            catch (...)
            {
                cout << "Invalid user ID" << endl;
            }
        }
        else if (cmd == "allUsers")
        {
            allUsers();
        }
        else if (cmd == "getUser")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: getUser {userId}" << endl;
                continue;
            }
            try
            {
                uint32_t id = stoul(tokens[1]);
                getUser(id);
            }
            catch (...)
            {
                cout << "Invalid user ID" << endl;
            }
        }
        else if (cmd == "createGroup")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: createGroup {groupId}" << endl;
                continue;
            }
            try
            {
                uint32_t id = stoul(tokens[1]);
                createGroup(id);
            }
            catch (...)
            {
                cout << "Invalid group ID" << endl;
            }
        }
        else if (cmd == "deleteGroup")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: deleteGroup {groupId}" << endl;
                continue;
            }
            try
            {
                uint32_t id = stoul(tokens[1]);
                deleteGroup(id);
            }
            catch (...)
            {
                cout << "Invalid group ID" << endl;
            }
        }
        else if (cmd == "allGroups")
        {
            allGroups();
        }
        else if (cmd == "getGroup")
        {
            if (tokens.size() < 2)
            {
                cout << "Usage: getGroup {groupId}" << endl;
                continue;
            }
            try
            {
                uint32_t id = stoul(tokens[1]);
                getGroup(id);
            }
            catch (...)
            {
                cout << "Invalid group ID" << endl;
            }
        }
        else if (cmd == "addUserToGroup")
        {
            if (tokens.size() < 3)
            {
                cout << "Usage: addUserToGroup {userId} {groupId}" << endl;
                continue;
            }
            try
            {
                uint32_t uid = stoul(tokens[1]);
                uint32_t gid = stoul(tokens[2]);
                addUserToGroup(uid, gid);
            }
            catch (...)
            {
                cout << "Invalid arguments" << endl;
            }
        }
        else if (cmd == "help")
        {
            printHelp();
        }
        else
        {
            cout << "Unknown command. Type 'help' for available commands." << endl;
        }
    }
    return 0;
}