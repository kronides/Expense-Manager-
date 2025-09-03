#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>  // for stringstream

using namespace std;

struct Expense 
{
    int ID;
    string category;
    double amount;
    string date;
    string note;
};

int main() 
{
    vector<Expense> expenses;
    string command;
    int nextID = 1;

    cout << "Welcome to the Expense Manager!\n";
    cout << "Commands: Add, Show Expenses, delete, quit\n";

    // --- Create file with header if it doesn't exist or is empty ---
    ifstream checkFile("expenses.csv");
    bool fileExists = checkFile.good();
    bool fileEmpty = false;

    if (fileExists) {
        checkFile.seekg(0, ios::end);
        if (checkFile.tellg() == 0) {
            fileEmpty = true;  // file exists but is empty
        }
    }
    checkFile.close();

    if (!fileExists || fileEmpty) {
        ofstream newFile("expenses.csv");
        newFile << "ID | Category | Amount | Date | Note" << endl;
        newFile.close();
    }

    // --- Load existing expenses from file ---
    ifstream infile("expenses.csv");
    string line;

    // Skip header line
    getline(infile, line);

    while (getline(infile, line)) {
        Expense e;
        string idStr, amountStr;

        stringstream ss(line);

        getline(ss, idStr, '|');
        getline(ss, e.category, '|');
        getline(ss, amountStr, '|');
        getline(ss, e.date, '|');
        getline(ss, e.note, '|');

        // Trim whitespace (because of " | " spacing)
        auto trim = [](string &s) {
            if (!s.empty() && s.front() == ' ') s.erase(0, 1);
            if (!s.empty() && s.back() == ' ') s.pop_back();
        };

        trim(idStr);
        trim(e.category);
        trim(amountStr);
        trim(e.date);
        trim(e.note);

        if (!idStr.empty()) e.ID = stoi(idStr);
        if (!amountStr.empty()) e.amount = stod(amountStr);

        expenses.push_back(e);

        if (e.ID >= nextID) {
            nextID = e.ID + 1;
        }
    }
    infile.close();

    while (true) {
        cout << "\n Enter Command: ";
        getline(cin, command);

        if (command == "Add") {
            Expense e;
            e.ID = nextID++;

            cout << "Enter Category: ";
            getline(cin, e.category);

            cout << "Enter Amount: ";
            cin >> e.amount;
            cin.ignore();

            cout << "Enter Date: ";
            getline(cin, e.date);

            cout << "Enter Note: ";
            getline(cin, e.note);

            expenses.push_back(e);

            ofstream file("expenses.csv", ios::app);
            file << e.ID << " | " << e.category << " | "
                 << e.amount << " | " << e.date << " | " << e.note << endl;
            file.close();

            cout << "Expense added and saved!\n";
        }
        else if (command == "Show Expenses") {
            cout << "\nYour Expenses:\n";
            for (auto &e : expenses) {
                cout << e.ID << " | " << e.category << " | "
                     << e.amount << " | " << e.date << " | " << e.note << endl;
            }
        }
        else if (command == "delete") {
            cout << "Enter the ID of the expense to delete: ";
            int deleteID;
            cin >> deleteID;
            cin.ignore();

            bool found = false;
            for (auto it = expenses.begin(); it != expenses.end(); ++it) {
                if (it->ID == deleteID) {
                    expenses.erase(it);
                    found = true;
                    break;
                }
            }

            if (found) {
                // Rewrite file with updated expenses
                ofstream file("expenses.csv");
                file << "ID | Category | Amount | Date | Note" << endl;
                for (auto &e : expenses) {
                    file << e.ID << " | " << e.category << " | "
                         << e.amount << " | " << e.date << " | " << e.note << endl;
                }
                file.close();
                cout << "Expense deleted successfully.\n";
            } else {
                cout << "Expense with ID " << deleteID << " not found.\n";
            }
        }
        else if (command == "quit") {
            cout << "Goodbye!\n";
            break;
        }
        else {
            cout << "Unknown Command.";
        }
    }

    return 0;
}