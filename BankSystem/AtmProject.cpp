#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;


const string ClientsFileName = "Record.txt";

void ShowMainMenue();
void  Login();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};
sClient CurrrentClient;
enum enATMChoice { QuickWithdraw = 1 , NormalWithdraw = 2 , Deposit = 3,CheckBalance = 4,Logout = 5};
enum enQuickWithdrawChoice { twenty = 1 , fivty = 2 , OneHundred = 3 , TwoHundred = 4 , FourHundred = 5, SixHundred = 6 , EightHundred = 7, OneThousend = 8 , Exit = 9  };
vector<string> SplitString(string S1, string Delim)
{

    vector<string> vString;

    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{

    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double


    return Client;

}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);

            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return vClients;

}
bool ClientExistsByAccountNumberAndPinCode(string AccountNumber, string PinCode,string FileName)
{

    vector <sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {

        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {

            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber && Client.PinCode == PinCode)
            {
                CurrrentClient = Client;
                MyFile.close();
                return true;
            }


            vClients.push_back(Client);
        }

        MyFile.close();

    }

    return false;


}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;

}
void SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;

            }

        }

        MyFile.close();

    }



}
bool DepositBalanceToClientByAccountNumber(double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient& C : vClients)
        {
            if (C.AccountNumber == CurrrentClient.AccountNumber)
            {
                C.AccountBalance += Amount;
                CurrrentClient = C;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }

        }


        return false;
    }

}
void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();
}
void ShowQuickWithDrawScreen(double Amount)
{
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);


    //Validate that the amount does not exceeds the balance
    while (Amount > CurrrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(Amount * -1, vClient);
   
}
void ShowNormalWithdrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\Normal Withdraw Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    int Amount ;
    cout << "\nPlease enter Amoubt multiple of 5's  ? ";
    cin >> Amount;
    while ( (Amount % 5) != 0)
    {
        cout << "\nPlease enter Amoubt multiple of 5's  ? ";
        cin >> Amount;
    }

    DepositBalanceToClientByAccountNumber(Amount * -1, vClients);
    
}
void PerfromQuickWithdrawMenueOption(enQuickWithdrawChoice QuickChoice)
{
    switch (QuickChoice)
    {
    case enQuickWithdrawChoice::twenty:
    {
    
        ShowQuickWithDrawScreen(20);
        GoBackToMainMenue();
    }

    case enQuickWithdrawChoice::fivty:
    {
        ShowQuickWithDrawScreen(50);
        GoBackToMainMenue();
    }


    case enQuickWithdrawChoice::OneHundred:
    {
    
        ShowQuickWithDrawScreen(100);
        GoBackToMainMenue();
    }


    case enQuickWithdrawChoice::TwoHundred:
    {
    
        ShowQuickWithDrawScreen(200);
        GoBackToMainMenue();


    }
    case enQuickWithdrawChoice::FourHundred:
    {

 
        ShowQuickWithDrawScreen(400);
        GoBackToMainMenue();

    }
    case enQuickWithdrawChoice::SixHundred:
    {

       
        ShowQuickWithDrawScreen(600);
        GoBackToMainMenue();

    }
    case enQuickWithdrawChoice::EightHundred:
    {

      
      
        ShowQuickWithDrawScreen(800);
        GoBackToMainMenue();
    }
    case enQuickWithdrawChoice::OneThousend:
    {
       
        ShowQuickWithDrawScreen(1000);
        GoBackToMainMenue();
       

    }
    case enQuickWithdrawChoice::Exit:
    {
        system("cls");
        GoBackToMainMenue();

    }
    }

}
short ReadQuickWithMenueOption()
{
    cout << "Choose what do you want to do? [1 to 9]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}
void ShowQuickQithdrawMenu()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20.\t";
    cout <<"\t\t[2] 50.\n";
    cout << "\t[3] 100.\t";
    cout << "\t[4] 200.\n";
    cout << "\t[5] 400.\t";
    cout << "\t[6] 600.\n";
    cout << "\t[7] 800.\t";
    cout << "\t[8] 1000.\n";
    cout << "\t[9] Exit.\n";
    cout << "===========================================\n";
    PerfromQuickWithdrawMenueOption((enQuickWithdrawChoice)ReadQuickWithMenueOption());
 
}

void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";


    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositBalanceToClientByAccountNumber(Amount, vClients);

}
void ShowCheckBalance()
{
    system("cls");
    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "Your Balance is " << CurrrentClient.AccountBalance << endl << endl;
}
void PerfromATMMenueOption(enATMChoice ATMChoice)
{
    switch (ATMChoice)
    {
    case enATMChoice::QuickWithdraw:
    {
        system("cls");
        ShowQuickQithdrawMenu();
        break;
    }

    case enATMChoice::NormalWithdraw:
    {
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenue();
        break;
    }


    case enATMChoice::Deposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;
    }


    case enATMChoice::CheckBalance:
    {
        ShowCheckBalance();
        GoBackToMainMenue();

    }
    case enATMChoice::Logout:
    {

        Login();

    }
    }

}
short ReadATMMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;

    return Choice;
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";


    PerfromATMMenueOption((enATMChoice)ReadATMMenueOption());
}

void Login()
{
    bool LoginFaild = false;

    string Accountnumber, PinCode;
    do
    {
        system("cls");

        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";

        if (LoginFaild)
        {
            cout << "Invlaid Accountnumber/PinCode!\n";
        }

        cout << "Enter Accountnumber? ";
        cin >> Accountnumber;

        cout << "Enter PinCode? ";
        cin >> PinCode;

        LoginFaild = !ClientExistsByAccountNumberAndPinCode(Accountnumber, PinCode, ClientsFileName);

    } while (LoginFaild);

    ShowMainMenue();

}
int main()
{
    Login();
}
