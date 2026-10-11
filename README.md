# Bank Client Management System (C++)

A console-based bank client management system written in C++, using file storage.

## Features
- List all clients
- Add new client (with duplicate account number check)
- Delete client
- Update client info
- Find client by account number
- Transactions: deposit, withdraw, total balances
- Input validation (empty fields, invalid numbers, invalid menu choices)

## Concepts Used
Structs, enums, vectors, file handling (fstream), string splitting, function decomposition, pass by reference / const reference.

## How to Run
1. Download or clone the repository.
2. Open `Bank-Client-Management-System-cpp.sln` in Visual Studio.
3. Make sure `Clients.txt` is in the project folder (next to the .cpp file).
4. Build and run (Ctrl + F5).

## Data Format
Each client is one line, fields separated by `#//#`:

`AccountNumber#//#PinCode#//#Name#//#Phone#//#Balance`

Example:

`A150#//#1234#//#Mohamed Ali#//#0100000000#//#5000.000000`

## Note
Windows only, because it uses `system("cls")` and `system("pause>0")`.
