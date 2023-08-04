#include<iostream>
#include <iomanip>
#include <Vector>
#include <fstream>
#include <cctype>
#include <string>
using namespace std;
const string ClientFile = "Record.txt";
const string UserFile = "Users.txt";
struct stClient
{
	string AccoutNumber;
	string PinCode;
	string FullName;
	string PhoneNumber;
	int AccountBalance;
	bool MarkClientForDelete = false;
};
struct stUsers
{
	string UserName;
	string UserPassword;
	int UserPermission;
	bool MarkForUserDelete = false;
};
void LoginScreen();
void BankSystem();
void TransactionExtension();
void ManageUserExtension();

stUsers CurrentUser;
enum enChoice {enShowList = 1 , enAddNew = 2 , enDelete = 3 , enUpdate = 4, enFind = 5 , enTransaction =6, enManageUsers = 7,enLogOut = 8 };
enum enTransactionChoice { enDeposit = 1, enWithdraw = 2, enTotalBalances = 3, enMainMenu = 4 };
enum enManageUsersChoice { enListUsers = 1 , enAddNewUser = 2, enDeleteUser = 3 , enUpdateUser = 4 , enFindUser = 5,enMainMenue = 6};
enum enPermission { enAll = -1 , enPerShowList = 1,enPerAddNew = 2,enPerDelete = 4,enPerUpdate = 8,enPerFind = 16,enPerTransaction = 32 , enPerManageUsers = 64};
void Permission(stUsers& User);
bool CheckUserPermission(enPermission Permission)
{
	if (enPermission::enAll == Permission)
	{
		return true;
	}
	if ((CurrentUser.UserPermission & Permission) == Permission)
		return true;
	return false;
}
void ShowAccessDeniedMessage()
{
	system("cls");
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";


	cout << "\n\nPress Any Key To Come Back ... ";
	system("pause > 0 ");
	BankSystem();
}

vector <string> SplitString(string Word, string delim)
{
	string S1;
	short pos;
	vector <string> vString;
	while ((pos = Word.find(delim)) != string::npos)
	{
		S1 = Word.substr(0, pos);
		if (!S1.empty())
			vString.push_back(S1);
		Word.erase(0, pos + delim.length());
	}
	if (!Word.empty())
		vString.push_back(Word);
	return vString;
}

stClient ConvertClientLineToRecord(string Line, string delim = "#//#")
{
	stClient Client;
	vector <string> vString;
	vString = SplitString(Line, delim);
	Client.AccoutNumber = vString[0];
	Client.PinCode = vString[1];
	Client.FullName = vString[2];
	Client.PhoneNumber = vString[3];
	Client.AccountBalance = stoi(vString[4]);
	return Client;
}
stUsers ConvertUserLineToRecord(string Line, string delim = "#//#")
{
	stUsers User;
	vector <string> vString;
	vString = SplitString(Line, delim);
	User.UserName = vString[0];
	User.UserPassword = vString[1];
	User.UserPermission = stoi(vString[2]);
	return User;
}

void ReadClient(stClient& Client)
{
	cout << "Please Enter Client Data :  \n\n";
	cout << "Enter PinCode ? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Full Name ? ";
	getline(cin, Client.FullName);
	cout << "Enter Phone Number ? ";
	getline(cin, Client.PhoneNumber);
	cout << "Enter AccoutBalance ? ";
	cin >> Client.AccountBalance;
	cout << "\n\n";
}


void UpdateClientData(stClient& Client)
{
	
	    cout << "Please Update Client Data :  \n\n";
		cout << "Accout Number : ";
		cout << Client.AccoutNumber<<endl;
		cout << "Enter PinCode ? ";
		getline(cin >> ws, Client.PinCode);
		cout << "Enter Full Name ? ";
		getline(cin, Client.FullName);
		cout << "Enter Phone Number ? ";
		getline(cin, Client.PhoneNumber);
		cout << "Enter AccoutBalance ? ";
		cin >> Client.AccountBalance;
		cout << "\n\n\n\n";
}
void UpdateUserData(stUsers& User)
{

	cout << "Please Update User Data :  \n\n";
	cout << "User Name : ";
	cout << User.UserName << endl;
	cout << "Enter Password ? ";
	getline(cin >> ws, User.UserPassword);
	Permission(User);
	cout << "\n\n\n\n";
}


string ConvertClientRecordToLine(stClient Client, string delim = "#//#")
{
	string S1 = "";
	S1 += Client.AccoutNumber + delim;
	S1 += Client.PinCode + delim;
	S1 += Client.FullName + delim;
	S1 += Client.PhoneNumber + delim;
	S1 += to_string(Client.AccountBalance);
	return S1;

}
string ConvertUserRecordToLine(stUsers Users, string delim = "#//#")
{
	string S1 = "";
	S1 += Users.UserName + delim;
	S1 += Users.UserPassword + delim;
	S1 += to_string(Users.UserPermission);
	return S1;

}

void AddDataToFile(string FileName, string stData)
{
	fstream File;
	File.open(FileName, ios::out | ios::app);
	if (!File.is_open())
	{
		cout << "There is Error ";
		File.close();
	}
	else
	{
		File << stData << endl;
		File.close();
	}
}

bool FindClientByAccountNumber(string accountNumber, vector <stClient> vClient, stClient& Client)
{
	for (stClient& i : vClient)
	{
		if (accountNumber == i.AccoutNumber)
		{
			Client = i;
			return true;
		}
	}
	
	return false;
}
bool FindUserByUserName(string UserName, vector <stUsers> vUser, stUsers& User)
{
	for (stUsers& i : vUser)
	{
		if (UserName == i.UserName)
		{
			User = i;
			return true;
		}
	}

	return false;
}

void AddNewClient(vector <stClient> vClient)
{
	stClient Client;
	cout << "Accout Number ?: ";
	getline(cin >> ws, Client.AccoutNumber);
	while(FindClientByAccountNumber(Client.AccoutNumber, vClient, Client))
	{
		cout << "the " << Client.AccoutNumber << " is exist Try Another one \n\n";

		cout << "Accout Number ?: ";
		getline(cin >> ws, Client.AccoutNumber);
	}
	ReadClient(Client);
	AddDataToFile(ClientFile, ConvertClientRecordToLine(Client));

}

void AddNewUser(vector <stUsers> vUsers)
{
	stUsers User;
	cout << "Enter Username?: ";
	getline(cin >> ws, User.UserName);
	while (FindUserByUserName(User.UserName,vUsers,User))
	{
		cout << "the " << User.UserName << " is exist Try Another one \n\n";
		cout << "Enter Username?: ";
		getline(cin >> ws, User.UserName);
	}
	cout << "Enter Password?: ";
	getline(cin >> ws, User.UserPassword);
	Permission(User);
	AddDataToFile(UserFile , ConvertUserRecordToLine(User));

}


void PrintTransactionTable(int Num)
{
	std::cout << "\n\t\t\t\t\t   Balances List (" << Num << ") Client(s)." << std::endl << std::endl;
	std::cout << "_______________________________________________________________________________________________________________________" << std::endl << std::endl;
	std::cout << "| " << std::setw(20) << std::left << "Account Number";
	std::cout << "| " << std::setw(60) << std::left << "Client Name";
	std::cout << "| " << std::setw(50) << std::left << "Balance" << std::endl;
	std::cout << "_______________________________________________________________________________________________________________________" << std::endl << std::endl;
}

void PrintClientRecordTable(int Num)
{
	cout << "\n\t\t\t\t\t   Client List (" << Num << ") Client(s)." << endl << endl;
	cout << "_______________________________________________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(20) << left << "Account Number";
	cout << "| " << setw(15) << left << "Pin Code";
	cout << "| " << setw(50) << left << "Client Name";
	cout << "| " << setw(15) << left << "Phone";
	cout << "| " << setw(10) << left << "Balance\n";
	cout << "_______________________________________________________________________________________________________________________" << endl << endl;

}
void PrintUserRecordTable(int Num)
{
	cout << "\n\t\t\t\t\t   User List (" << Num << ") User(s)." << endl << endl;
	cout << "___________________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(15) << left << "User Name";
	cout << "| " << setw(15) << left << "User Password";
	cout << "| " << setw(15) << left << "Permisson\n";
	cout << "___________________________________________________________________________________________" << endl << endl;

}

void PrintClientRecord(stClient Client)
{
	cout << "| " << setw(20) << left << Client.AccoutNumber;
	cout << "| " << setw(15) << left << Client.PinCode;
	cout << "| " << setw(50) << left << Client.FullName;
	cout << "| " << setw(15) << left << Client.PhoneNumber;
	cout << "| " << setw(10) << left << Client.AccountBalance;
}
void PrintUserRecord(stUsers User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(15) << left << User.UserPassword;
	cout << "| " << setw(15) << left << User.UserPermission;
}
void PrintRecordBalance(stClient Client)
{
	cout << "| " << setw(20) << left << Client.AccoutNumber;
	cout << "| " << setw(60) << left << Client.FullName;
	cout << "| " << setw(30) << left << Client.AccountBalance;
}

vector <stClient> PutClientFileContentToVector(string FileName = ClientFile)
{
	fstream File;
	stClient Client;
	vector <stClient> vClient;
	File.open(FileName, ios::in);
	if (!File.is_open())
	{
		cout << "There is Error ";
		File.close();
	}
	else
	{
		string Line;
		while (getline(File, Line))
		{
			vClient.push_back(ConvertClientLineToRecord(Line));
		}
		File.close();
	}
	return vClient;
}
vector <stUsers> PutUserFileContentToVector(string FileName = ClientFile)
{
	fstream File;
	stUsers User;
	vector <stUsers> vUsers;
	File.open(FileName, ios::in);
	if (!File.is_open())
	{
		cout << "There is Error ";
		File.close();
	}
	else
	{
		string Line;
		while (getline(File, Line))
		{
			vUsers.push_back(ConvertUserLineToRecord(Line));
		}
		File.close();
	}
	return vUsers;
}
void AddClient(vector <stClient>vClient)
{
	char Answer;
	do
	{
		system("cls");
		AddNewClient(vClient);
		cout << "\n\n Do You Want To Add More ? [y/n] \n";
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}
int GivePermission()
{
	char Answer;
	int Sum = 0;
	cout << "Do you want to give it access to : \n\n";
	cout << "Show List Menu : y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerShowList;
	cout << "Add New Screen : y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerAddNew;
	cout << "Delete Screen : y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerDelete;
	cout << "Update Screen : y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerUpdate;
	cout << "Find Client Screen : y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerFind;
	cout << "Transaction Screen : y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerTransaction;
	cout << "Manage Users Screen: y/n ";
	cin >> Answer;
	if (Answer == towlower('y')) Sum += enPermission::enPerManageUsers;

	return Sum;

}
void Permission(stUsers &User)
{
	char Answer;
	short Sum = 0;
	cout << "Do you want to give full access? y/n? ";
	cin >> Answer;
	if (Answer == towlower('y'))  User.UserPermission =enPermission::enAll;
	else
		User.UserPermission = GivePermission();
}
void AddUser(vector <stUsers> vUsers)
{
	char Answer;
	do
	{
		system("cls");
		AddNewUser(vUsers);
		cout << "\n\nUser Added Successfully Do You Want To Add More ? [y/n] \n";
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}

void PrintAllContent()
{
	system("cls");
	if (!CheckUserPermission(enPermission::enPerShowList))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <stClient> vClient;
	vClient = PutClientFileContentToVector();
	PrintClientRecordTable(vClient.size());
	if (vClient.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	for (stClient& i : vClient)
	{
		PrintClientRecord(i);
		cout << endl;
	}
	cout << "\n\nPress Any Key To Come Back ... ";
	system("pause > 0 ");
	BankSystem();
}

void PrintAllBalances()
{
	system("cls");
	vector <stClient> vClient;
	vClient = PutClientFileContentToVector();
	PrintTransactionTable(vClient.size());
	int Sum = 0;
	for (stClient& i : vClient)
	{
		Sum += i.AccountBalance;
		PrintRecordBalance(i);
		cout << endl;
	}
	cout << "\n\n Total Balances = " << Sum;
	cout << "\n\n\n\nPress Any Key To Come Back ... ";
	system("pause > 0 ");
	TransactionExtension();
}

void PrintClientCard(stClient Client)
{
	cout << " \n\n";
	cout << "Accout Number : ";
	cout << Client.AccoutNumber;
	cout << " \n";
	cout << "PinCode : ";
	cout << Client.PinCode;
	cout << " \n";
	cout << "Full Name : ";
	cout << Client.FullName;
	cout << " \n";
	cout << "Phone Number : ";
	cout << Client.PhoneNumber;
	cout << " \n";
	cout << "AccoutBalance : ";
	cout << Client.AccountBalance;
	cout << "\n\n";
}
void PrintUserCard(stUsers User)
{
	cout << " \n\n";
	cout << "User Name : ";
	cout << User.UserName;
	cout << " \n";
	cout << "Password : ";
	cout << User.UserPassword;
	cout << " \n";
	cout << "Permission : ";
	cout << User.UserPermission;
}


void PutClientVectorToFile(vector <stClient>& vClient , string FileName = ClientFile)
{
	fstream File;
	File.open(FileName, ios::out);
	if (!File.is_open())
	{
		cout << "There is Error";
		File.close();
	}
	else
	{
		for (stClient& i : vClient)
		{
			if (i.MarkClientForDelete == false)
			{
				File << ConvertClientRecordToLine(i) << endl;
				
			}

		}
		File.close();
	}
}
void PutUserVectorToFile(vector <stUsers>& vUsers, string FileName = ClientFile)
{
	fstream File;
	File.open(FileName, ios::out);
	if (!File.is_open())
	{
		cout << "There is Error";
		File.close();
	}
	else
	{
		for (stUsers& i : vUsers)
		{
			if (i.MarkForUserDelete == false)
			{
				File << ConvertUserRecordToLine(i) << endl;

			}

		}
		File.close();
	}
}
bool FindClientByAccountNumber(string accountNumber, vector <stClient> vClient)
{
	for (stClient& i : vClient)
	{
		if (accountNumber == i.AccoutNumber)
		{
			PrintClientCard(i);
			return true;
		}
	}
	cout << "Client Not Found \n";
	return false;
}

bool MarkClientForDelete(string accountNumber, vector <stClient>& vClient)
{
	for (stClient& i : vClient)
	{
		if (accountNumber == i.AccoutNumber)
		{
			i.MarkClientForDelete = true;
			return true;
		}
	}
	return false;
}
bool MarkUserForDelete(string UserName, vector <stUsers>& vUsers)
{
	for (stUsers& i : vUsers)
	{
		if (UserName == i.UserName)
		{
			i.MarkForUserDelete = true;
			return true;
		}
	}
	return false;
}

string ReadAccountNumber()
{
	cout << "Please Enter Account Number : \n";
	string accountNumber;
	cin >> accountNumber;
	return accountNumber;
}
string ReadUserName()
{
	cout << "Please Enter User Name : \n";
	string UserName;
	cin >> UserName;
	return UserName;
}

bool DeleteClientByAccountNumber(vector <stClient> &vClient, string accountNumber)
{
	stClient Client;
	if (FindClientByAccountNumber(accountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		cout << "\nAre You Sure  To Delete this ? : [y/n]  ";
		char Answer;
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkClientForDelete(accountNumber, vClient);
			PutClientVectorToFile(vClient);
			vClient = PutClientFileContentToVector();
			cout << "Delete Succseful !! ";
			return true;
		}

	}
	else
		cout << "No  Client with account number ["<<accountNumber<<"] was found ,";
		return false;
	

}
bool DeleteUserByUserName(vector <stUsers>& vUsers, string UserName)
{
	stUsers User;
	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);
		cout << "\nAre You Sure  To Delete this ? : [y/n]  ";
		char Answer;
		cin >> Answer;
		if (User.UserName == "Admin")
		{
			cout << "You cannot Delete This User,,\n\n";
			return false;
		}
		if (toupper(Answer) == 'Y')
		{
			MarkUserForDelete(UserName, vUsers);
			PutUserVectorToFile(vUsers,UserFile);
			
			cout << "Delete Succseful !! ";
			return true;
		}

	}
	else
	
		cout << "No  User with This User Name [" << UserName << "] was found ,";
		return false;
	

}

bool UpdateClientByAccountNumber(vector <stClient>& vClient, string accountNumber)
{
	stClient Client;
	if (FindClientByAccountNumber(accountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		cout << "\n Are You Sure  To Update this ? : [y/n]  ";
		char Answer;
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stClient& i : vClient)
			{
				if (accountNumber == i.AccoutNumber)
				{
					UpdateClientData(i);
					break;
				
				}
			}
			PutClientVectorToFile(vClient);
			cout << "\n Updated Succesful";
			return true;
		}

	}
	else
	{
		cout << "Not Found";
		return false;
	}

}
bool UpdateUserByUserName(vector <stUsers>& vUsers, string UserName)
{
	stUsers User;
	if (FindUserByUserName(UserName, vUsers, User))
	{
		PrintUserCard(User);
		cout << "\n Are You Sure  To Update this ? : [y/n]  ";
		char Answer;
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stUsers& i : vUsers)
			{
				if (UserName == i.UserName)
				{
					UpdateUserData(i);
					break;

				}
			}
			PutUserVectorToFile(vUsers,UserFile);
			cout << "\n Updated Succesful";
			return true;
		}

	}
	else
	{
		cout << "Not Found";
		return false;
	}

}

void DepositClientByAccountNumber(vector <stClient>& vClient, string accountNumber)
{
	stClient Client;
	while(!FindClientByAccountNumber(accountNumber, vClient, Client))
	{
		cout << "Client with [" << accountNumber << "] does not exist.\n";
		accountNumber = ReadAccountNumber();
	}
		
		PrintClientCard(Client);
		cout << "\n Please enter deposit amount ? : ";
		int Answer;
		cin >> Answer;
		cout << "\n Are you sure you want perform this transaction ? [y/n] ";
		char answer;
		cin >> answer;
		if (toupper(answer) == 'Y')
		{
			for (stClient& i : vClient)
			{
				if (accountNumber == i.AccoutNumber)
				{
					i.AccountBalance += Answer;
					break;

				}
			}
			PutClientVectorToFile(vClient);
			cout << "\n Deposit Succesful";
		}
		
		
}

void WithdrawClientByAccountNumber(vector <stClient>& vClient, string accountNumber)
{
	stClient Client;
	while (!FindClientByAccountNumber(accountNumber, vClient, Client))
	{
		cout << "Client with [" << accountNumber << "] does not exist.\n";
		accountNumber = ReadAccountNumber();
	}

	PrintClientCard(Client);
	cout << "\n Please enter Withdraw amount ? : ";
	int Answer;
	cin >> Answer;
	while (Client.AccountBalance < Answer)
	{
		cout << "Amount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Answer;
	}
	cout << "\n Are you sure you want perform this transaction ? [y/n] ";
	char answer;
	cin >> answer;
	
	if (toupper(answer) == 'Y')
	{
		Client.AccountBalance -= Answer;
		for (stClient& i : vClient)
		{
			if (i.AccoutNumber == Client.AccoutNumber)
				i = Client;
					break;
		}
		PutClientVectorToFile(vClient);
		cout << "\n Withdraw Succesful";
	}
}

void DeleteClientScreen()
{
	if (!CheckUserPermission(enPermission::enPerDelete))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <stClient> vClient = PutClientFileContentToVector();
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\t Delete Client Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	DeleteClientByAccountNumber(vClient, ReadAccountNumber());
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	BankSystem();
}
void DeleteUserScreen()
{
	vector <stUsers> vUsers = PutUserFileContentToVector(UserFile);
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\t Delete Client Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	DeleteUserByUserName(vUsers, ReadUserName());
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	ManageUserExtension();
}

void UpdateClientScreen()
{
	if (!CheckUserPermission(enPermission::enPerUpdate))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <stClient> vClient = PutClientFileContentToVector();
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\t Update Client Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	UpdateClientByAccountNumber(vClient, ReadAccountNumber());
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	BankSystem();
}
void UpdateUserScreen()
{
	vector <stUsers> vUsers = PutUserFileContentToVector(UserFile);
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\t Update User Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	UpdateUserByUserName(vUsers, ReadAccountNumber());
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	ManageUserExtension();
}

void AddClientScreen()
{
	if (!CheckUserPermission(enPermission::enPerAddNew))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <stClient> vClient = PutClientFileContentToVector();
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\tAdd Client Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;

	AddClient(vClient);
	vClient = PutClientFileContentToVector();
	cout << "\n\nPress Any Key To Come Back ... ";
	system("pause > 0 ");
	BankSystem();
}

void FindUserScreen()
{
	vector <stUsers> vUsers = PutUserFileContentToVector(UserFile);
	stUsers User;
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\tFind User Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	FindUserByUserName(ReadUserName(), vUsers,User);
	
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	BankSystem();
}
void FindClientScreen()
{
	if (!CheckUserPermission(enPermission::enPerFind))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <stClient> vClient = PutClientFileContentToVector();
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\tFind Client Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	FindClientByAccountNumber(ReadAccountNumber(), vClient);
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	BankSystem();
}


void LogoutScreen()
{
	system("cls");
	LoginScreen();
	exit(0);
}

void DepositShowScreen()
{
	vector <stClient> vClient = PutClientFileContentToVector();
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\t Deposit Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	DepositClientByAccountNumber(vClient, ReadAccountNumber());
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	TransactionExtension();
}

void WithdrawShowScreen()
{
	vector <stClient> vClient = PutClientFileContentToVector();
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\t Withdraw Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	WithdrawClientByAccountNumber(vClient, ReadAccountNumber());
	cout << "\n\nPress Any Key To Come Back ...";
	system("pause > 0 ");
	TransactionExtension();
}

void ShowTransactionMenu()
{
	system("cls");
	cout << "==================================================================" << endl << endl;
	cout << "\t\t  Transaction Menu Screen \n\n";
	cout << "==================================================================" << endl << endl;
	cout << "\t[1] Deposit." << endl;
	cout << "\t[2] Withdraw." << endl;
	cout << "\t[3] Total Balances." << endl;
	cout << "\t[4] Main Menu." << endl;
	cout << "==================================================================" << endl << endl;
}
void ShowMangeUsersMenu()
{
	system("cls");
	cout << "==================================================================" << endl << endl;
	cout << "\t\t  Manage Menu Screen \n\n";
	cout << "==================================================================" << endl << endl;
	cout << "\t[1] List Users." << endl;
	cout << "\t[2] Add New User." << endl;
	cout << "\t[3] Delete User." << endl;
	cout << "\t[4] Update User." << endl;
	cout << "\t[5] Find User." << endl;
	cout << "\t[6] Main Menu." << endl;
	cout << "==================================================================" << endl << endl;
}

void TransactionExtension()
{
	if (!CheckUserPermission(enPermission::enPerTransaction))
	{
		ShowAccessDeniedMessage();
		return;
	}
	ShowTransactionMenu();
	cout << "Choose What do you Want to do? [1 to 4]? ";
	short Answer;
	cin >> Answer;
	switch ((enTransactionChoice)Answer)
	{
	case enTransactionChoice::enDeposit:
		DepositShowScreen();
		break;
	case enTransactionChoice::enWithdraw:
		WithdrawShowScreen();
		break;
	case enTransactionChoice::enTotalBalances:
		PrintAllBalances();
		break;
	case enTransactionChoice::enMainMenu:
		BankSystem();
		break;
	}
}
void ShowUsersList()
{
	system("cls");
	vector <stUsers> vUsers;
	vUsers = PutUserFileContentToVector("Users.txt");
	PrintUserRecordTable(vUsers.size());
	for (stUsers& i : vUsers)
	{
		PrintUserRecord(i);
		cout << endl;
	}
	cout << "\n\nPress Any Key To Come Back ... ";
	system("pause > 0 ");
	ManageUserExtension();
}
void AddUserScreen()
{
	vector <stUsers> vUsers = PutUserFileContentToVector("Users.txt");
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\tAdd New User Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
	AddUser(vUsers);
	cout << "\n\nPress Any Key To Come Back ... ";
	system("pause > 0 ");
	ManageUserExtension();
}

void ManageUserExtension()
{
	if (!CheckUserPermission(enPermission::enPerManageUsers))
	{
		ShowAccessDeniedMessage();
		return;
	}
	ShowMangeUsersMenu();
	cout << "Choose What do you Want to do? [1 to 6]? ";
	short Answer;
	cin >> Answer;
	switch ((enManageUsersChoice)Answer)
	{
	case enManageUsersChoice::enListUsers:
		ShowUsersList();
		break;
	case enManageUsersChoice::enAddNewUser:
		AddUserScreen();
		break;
	case enManageUsersChoice::enDeleteUser:
		DeleteUserScreen();
		break;
	case enManageUsersChoice::enUpdateUser:
		UpdateUserScreen();
		break;
	case enManageUsersChoice::enFindUser:
		FindUserScreen();
		break;
	case enManageUsersChoice::enMainMenue:
		BankSystem();
		break;
	}
}
void ShowMainMenu()
{
	system("cls");
	cout <<"==================================================================" << endl << endl;
	cout << "\t\t\t\tMenu\n\n";
	cout <<"==================================================================" << endl << endl;
	cout << "\t[1] Show Client List." << endl;
	cout << "\t[2] Add New Client." << endl;
	cout << "\t[3] Delete Client." << endl;
	cout << "\t[4] Update Client Info." << endl;
	cout << "\t[5] Find Client." << endl;
	cout << "\t[6] Transaction." << endl;
	cout << "\t[7] Manage Users." << endl;
	cout << "\t[8] Logout." << endl;
	cout << "==================================================================" << endl << endl;


}

void BankSystem()
{
	
	ShowMainMenu();
	short Answer;
	
	cout << "Choose What do you want to do? [1 to 7]?  ";
	cin >> (Answer);
	switch ((enChoice)Answer)
	{
	case enChoice::enShowList:
	
		PrintAllContent();
	
		break;
	case enChoice::enAddNew:

		AddClientScreen();

		break;
	case enChoice::enDelete:
	
		DeleteClientScreen();

		break;
	case enChoice::enUpdate:
	
		UpdateClientScreen();

		break;
	case enChoice::enFind:

		FindClientScreen();

		break;
	case enChoice::enTransaction:

		TransactionExtension();
		
		break;
	case enChoice::enManageUsers:
		
		ManageUserExtension();
		
		break;
	case enChoice::enLogOut:

		LogoutScreen();

		break;
		
	}
}
stUsers ReadUserLogin()
{
	stUsers User;
	cout << "Enter User Name : ";
	getline(cin >> ws, User.UserName);
	cout << "\nEnter User Password : ";
	getline(cin >> ws, User.UserPassword);
	return User;
}
void DrawLoginScreen()
{
	system("cls");
	cout << "--------------------------------------------------" << endl << endl;
	cout << "\t\tLogin Screen " << endl << endl;
	cout << "--------------------------------------------------" << endl << endl;
}
void LoginScreen()
{
	vector <stUsers> vUsers = PutUserFileContentToVector("Users.txt");
	
	DrawLoginScreen();
	stUsers User = ReadUserLogin();
	while (!FindUserByUserName(User.UserName,vUsers,CurrentUser))
	{
		system("cls");
		DrawLoginScreen();
		cout << "Passower or User Name is invalid please try again !! : \n\n";
		User = ReadUserLogin();

	}
	BankSystem();
}
int main()
{
	LoginScreen();
	
}