#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

class User {
protected:
    string name;
    int accountNumber;
    double balance;

public:
    virtual void viewDetails() = 0;
    int getAccountNumber() { return accountNumber; }
};

class Customer : public User {
public:
    Customer(string n, int accNo, double bal) {
        name = n;
        accountNumber = accNo;
        balance = bal;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << ", New Balance: " << balance << endl;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
            return false;
        }
        balance -= amount;
        cout << "Withdrawn: " << amount << ", New Balance: " << balance << endl;
        return true;
    }

    void viewDetails() override {
        cout << "Name: " << name << "\nAccount Number: " << accountNumber << "\nBalance: " << balance << endl;
    }

    string toString() {
        return name + "," + to_string(accountNumber) + "," + to_string(balance);
    }

    double getBalance() { return balance; }
    string getName() { return name; }
};

class Admin : public User {
public:
    void viewAllAccounts(map<int, Customer>& customers) {
        for (auto& [accNo, cust] : customers) {
            cout << "-----------------------" << endl;
            cust.viewDetails();
        }
    }

    void viewDetails() override {
        cout << "Admin has access to all customer accounts.\n";
    }
};

void loadFromFile(map<int, Customer>& customers) {
    ifstream file("accounts.txt");
    if (!file.is_open()) return;

    string name;
    int accNo;
    double balance;

    while (file >> name >> accNo >> balance) {
        customers[accNo] = Customer(name, accNo, balance);
    }

    file.close();
}

void saveToFile(map<int, Customer>& customers) {
    ofstream file("accounts.txt", ios::trunc);
    for (auto& [accNo, cust] : customers) {
        file << cust.getName() << " " << accNo << " " << cust.getBalance() << "\n";
    }
    file.close();
}

int main() {
    map<int, Customer> customers;
    loadFromFile(customers);

    Admin admin;
    int choice;

    while (true) {
        cout << "\n===== Bank Management Menu =====\n";
        cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. View My Account\n5. Admin: View All Accounts\n6. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            int accNo;
            double bal;
            cout << "Enter name: "; cin >> name;
            cout << "Enter account number: "; cin >> accNo;
            cout << "Enter initial balance: "; cin >> bal;
            customers[accNo] = Customer(name, accNo, bal);
            cout << "Account created successfully.\n";

        } else if (choice == 2) {
            int accNo;
            double amount;
            cout << "Enter account number: "; cin >> accNo;
            if (customers.find(accNo) != customers.end()) {
                cout << "Enter amount to deposit: "; cin >> amount;
                customers[accNo].deposit(amount);
            } else {
                cout << "Account not found.\n";
            }

        } else if (choice == 3) {
            int accNo;
            double amount;
            cout << "Enter account number: "; cin >> accNo;
            if (customers.find(accNo) != customers.end()) {
                cout << "Enter amount to withdraw: "; cin >> amount;
                customers[accNo].withdraw(amount);
            } else {
                cout << "Account not found.\n";
            }

        } else if (choice == 4) {
            int accNo;
            cout << "Enter account number: "; cin >> accNo;
            if (customers.find(accNo) != customers.end()) {
                customers[accNo].viewDetails();
            } else {
                cout << "Account not found.\n";
            }

        } else if (choice == 5) {
            admin.viewAllAccounts(customers);

        } else if (choice == 6) {
            saveToFile(customers);
            cout << "Thank you for using the Bank System.\n";
            break;

        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
