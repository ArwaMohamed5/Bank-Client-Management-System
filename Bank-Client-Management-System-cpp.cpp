#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

enum enMainMenuOptions
{
	eListClients = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eTransactions = 6,
	eExit = 7
};

enum enTransactionsMenuOptions
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
	eMainMenu = 4
};

const string clientsFileName = "Clients.txt";

struct stClient
{
	string accountNumber;
	string pinCode;
	string name;
	string phone;
	bool MarkForDelete = false;
	double accountBalance = 0;
};

void showTransactionsMenuScreen();

void showMainMenuScreen();

double readPositiveNumber(string message)
{
	double number;
	cout << message;
	while (!(cin >> number) || number <= 0)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Invalid input, " << message;
	}
	return number;
}

double readNonNegativeNumber(string message)
{
	double number;
	cout << message;
	while (!(cin >> number) || number < 0)
	{
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Invalid input, " << message;
	}
	return number;
}

string readNonEmptyString(string message)
{
	string text;
	cout << message;
	getline(cin >> ws, text);
	while (text.empty())
	{
		cout << "Field cannot be empty, " << message;
		getline(cin >> ws, text);
	}
	return text;
}

vector<string> SplitString(string String, string delim)
{
	vector<string>Words;
	string SWord;
	size_t Pos = 0;
	while ((Pos = String.find(delim)) != std::string::npos)
	{
		SWord = String.substr(0, Pos);
		if (SWord != "")
			Words.push_back(SWord);
		String.erase(0, Pos + delim.length());
	}
	if (String != "")
		Words.push_back(String);
	return Words;
}

void addClientToFile(string fileName, string stLineData)
{
	ofstream myFile;
	myFile.open(fileName, ios::out | ios::app);
	if (myFile.is_open())
	{
		myFile << stLineData << endl;
		myFile.close();
	}

}

stClient convertLineToRecord(string lineRecord, string separator = "#//#")
{
	stClient client;
	vector<string>vClientData;

	vClientData = SplitString(lineRecord, separator);

	client.accountNumber = vClientData[0];
	client.pinCode = vClientData[1];
	client.name = vClientData[2];
	client.phone = vClientData[3];
	client.accountBalance = stod(vClientData[4]);
	return client;
}

void printClientRecordLine(const stClient& Client)
{
	cout << "| " << setw(15) << left << Client.accountNumber;
	cout << "| " << setw(10) << left << Client.pinCode;
	cout << "| " << setw(40) << left << Client.name;
	cout << "| " << setw(12) << left << Client.phone;
	cout << "| " << setw(12) << left << Client.accountBalance;
}

vector<stClient> LoadClientsDataFromFile(string clients)
{
	vector<stClient> clientsData;
	ifstream myFile;
	myFile.open(clients, ios::in);

	if (myFile.is_open())
	{
		string line;
		stClient clientData;

		while (getline(myFile, line))
		{
			if (line.empty()) continue;
			clientData = convertLineToRecord(line);
			clientsData.push_back(clientData);
		}
		myFile.close();
	}
	return clientsData;
}

void showAllClientsScreen()
{
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << clientsData.size() << "): Client(s).";
	cout << "\n______________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n______________________________________________________________________________________________________\n";
	if (clientsData.size() == 0)

		cout << "\t\t\tNo Clients Available In The System!";

	else

		for (const stClient& Client : clientsData)
		{
			printClientRecordLine(Client);
			cout << endl;
		}

	cout << "\n______________________________________________________________________________________________________\n\n";
}

string readAccountNumber()
{
	string accountNumber;
	cout << "\nPlease Enter Account Number ? ";
	getline(cin >> ws, accountNumber);
	return accountNumber;

}

string convertRecordToLine(const stClient& client, string separator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += client.accountNumber + separator;
	stClientRecord += client.pinCode + separator;
	stClientRecord += client.name + separator;
	stClientRecord += client.phone + separator;
	stClientRecord += to_string(client.accountBalance);
	return stClientRecord;
}

bool SearchForClientByAccountNumber(stClient& clientData, const string& AccountNumber, const vector<stClient>& clientsData)
{

	for (const stClient& C : clientsData)
	{
		if (C.accountNumber == AccountNumber)
		{
			clientData = C;
			return true;
		}
	}
	return false;
}

bool ClientExistsByAccountNumber(string fileName, string accountNumber)
{
	vector<stClient> clients = LoadClientsDataFromFile(fileName);
	stClient client;
	return SearchForClientByAccountNumber(client, accountNumber, clients);
}

stClient readNewClient()
{

	stClient client;
	client.accountNumber = readNonEmptyString("Enter Account Number? ");

	while (ClientExistsByAccountNumber(clientsFileName, client.accountNumber))
	{
		cout << "\nClient With Account Number [" << client.accountNumber << "] Already Exists, Enter Another Account Number ? ";
		client.accountNumber = readNonEmptyString("");
	}
	client.pinCode = readNonEmptyString("Enter PinCode: ");
	client.name = readNonEmptyString("Enter Name: ");
	client.phone = readNonEmptyString("Enter Phone: ");
	client.accountBalance = readNonNegativeNumber("Enter AccountBalance: ");
	return client;
}

void addNewClient()
{
	stClient client;

	client = readNewClient();

	addClientToFile(clientsFileName, convertRecordToLine(client));
}

void addNewClients()
{

	char addMoreClients = 'Y';
	do
	{
		cout << "Adding New Client:\n\n";
		addNewClient();
		cout << "\nClient Added Successfully, Do You Want To Add More Clients Y/N? ";
		cin >> addMoreClients;

	} while (addMoreClients == 'Y' || addMoreClients == 'y');

}

void showAddNewClientsScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "         Add New Clients Screen\n";
	cout << "--------------------------------------------\n";
	addNewClients();
}

void printClientData(const stClient& client)
{
	cout << "\nThe Following are the details of the client with account number [" << client.accountNumber << "]:\n\n";
	cout << "--------------------------------------------\n";
	cout << "Account Number : " << client.accountNumber << endl;
	cout << "Pin Code : " << client.pinCode << endl;
	cout << "Name : " << client.name << endl;
	cout << "Phone : " << client.phone << endl;
	cout << "Account Balance : " << client.accountBalance << endl;
	cout << "--------------------------------------------\n";
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& clientsData)
{
	for (stClient& C : clientsData)
	{
		if (C.accountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

void saveClientToFile(string fileName, vector<stClient>& clientsData)
{
	fstream myFile;
	myFile.open(fileName, ios::out);
	string DataLine;
	if (myFile.is_open())
	{
		for (const stClient& C : clientsData)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = convertRecordToLine(C);
				myFile << DataLine << endl;
			}
		}
		myFile.close();
	}
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>clientsData)
{
	stClient Client;
	char DelClient = 'Y';

	if (SearchForClientByAccountNumber(Client, AccountNumber, clientsData))
	{

		printClientData(Client);
		cout << "--------------------------------------------\n";
		cout << "\nAre you sure you want to delete this client? (Y/N): ";
		cin >> DelClient;
		if (DelClient == 'Y' || DelClient == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, clientsData);
			saveClientToFile(clientsFileName, clientsData);

			cout << "\n\nClient Deleted Successfully.\n\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient With Account Number (" << AccountNumber << ") Not Found!\n";
	}
	return false;
}

void showDeleteClientScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "\tDelete Client Screen\n";
	cout << "\n--------------------------------------------\n";
	string AccountNumber = readAccountNumber();
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);
	DeleteClientByAccountNumber(AccountNumber, clientsData);
}

stClient changeClientData(string AccountNumber)
{
	stClient client;
	client.accountNumber = AccountNumber;
	client.pinCode = readNonEmptyString("Enter PinCode: ");
	client.name = readNonEmptyString("Enter Name: ");
	client.phone = readNonEmptyString("Enter Phone: ");
	client.accountBalance = readNonNegativeNumber("Enter AccountBalance: ");
	return client;
}

bool updateClientByAccountNumber(string AccountNumber, vector<stClient>clientsData)
{

	stClient Client;
	char DelClient = 'Y';

	if (SearchForClientByAccountNumber(Client, AccountNumber, clientsData))
	{
		printClientData(Client);
		cout << "--------------------------------------------\n";
		cout << "\nAre you sure you want to Update this client? (Y/N): ";
		cin >> DelClient;
		if (DelClient == 'Y' || DelClient == 'y')
		{
			for (stClient& C : clientsData)
			{
				if (C.accountNumber == AccountNumber)
				{
					C = changeClientData(AccountNumber);
					break;
				}
			}
			saveClientToFile(clientsFileName, clientsData);

			cout << "\n\nClient Updated Successfully.\n\n";
			return true;
		}
	}
	else
	{
		cout << "\nClient With Account Number (" << AccountNumber << ") Not Found!\n";
	}
	return false;
}

void showUpdateClientScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "         Update Client Info Screen\n";
	cout << "--------------------------------------------\n";
	string AccountNumber = readAccountNumber();
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);
	updateClientByAccountNumber(AccountNumber, clientsData);
}

void showFindClientScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "\tFind Client Screen\n";
	cout << "--------------------------------------------\n";
	string AccountNumber = readAccountNumber();
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);
	stClient client;
	if (SearchForClientByAccountNumber(client, AccountNumber, clientsData))
	{
		printClientData(client);
	}
	else
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found!\n";
	}
}

short readMainMenuOption()
{
	cout << "Choose What Do You Want To Do ? [1 to 7] ? ";
	short choice = 0;
	cin >> choice;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(10000, '\n');
		choice = 0;
	}
	return choice;
}

short readTransactionsMenuOption()
{
	cout << "Choose What Do You Want To Do ? [1 to 4] ? ";
	short choice = 0;
	cin >> choice;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(10000, '\n');
		choice = 0;
	}
	return choice;
}

void showEndScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "\tProgram Ends :-)\n";
	cout << "--------------------------------------------\n";
}

void goBackToMainMenu()
{
	cout << "\n\nPress Any Key To Go Back To Main Menu... ";
	system("pause>0");
	showMainMenuScreen();
}

void goBackToTransactionsMenu()
{
	cout << "\n\nPress Any Key To Go Back To Transactions Menu... ";
	system("pause>0");
	showTransactionsMenuScreen();
}

bool updateBalanceToClientByAccountNumber(vector<stClient>& clientsData, double amount, string accountNumber)
{

	char Transaction = 'Y';
	cout << "\n\nAre You Sure You Want Perform This Transaction ? [Y/N] ? ";
	cin >> Transaction;
	if (Transaction == 'Y' || Transaction == 'y')
	{
		for (stClient& C : clientsData)
		{
			if (C.accountNumber == accountNumber)
			{

				C.accountBalance += amount;
				saveClientToFile(clientsFileName, clientsData);
				cout << "\n\nDone Successfully, New Balance is : " << C.accountBalance << " .\n";
				return true;
			}
		}
	}
	else
	{
		cout << "Transaction Cancelled\n";
	}
	return false;
}

void showDepositScreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "\tDeposit Screen\n";
	cout << "--------------------------------------------\n";
	string AccountNumber = readAccountNumber();
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);
	stClient client;
	while (!SearchForClientByAccountNumber(client, AccountNumber, clientsData))
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found!\n";
		AccountNumber = readAccountNumber();
	}
	printClientData(client);
	double deposit = readPositiveNumber("Please Enter Deposit Amount ? ");
	updateBalanceToClientByAccountNumber(clientsData, deposit, AccountNumber);
}

void showWithdrawScreen()
{
	cout << "\n--------------------------------------\n";
	cout << "\tWithdraw Screen\n";
	cout << "----------------------------------------\n";
	string AccountNumber = readAccountNumber();
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);
	stClient client;
	while (!SearchForClientByAccountNumber(client, AccountNumber, clientsData))
	{
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found!\n";
		AccountNumber = readAccountNumber();
	}
	printClientData(client);
	double withdraw = readPositiveNumber("Please Enter Withdraw Amount ? ");
	while (withdraw > client.accountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << client.accountBalance << endl;
		withdraw = readPositiveNumber("Please enter another amount? ");
	}
	updateBalanceToClientByAccountNumber(clientsData, withdraw * -1, AccountNumber);

}

void printClientRecordBalanceLine(const stClient& Client)
{
	cout << "| " << setw(15) << left << Client.accountNumber;
	cout << "| " << setw(40) << left << Client.name;
	cout << "| " << setw(12) << left << Client.accountBalance;
}

void showTotalBalancesScreen()
{
	vector<stClient> clientsData = LoadClientsDataFromFile(clientsFileName);

	cout << "\n\t\t\t\tBalances List (" << clientsData.size() << "): Client(s).";
	cout << "\n______________________________________________________________________________________________________\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n______________________________________________________________________________________________________\n";
	double totalBalance = 0;
	if (clientsData.size() == 0)

		cout << "\t\t\tNo Clients Available In The System!";

	else

		for (const stClient& Client : clientsData)
		{
			printClientRecordBalanceLine(Client);
			totalBalance += Client.accountBalance;
			cout << endl;
		}

	cout << "\n______________________________________________________________________________________________________\n\n";
	cout << "\n\t\t\t\tTotal Balance = " << totalBalance << endl;
}

void performTransactionsMenuOption(enTransactionsMenuOptions transactionsMenuOption)
{
	switch (transactionsMenuOption)
	{
	case enTransactionsMenuOptions::eDeposit:
		system("cls");
		showDepositScreen();
		goBackToTransactionsMenu();
		break;

	case enTransactionsMenuOptions::eWithdraw:
		system("cls");
		showWithdrawScreen();
		goBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eTotalBalances:
		system("cls");
		showTotalBalancesScreen();
		goBackToTransactionsMenu();
		break;
	case enTransactionsMenuOptions::eMainMenu:
		system("cls");
		showMainMenuScreen();
		break;
	default:
		cout << "\nInvalid choice!\n";
		goBackToTransactionsMenu();
		break;
	}
}

void showTransactionsMenuScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "===========================================\n";
	performTransactionsMenuOption((enTransactionsMenuOptions)readTransactionsMenuOption());
}

void performMainMenuOption(enMainMenuOptions MainMenuOption)
{
	switch (MainMenuOption)
	{
	case enMainMenuOptions::eListClients:
		system("cls");
		showAllClientsScreen();
		goBackToMainMenu();
		break;

	case enMainMenuOptions::eAddNewClient:
		system("cls");
		showAddNewClientsScreen();
		goBackToMainMenu();
		break;
	case enMainMenuOptions::eDeleteClient:
		system("cls");
		showDeleteClientScreen();
		goBackToMainMenu();
		break;
	case enMainMenuOptions::eUpdateClient:
		system("cls");
		showUpdateClientScreen();
		goBackToMainMenu();
		break;
	case enMainMenuOptions::eFindClient:
		system("cls");
		showFindClientScreen();
		goBackToMainMenu();
		break;
	case enMainMenuOptions::eTransactions:
		system("cls");
		showTransactionsMenuScreen();
		break;
	case enMainMenuOptions::eExit:
		system("cls");
		showEndScreen();
		break;
	default:
		cout << "\nInvalid choice!\n";
		goBackToMainMenu();
		break;
	}
}

void showMainMenuScreen()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";
	performMainMenuOption((enMainMenuOptions)readMainMenuOption());
}

int main()
{
	showMainMenuScreen();
	system("pause>0");
	return 0;
}


