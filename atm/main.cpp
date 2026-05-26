#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>

#include "file_utils.h"
#include "operations.h"

using namespace std;
using namespace ATM;

static const string DATA_DIR = "./atm/data";
static const string USERS_FILE = DATA_DIR + "/users.txt";
static const string TX_FILE = DATA_DIR + "/transactions.txt";

static void pause() { cout << "Press Enter to continue..."; cin.ignore(numeric_limits<streamsize>::max(), '\n'); }

int main() {
    ensureDataFiles(DATA_DIR);
    vector<User> users;
    if (!loadUsers(USERS_FILE, users)) {
        cerr << "Failed to load users\n";
        return 1;
    }

    while (true) {
        cout << "--- ATM ---\n";
        cout << "1) Login\n2) Register (common user)\n3) Exit\nChoose: ";
        int choice; if (!(cin >> choice)) break;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (choice == 3) break;
        if (choice == 2) {
            string uname, pin;
            double bal = 0.0;
            cout << "Enter new username: "; getline(cin, uname);
            cout << "Enter new PIN: "; getline(cin, pin);
            if (addUser(users, uname, pin, bal, "common")) {
                saveUsers(USERS_FILE, users);
                cout << "User added.\n";
            } else cout << "Failed to add user (exists?).\n";
            pause();
            continue;
        }
        if (choice == 1) {
            string uname, pin;
            cout << "Username: "; getline(cin, uname);
            cout << "PIN: "; getline(cin, pin);
            int idx = findUserIndex(users, uname);
            if (idx == -1 || users[idx].pin != pin) {
                cout << "Invalid credentials.\n";
                pause();
                continue;
            }
            bool logged = true;
            if (users[idx].type == "common") {
                while (logged) {
                    cout << "--- User Menu (" << users[idx].username << ")---\n";
                    cout << "1) Check Balance\n2) Deposit\n3) Withdraw\n4) Transfer\n5) Change PIN\n6) Logout\nChoose: ";
                    int c; if (!(cin >> c)) { logged = false; break; }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (c == 1) {
                        cout << "Balance: " << users[idx].balance << "\n";
                    } else if (c == 2) {
                        double amt; cout << "Amount to deposit: "; cin >> amt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (deposit(users, idx, amt, TX_FILE)) { saveUsers(USERS_FILE, users); cout << "Deposit successful.\n"; } else cout << "Deposit failed.\n";
                    } else if (c == 3) {
                        double amt; cout << "Amount to withdraw: "; cin >> amt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (withdraw(users, idx, amt, TX_FILE)) { saveUsers(USERS_FILE, users); cout << "Withdraw successful.\n"; } else cout << "Withdraw failed (insufficient funds?).\n";
                    } else if (c == 4) {
                        string to; double amt; cout << "Transfer to username: "; getline(cin, to);
                        cout << "Amount: "; cin >> amt; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (transfer(users, idx, to, amt, TX_FILE)) { saveUsers(USERS_FILE, users); cout << "Transfer successful.\n"; } else cout << "Transfer failed.\n";
                    } else if (c == 5) {
                        string np; cout << "New PIN: "; getline(cin, np);
                        changePin(users, idx, np); saveUsers(USERS_FILE, users); cout << "PIN changed.\n";
                    } else if (c == 6) { logged = false; }
                    pause();
                }
            } else if (users[idx].type == "admin") {
                while (logged) {
                    cout << "--- Admin Menu (" << users[idx].username << ")---\n";
                    cout << "1) Add User\n2) Delete User\n3) View All Users\n4) View Transactions\n5) Logout\nChoose: ";
                    int c; if (!(cin >> c)) { logged = false; break; }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (c == 1) {
                        string uname, pin; double bal; string type;
                        cout << "Username: "; getline(cin, uname);
                        cout << "PIN: "; getline(cin, pin);
                        cout << "Initial balance: "; cin >> bal; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Type (common/admin): "; getline(cin, type);
                        if (addUser(users, uname, pin, bal, type)) { saveUsers(USERS_FILE, users); cout << "Added.\n"; } else cout << "Failed to add.\n";
                    } else if (c == 2) {
                        string uname; cout << "Username to delete: "; getline(cin, uname);
                        if (deleteUser(users, uname)) { saveUsers(USERS_FILE, users); cout << "Deleted.\n"; } else cout << "Delete failed.\n";
                    } else if (c == 3) {
                        cout << "Users:\n";
                        for (const auto &u : users) cout << u.username << " | " << u.type << " | " << u.balance << "\n";
                    } else if (c == 4) {
                        ifstream in(TX_FILE);
                        cout << "Transactions:\n";
                        string line;
                        while (getline(in, line)) cout << line << "\n";
                        in.close();
                    } else if (c == 5) { logged = false; }
                    pause();
                }
            }
        }
    }

    cout << "Goodbye\n";
    return 0;
}
