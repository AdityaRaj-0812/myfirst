#include "operations.h"
#include <ctime>
#include <sstream>

namespace ATM {

int findUserIndex(const std::vector<User> &users, const std::string &username) {
    for (size_t i = 0; i < users.size(); ++i) if (users[i].username == username) return (int)i;
    return -1;
}

static std::string now() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

bool deposit(std::vector<User> &users, int idx, double amount, const std::string &txFile) {
    if (idx < 0 || idx >= (int)users.size()) return false;
    if (amount <= 0) return false;
    users[idx].balance += amount;
    std::ostringstream rec;
    rec << now() << "|DEPOSIT|" << users[idx].username << "|" << amount << "|" << users[idx].balance;
    appendTransaction(txFile, rec.str());
    return true;
}

bool withdraw(std::vector<User> &users, int idx, double amount, const std::string &txFile) {
    if (idx < 0 || idx >= (int)users.size()) return false;
    if (amount <= 0) return false;
    if (users[idx].balance < amount) return false;
    users[idx].balance -= amount;
    std::ostringstream rec;
    rec << now() << "|WITHDRAW|" << users[idx].username << "|" << amount << "|" << users[idx].balance;
    appendTransaction(txFile, rec.str());
    return true;
}

bool transfer(std::vector<User> &users, int fromIdx, const std::string &toUser, double amount, const std::string &txFile) {
    int toIdx = findUserIndex(users, toUser);
    if (fromIdx < 0 || fromIdx >= (int)users.size()) return false;
    if (toIdx < 0) return false;
    if (amount <= 0) return false;
    if (users[fromIdx].balance < amount) return false;
    users[fromIdx].balance -= amount;
    users[toIdx].balance += amount;
    std::ostringstream rec;
    rec << now() << "|TRANSFER|" << users[fromIdx].username << "->" << users[toIdx].username << "|" << amount << "|" << users[fromIdx].balance << "|" << users[toIdx].balance;
    appendTransaction(txFile, rec.str());
    return true;
}

void changePin(std::vector<User> &users, int idx, const std::string &newPin) {
    if (idx < 0 || idx >= (int)users.size()) return;
    users[idx].pin = newPin;
}

bool addUser(std::vector<User> &users, const std::string &username, const std::string &pin, double balance, const std::string &type) {
    if (username.empty() || pin.empty()) return false;
    if (findUserIndex(users, username) != -1) return false;
    User u; u.username = username; u.pin = pin; u.balance = balance; u.type = type;
    users.push_back(u);
    return true;
}

bool deleteUser(std::vector<User> &users, const std::string &username) {
    int idx = findUserIndex(users, username);
    if (idx < 0) return false;
    users.erase(users.begin() + idx);
    return true;
}

} // namespace ATM
