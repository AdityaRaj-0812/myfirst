#pragma once
#include <string>
#include <vector>

namespace ATM {

struct User {
    std::string username;
    std::string pin;
    double balance;
    std::string type; // "common" or "admin"
};

// Data file helpers
bool ensureDataFiles(const std::string &dataDir);
bool loadUsers(const std::string &userFile, std::vector<User> &users);
bool saveUsers(const std::string &userFile, const std::vector<User> &users);
void appendTransaction(const std::string &txFile, const std::string &record);

} // namespace ATM
