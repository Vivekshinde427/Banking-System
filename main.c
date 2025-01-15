#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_TRANSACTIONS 100

typedef struct {
    int transactionID;
    char type[11]; 
    float amount;
} Transaction;

typedef struct {
    int accountNumber;
    char name[MAX_NAME_LENGTH];
    float balance;
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

void createAccount();
void viewAccount();
void deposit();
void withdraw();
void viewTransactionHistory();
void saveData();
void loadData();
void displayMenu();

int main() {
    loadData();

    int choice;
    do {
        displayMenu();
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) { 
            printf("Invalid input. Exiting...\n");
            break;
        }
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                viewAccount();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 5:
                viewTransactionHistory();
                break;
            case 6:
                saveData();
                printf("Data saved successfully. Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}

void displayMenu() {
    printf("\nBanking System Menu:\n");
    printf("1. Create Account\n");
    printf("2. View Account\n");
    printf("3. Deposit\n");
    printf("4. Withdraw\n");
    printf("5. View Transaction History\n");
    printf("6. Exit\n");
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("\nMaximum account limit reached!\n");
        return;
    }

    Account newAccount;
    printf("\nEnter account number: ");
    if (scanf("%d", &newAccount.accountNumber) != 1 || newAccount.accountNumber <= 0) {
        printf("Invalid account number!\n");
        return;
    }
    getchar(); // Clear buffer
    printf("Enter account holder's name: ");
    fgets(newAccount.name, MAX_NAME_LENGTH, stdin);
    newAccount.name[strcspn(newAccount.name, "\n")] = '\0'; 
    newAccount.balance = 0.0;
    newAccount.transactionCount = 0;

    accounts[accountCount++] = newAccount;
    printf("Account created successfully!\n");
}

void viewAccount() {
    int accountNumber;
    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid input!\n");
        return;
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("\nAccount Number: %d\n", accounts[i].accountNumber);
            printf("Account Holder: %s\n", accounts[i].name);
            printf("Balance: %.2f\n", accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
}

void deposit() {
    int accountNumber;
    float amount;

    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid input!\n");
        return;
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Enter deposit amount: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount!\n");
                return;
            }

            accounts[i].balance += amount;
            Transaction t = {accounts[i].transactionCount + 1, "Deposit", amount};
            accounts[i].transactions[accounts[i].transactionCount++] = t;

            printf("Deposit successful! New balance: %.2f\n", accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
}

void withdraw() {
    int accountNumber;
    float amount;

    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid input!\n");
        return;
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("Enter withdrawal amount: ");
            if (scanf("%f", &amount) != 1 || amount <= 0 || amount > accounts[i].balance) {
                printf("Invalid or insufficient balance!\n");
                return;
            }

            accounts[i].balance -= amount;
            Transaction t = {accounts[i].transactionCount + 1, "Withdrawal", amount};
            accounts[i].transactions[accounts[i].transactionCount++] = t;

            printf("Withdrawal successful! New balance: %.2f\n", accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
}

void viewTransactionHistory() {
    int accountNumber;

    printf("\nEnter account number: ");
    if (scanf("%d", &accountNumber) != 1) {
        printf("Invalid input!\n");
        return;
    }

    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accountNumber) {
            printf("\nTransaction History for Account %d:\n", accounts[i].accountNumber);
            printf("%-5s %-10s %-10s\n", "ID", "Type", "Amount");
            for (int j = 0; j < accounts[i].transactionCount; j++) {
                printf("%-5d %-10s %.2f\n", accounts[i].transactions[j].transactionID, accounts[i].transactions[j].type, accounts[i].transactions[j].amount);
            }
            return;
        }
    }
    printf("Account not found!\n");
}

void saveData() {
    FILE *file = fopen("accounts.dat", "wb");
    if (!file) {
        printf("Error saving data!\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

void loadData() {
    FILE *file = fopen("accounts.dat", "rb");
    if (!file) return;
    if (fread(&accountCount, sizeof(int), 1, file) != 1) {
        printf("Error loading data!\n");
        fclose(file);
        return;
    }
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}
