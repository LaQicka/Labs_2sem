#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <windows.h>
#include <algorithm>
using namespace std;

struct Bank {
    int daily_lim;
    int onet_lim;
    int bill_lim;
    int balance;
    vector <pair<int, int>> amount; // { {100, 10}, { 200,10 }, { 500,0 }, { 1000,0 }, { 5000,0 } };  // база данных купюр
    vector <int> st;
};

Bank bank;
map <string, int> base; // база данных клиентов
string test_pin = "123"; // пин-код полученный программой от клиента

bool test_run() {
    ifstream test("clients.txt");
    if (!test) return 0;
    test.close();
    test.open("bank_info.txt");
    if (!test) return 0;
    test.close();
    test.open("bills.txt");
    if (!test) return 0;
    return 1;
}

void init_clients() {
    ifstream input("clients.txt");
    string t_pin;
    while (t_pin != "end") {
        int t_balance;
        input >> t_pin >> t_balance;
        if (t_pin == "end") break;
        else base[t_pin] = t_balance;
    }
}// инициализация базы данных клиентов

void init_bank() {
    ifstream input("bank_info.txt");
    input >> bank.daily_lim >> bank.onet_lim >> bank.bill_lim >> bank.balance;
    int temp;
    input >> temp;
    while (temp) {
        bank.st.push_back(temp);
        input >> temp;
    }
    sort(bank.st.begin(), bank.st.end());
} // инициализация банковских параметров и стандартных сумм 

void init_bills(bool flag) {
    if (!flag) {
        ifstream input("bills.txt");
        int t_bill, t_amount;
        input >> t_bill >> t_amount;
        while (t_bill) {
            pair <int, int> buff;
            buff.first = t_bill, buff.second = t_amount;
            bank.amount.push_back(buff);
            input >> t_bill >> t_amount;
        }
    }
    else {
        int temp, t;
        bank.amount.clear();
        cout << "Enter new standard amount different from 0 and number of bills\n";
        cin >> temp >> t;
        pair<int, int> buff;
        while (temp) {
            buff.first = temp;
            buff.second = t;
            bank.amount.push_back(buff);
            cout << "Enter new standard amount different from 0 and number of bills\n";
            cin >> temp;
            if (temp) cin >> t;
        }
    }
    sort(bank.amount.begin(), bank.amount.end());
} // инициализация купюр и их количества

int check(int t, string pin) {
    int flag = 0;
    if (t > bank.balance) flag = 1; // "too large amount for bank\n";
    else if (t > bank.onet_lim) flag = 2; //"too large amount for one time receive\n";
    else if (t > bank.daily_lim) flag = 3; //"too large amount for currently day\n";
    else if (t > base[pin]) flag = 4; // "you dont have enough money\n";
    else if (!flag) {
        int b_lim = bank.bill_lim, count = 0, temp = 0;
        for (int i = bank.amount.size() - 1; i >= 0; i--) {
            count = 0;
            while (b_lim && temp < t && count < bank.amount[i].second) {
                temp += bank.amount[i].first;
                count++;
                b_lim--;
            }
        }
        if (b_lim < 0 || temp < t) flag = 5; // "bank couldnt give you this summ because of lack of bills\n";
    }
    return flag;
} // функция проверки суммы на вохможность быть выданной

void give_cash(int t,bool type) {
    vector <int> ar(bank.amount.size());
    int bills = 0;
    for (int i = bank.amount.size()-1; i >= 0 && t; i--) {
        while (bank.amount[i].first <= t && bank.amount[i].second) {
            ar[i]++;
            bills++;
            bank.amount[i].second--;
            t -= bank.amount[i].first;
        }
    }
    if (type) {
        vector <int> c(bank.amount.size());
        c[0] = 1;
        bool flag = 1;
        for (int i = 1; i < ar.size(); i++) c[i] = bank.amount[i].first / bank.amount[0].first;
        for (int j = ar.size() - 1; j > 1 && flag; j--) {
            for (int i = j - 1; i > 0 && flag; i--) {
                if (ar[j] && !(c[j] % c[i]) && bills + (c[j] / c[i]) - 1 < bank.bill_lim && ar[i - 1] + (c[j] / c[i]) <= bank.amount[i].second) {
                    ar[j]--;
                    ar[i] += c[j] / c[i];
                    bank.amount[i].second -= c[j] / c[i];
                    flag = 0;
                }
            }
        }
    }
    for (int i = ar.size() - 1; i >= 0; i--) {
        if (ar[i]) cout << ar[i] << " bills of " << bank.amount[i].first << " were issued\n";
    }
}// функция выдачи денег пользователю

void st_amount(string pin) {
    cout << "choose standart amount" << '\n';
    for (int i = 0; i < bank.st.size(); i++) {
        if (!check(bank.st[i], pin)) cout << bank.st[i] << " Enter " << i << '\n';
    }
    int i;
    cin >> i;
    cout << "Enter type of receiving - [0 - max][1 - exangeable]\n";
    bool type;
    cin >> type;
    give_cash(bank.st[i], type);
    base[pin] -= bank.st[i];
} // функция выдачи стандартных сумм 

void ct_amount(string pin) {
    cout << "Enter amount which you want to receive\n";
    int t;
    cin >> t;
    while (check(t, pin)) {
        switch (check(t, pin)) {
        case(1):
            cout << "too large amount for bank\n";
            break;
        case(2):
            cout << "too large amount for one time receive\n";
            break;
        case(3):
            cout << "too large amount for currently day\n";
            break;
        case(4):
            cout << "you dont have enough money\n";
            break;
        case(5):
            cout << "bank couldnt give you this summ because of lack of bills\n";
            break;
        }
        cout << "Enter other amount\n";
        cin >> t;
    }
    cout << "choose type of receiving [0 - max][1 - exchangeable]\n";
    bool type;
    cin >> type;
    give_cash(t, type);
    base[pin] -= t;
} // функция выдачи пользовательской суммы

void main_menu() { // функция вывода главного меню
    //cout << "Enter you pin\n";
    string pin = test_pin;
    //cin >> pin;
    cout << "Enter\n[1 - to get your own amount]\n[2 - to get standard amount]\n[3 - to check balance]\n[0 - to exit]\n";
    int t;
    cin >> t;
    while (t) {
        switch (t)
        {
        case(1): {
            ct_amount(pin);
            break;
        }
        case(2): {
            st_amount(pin);
            break;
        }
        case(3): {
            cout << base[pin] << '\n';
            break;
        }
        }
        system("pause");
        system("cls");
        cout << "Enter\n[1 - to get your own amount]\n[2 - to get standard amount]\n[3 - to check balance]\n[0 - to exit]\n";
        cin >> t;
    }
}

int main() {
    if (!test_run()) {
        cout << "Programme emergency stopped because of error with source files\n";
        return 0;
    }
    init_bills(0);
    init_bank();
    init_clients();
    main_menu();
}
