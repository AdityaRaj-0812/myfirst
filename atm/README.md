ATM simulation (procedural C++)

Structure:
- main.cpp: CLI and menus
- file_utils.h/.cpp: file handling for users and transactions
- operations.h/.cpp: procedural operations (deposit/withdraw/transfer/admin tasks)
- data/users.txt: stored user records (username|pin|balance|type)
- data/transactions.txt: append-only transaction log

Build:

```bash
g++ atm/*.cpp -std=c++17 -O2 -o atm/atm
```

Run:

```bash
./atm/atm
```

Default accounts: admin (PIN 1234) and user1 (PIN 1111). Use admin to add/delete users.
