#include "file_utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

namespace ATM {

bool ensureDataFiles(const string &dataDir) {
    try {
        if (!fs::exists(dataDir)) fs::create_directories(dataDir);
        string users = dataDir + "/users.txt";
        string tx = dataDir + "/transactions.txt";
        if (!fs::exists(users)) {
            ofstream out(users);
            // default admin and a sample common user
            out << "admin|1234|10000|admin\n";
            out << "user1|1111|5000|common\n";
            out.close();
        }
        if (!fs::exists(tx)) {
            ofstream out(tx);
            out.close();
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool parseUserLine(const string &line, User &u) {
    if (line.empty()) return false;
    stringstream ss(line);
    string part;
    vector<string> parts;
    while (getline(ss, part, '|')) parts.push_back(part);
    if (parts.size() < 4) return false;
    u.username = parts[0];
    u.pin = parts[1];
    try { u.balance = stod(parts[2]); } catch (...) { u.balance = 0.0; }
    u.type = parts[3];
    return true;
}

bool loadUsers(const string &userFile, vector<User> &users) {
    users.clear();
    ifstream in(userFile);
    if (!in.is_open()) return false;
    string line;
    while (getline(in, line)) {
        User u;
        if (parseUserLine(line, u)) users.push_back(u);
    }
    in.close();
    return true;
}

bool saveUsers(const string &userFile, const vector<User> &users) {
    ofstream out(userFile, ios::trunc);
    if (!out.is_open()) return false;
    for (const auto &u : users) {
        out << u.username << "|" << u.pin << "|" << fixed << setprecision(2) << u.balance << "|" << u.type << "\n";
    }
    out.close();
    return true;
}

void appendTransaction(const string &txFile, const string &record) {
    ofstream out(txFile, ios::app);
    if (!out.is_open()) return;
    out << record << "\n";
    out.close();
}

} // namespace ATM
