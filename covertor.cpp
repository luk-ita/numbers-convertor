#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
using namespace std;

// ANSI Color Codes
const string RESET   = "\033[0m";
const string RED     = "\033[31m";
const string GREEN   = "\033[32m";
const string YELLOW  = "\033[33m";
const string CYAN    = "\033[36m";
const string PINK    = "\033[95m";

// Helper: Convert character ('0'-'9', 'A'-'F', 'a'-'f') to its integer value (0-15)
int charToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

// Helper: Convert integer value (0-15) to its character representation
char valueToChar(int val) {
    if (val >= 0 && val <= 9) return '0' + val;
    if (val >= 10 && val <= 15) return 'A' + (val - 10);
    return '0';
}

// Safe integer input function (prevents infinite loops when letters are typed)
int getSafeInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << RED << "Invalid input! Please enter a valid integer.\n" << RESET;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Convert any base (2-16) string (with optional fraction) to decimal double
double into_decimal_fraction(string N_str, int B1) {
    size_t dotPos = N_str.find('.');
    string intPart = (dotPos == string::npos) ? N_str : N_str.substr(0, dotPos);
    string fracPart = (dotPos == string::npos) ? "" : N_str.substr(dotPos + 1);

    // 1. Integer part conversion
    long long decInt = 0;
    long long multiplier = 1;
    for (int i = intPart.length() - 1; i >= 0; i--) {
        int val = charToValue(intPart[i]);
        decInt += val * multiplier;
        multiplier *= B1;
    }

    // 2. Fractional part conversion
    double decFrac = 0.0;
    double divisor = B1;
    for (size_t i = 0; i < fracPart.length(); i++) {
        int val = charToValue(fracPart[i]);
        decFrac += val / divisor;
        divisor *= B1;
    }

    return decInt + decFrac;
}

// Convert decimal double to any base (2-16) string with fraction support
string into_base_fraction(double N, int B2, int precision = 6) {
    long long intPart = (long long)N;
    double fracPart = N - intPart;

    // Convert integer part
    string intStr = "";
    if (intPart == 0) {
        intStr = "0";
    } else {
        long long tempInt = intPart;
        while (tempInt > 0) {
            intStr = valueToChar(tempInt % B2) + intStr;
            tempInt /= B2;
        }
    }

    // Convert fractional part by multiplying
    string fracStr = "";
    while (fracPart > 0 && fracStr.length() < precision) {
        fracPart *= B2;
        int digit = (int)fracPart;
        fracStr += valueToChar(digit);
        fracPart -= digit;
    }
    if (fracStr.empty()) {
        return intStr;
    }
    return intStr + "." + fracStr;
}
// Validate that every character in the string (ignoring the dot) is valid for the given base
bool check_fraction(string N_str, int B) {
    int dotCount = 0;
    for (char c : N_str) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false; // Max one decimal point allowed
            continue;
        }
        int val = charToValue(c);
        if (val == -1 || val >= B) {
            return false;
        }
    }
    return true;
}
int main() {
    string n_str;
    int B1, B2, base, choice, continuing;
    
    cout << PINK << "=== ADVANCED FRACTIONAL BASE CONVERTER (BASES 2-16) ===" << RESET << endl;
    do {
        do {
            choice = getSafeInt("\n" + YELLOW + "1-" + RESET + " A base to decimal \n" + YELLOW + "2-" + RESET + " From decimal to a base \n" + YELLOW + "3-" + RESET + " From base1 to base2\nEnter your choice: ");
            if (choice < 1 || choice > 3) {
                cout << RED << "Invalid choice! Please enter 1, 2, or 3.\n" << RESET;
            }
        } while (choice < 1 || choice > 3);
        if (choice == 2) {
            double decimal_n;
            cout << "Enter the decimal number (can include fractions, e.g., 10.25): ";
            cin >> decimal_n;
            do {
                base = getSafeInt("Enter the target base (2 to 16): ");
                if (base < 2 || base > 16) {
                    cout << RED << "Base must be between 2 and 16!\n" << RESET;
                }
            } while (base < 2 || base > 16);
            cout << GREEN << "Result: " << into_base_fraction(decimal_n, base) << RESET << endl;
        }
        else if (choice == 1) {
            cout << "Enter the number (use A-F for digits 10-15, use '.' for fractions): ";
            cin >> n_str;
            do {
                base = getSafeInt("Enter its current base (2 to 16): ");
                if (base < 2 || base > 16 || !check_fraction(n_str, base)) {
                    cout << RED << "Invalid base or number contains digits too large for this base!\n" << RESET;
                }
            } while (base < 2 || base > 16 || !check_fraction(n_str, base));
            cout << GREEN << "Result: " << into_decimal_fraction(n_str, base) << RESET << endl;
        }
        else {
            cout << "Enter the number (use A-F for digits 10-15, use '.' for fractions): ";
            cin >> n_str;
            
            do {
                B1 = getSafeInt("Enter its current base (2 to 16): ");
                if (B1 < 2 || B1 > 16 || !check_fraction(n_str, B1)) {
                    cout << RED << "Invalid base or number contains digits too large for this base!\n" << RESET;
                }
            } while (B1 < 2 || B1 > 16 || !check_fraction(n_str, B1));           
            do {
                B2 = getSafeInt("Enter the wanted target base (2 to 16): ");
                if (B2 < 2 || B2 > 16) {
                    cout << RED << "Target base must be between 2 and 16!\n" << RESET;
                }
            } while (B2 < 2 || B2 > 16);
            double dec = into_decimal_fraction(n_str, B1);
            cout << GREEN << "Result: " << into_base_fraction(dec, B2) << RESET << endl;
        }
        do {
            continuing = getSafeInt("\nDo you want to continue? (" + YELLOW + "0 for no" + RESET + ", " + GREEN + "1 for yes" + RESET + "): ");
            if (continuing < 0 || continuing > 1) {
                cout << RED << "Please enter 0 or 1.\n" << RESET;
            }
        } while (continuing < 0 || continuing > 1);
    } while (continuing == 1);
    cout << CYAN << "Goodbye!" << RESET << endl;
    cin.get();
}