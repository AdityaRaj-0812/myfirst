#pragma once
#include "file_utils.h"
#include <string>
#include <vector>

namespace ATM {

// Utility functions
int findUserIndex(const std::vector<User> &users, const std::string &username);

// User operations (procedural style)
bool deposit(std::vector<User> &users, int idx, double amount, const std::string &txFile);
bool withdraw(std::vector<User> &users, int idx, double amount, const std::string &txFile);
bool transfer(std::vector<User> &users, int fromIdx, const std::string &toUser, double amount, const std::string &txFile);
void changePin(std::vector<User> &users, int idx, const std::string &newPin);

// Admin operations
bool addUser(std::vector<User> &users, const std::string &username, const std::string &pin, double balance, const std::string &type);
bool deleteUser(std::vector<User> &users, const std::string &username);

} // namespace ATM
