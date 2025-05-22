#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdint>
#include <map>
#include <set>

#ifndef MANAGEMENT
#define MANAGEMENT

using namespace std;

class User {
private:
    uint32_t userId;
    string name;
    string email;
    set<uint32_t> groups;

public:
    User(uint32_t userId, string name, string email)
        : userId(userId), name(name), email(email) {}

    User() {}

    void addGroup(uint32_t groupId) { groups.insert(groupId); }
    void removeGroup(uint32_t groupId) { groups.erase(groupId); }

    void printInfo() const {
        cout << "User Info\n"
             << "userId: " << userId << "\n"
             << "name: " << name << "\n"
             << "email: " << email << "\n"
             << "Groups: ";
        for (auto g : groups) cout << g << " ";
        cout << "\n" << endl;
    }
};

class Group {
private:
    uint32_t groupId;
    set<uint32_t> userIds;

public:
    Group(uint32_t groupId) : groupId(groupId) {}

    Group() {}

    void addMember(uint32_t userId) { userIds.insert(userId); }
    void removeMember(uint32_t userId) { userIds.erase(userId); }

    void printInfo() const {
        cout << "Group Info\n"
             << "groupId: " << groupId << "\n"
             << "Members: ";
        for (auto uid : userIds) cout << uid << " ";
        cout << "\n" << endl;
    }
};

class ManagementSystem {
private:
    map<uint32_t, User> users;
    map<uint32_t, Group> groups;

public:
    bool userExists(uint32_t userId) const { return users.count(userId); }
    bool groupExists(uint32_t groupId) const { return groups.count(groupId); }

    void createUser(uint32_t userId, string name, string email) {
        users[userId] = User(userId, name, email);
    }

    bool deleteUser(uint32_t userId) {
        if (!users.erase(userId)) return false;
        
        for (auto& [_, group] : groups) {
            group.removeMember(userId);
        }
        return true;
    }

    void createGroup(uint32_t groupId) {
        groups[groupId] = Group(groupId);
    }

    bool deleteGroup(uint32_t groupId) {
        if (!groups.erase(groupId)) return false;
        
        for (auto& [_, user] : users) {
            user.removeGroup(groupId);
        }
        return true;
    }

    bool addUserToGroup(uint32_t userId, uint32_t groupId) {
        if (!userExists(userId) || !groupExists(groupId)) return false;
        
        users.at(userId).addGroup(groupId);
        groups.at(groupId).addMember(userId);
        return true;
    }

    vector<User> getAllUsers() const {
        vector<User> result;
        for (const auto& [_, user] : users) result.push_back(user);
        return result;
    }

    vector<Group> getAllGroups() const {
        vector<Group> result;
        for (const auto& [_, group] : groups) result.push_back(group);
        return result;
    }

    const User* getUser(uint32_t userId) const {
        auto it = users.find(userId);
        return it != users.end() ? &it->second : nullptr;
    }

    const Group* getGroup(uint32_t groupId) const {
        auto it = groups.find(groupId);
        return it != groups.end() ? &it->second : nullptr;
    }
};

class ConsoleUI {
public:
    void run() {
        string line;
        while (true) {
            cout << "> ";
            getline(cin, line);
            auto tokens = split(line);
            if (tokens.empty()) continue;

            const string& cmd = tokens[0];
            if (cmd == "exit") break;
            else if (cmd == "createUser") handleCreateUser(tokens);
            else if (cmd == "deleteUser") handleDeleteUser(tokens);
            else if (cmd == "allUsers") handleAllUsers();
            else if (cmd == "getUser") handleGetUser(tokens);
            else if (cmd == "createGroup") handleCreateGroup(tokens);
            else if (cmd == "deleteGroup") handleDeleteGroup(tokens);
            else if (cmd == "allGroups") handleAllGroups();
            else if (cmd == "getGroup") handleGetGroup(tokens);
            else if (cmd == "addUserToGroup") handleAddUserToGroup(tokens);
            else if (cmd == "help") printHelp();
            else cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

private:
    ManagementSystem system;
    
    vector<string> split(const string& s) {
        vector<string> tokens;
        string token;
        istringstream iss(s);
        while (iss >> token) tokens.push_back(token);
        return tokens;
    }

    void printHelp() {
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
    void handleCreateUser(const vector<string>& tokens) {
        if (tokens.size() < 4) {
            cout << "Usage: createUser <userId> <username> <email>\n";
            return;
        }
        try {
            uint32_t id = stoul(tokens[1]);
            if (system.userExists(id)) {
                cout << "User with ID " << id << " exists. Replace? (Y/N): ";
                char answer; cin >> answer; cin.ignore();
                if (toupper(answer) != 'Y') return;
            }
            system.createUser(id, tokens[2], tokens[3]);
            cout << "User created.\n";
        } catch (...) {
            cout << "Invalid user ID\n";
        }
    }

    void handleDeleteUser(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Usage: deleteUser <userId>\n";
            return;
        }
        try {
            uint32_t id = stoul(tokens[1]);
            cout << (system.deleteUser(id) ? "User deleted.\n" : "User not found.\n");
        } catch (...) {
            cout << "Invalid user ID\n";
        }
    }

    void handleAllUsers() {
        auto users = system.getAllUsers();
        if (users.empty()) {
            cout << "No users found.\n";
            return;
        }
        for (const auto& user : users) user.printInfo();
    }

    void handleGetUser(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Usage: getUser <userId>\n";
            return;
        }
        try {
            uint32_t id = stoul(tokens[1]);
            if (const User* user = system.getUser(id)) user->printInfo();
            else cout << "User not found.\n";
        } catch (...) {
            cout << "Invalid user ID\n";
        }
    }

    void handleCreateGroup(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Usage: createGroup <groupId>\n";
            return;
        }
        try {
            uint32_t id = stoul(tokens[1]);
            if (system.groupExists(id)) {
                cout << "Group with ID " << id << " exists. Replace? (Y/N): ";
                char answer; cin >> answer; cin.ignore();
                if (toupper(answer) != 'Y') return;
            }
            system.createGroup(id);
            cout << "Group created.\n";
        } catch (...) {
            cout << "Invalid group ID\n";
        }
    }

    void handleDeleteGroup(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Usage: deleteGroup <groupId>\n";
            return;
        }
        try {
            uint32_t id = stoul(tokens[1]);
            cout << (system.deleteGroup(id) ? "Group deleted.\n" : "Group not found.\n");
        } catch (...) {
            cout << "Invalid group ID\n";
        }
    }

    void handleAllGroups() {
        auto groups = system.getAllGroups();
        if (groups.empty()) {
            cout << "No groups found.\n";
            return;
        }
        for (const auto& group : groups) group.printInfo();
    }

    void handleGetGroup(const vector<string>& tokens) {
        if (tokens.size() < 2) {
            cout << "Usage: getGroup <groupId>\n";
            return;
        }
        try {
            uint32_t id = stoul(tokens[1]);
            if (const Group* group = system.getGroup(id)) group->printInfo();
            else cout << "Group not found.\n";
        } catch (...) {
            cout << "Invalid group ID\n";
        }
    }

    void handleAddUserToGroup(const vector<string>& tokens) {
        if (tokens.size() < 3) {
            cout << "Usage: addUserToGroup <userId> <groupId>\n";
            return;
        }
        try {
            uint32_t uid = stoul(tokens[1]);
            uint32_t gid = stoul(tokens[2]);
            if (system.addUserToGroup(uid, gid)) {
                cout << "User " << uid << " added to group " << gid << endl;
            } else {
                cout << "User or group not found!\n";
            }
        } catch (...) {
            cout << "Invalid arguments\n";
        }
    }
};
#endif