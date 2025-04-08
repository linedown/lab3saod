#include <iostream>
#include <time.h>
#include <cmath>
#include <vector>
#include <windows.h>
#include <algorithm>
#include <set>

#define inf 99999

using namespace std;

const double pi = 3.14159;

const double pogreshnost = 1e-9;

HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);

struct tochka {
    int x, y;

    //bool operator > (const tochka& tochka) const {
    //    return sqrt(pow(this->x, 2) + pow(this->y, 2)) > sqrt(pow(tochka.x, 2) + pow(tochka.y, 2));
    //}

    /*bool operator < (const tochka& tochka) const {
        return sqrt(pow(this->x, 2) + pow(this->y, 2)) < sqrt(pow(tochka.x, 2) + pow(tochka.y, 2));
    }*/

    bool operator >= (const tochka& tochka) const {
        return sqrt(pow(this->x, 2) + pow(this->y, 2)) >= sqrt(pow(tochka.x, 2) + pow(tochka.y, 2));
    }

    bool operator <= (const tochka& tochka) const {
        return sqrt(pow(this->x, 2) + pow(this->y, 2)) <= sqrt(pow(tochka.x, 2) + pow(tochka.y, 2));
    }

    tochka operator = (const tochka& tochka) {
        /*if (this == &tochka) {
            return *this;
        }*/
        x = tochka.x;
        y = tochka.y;

        return *this;

    }

    bool operator == (const tochka& tochka) const {
        return this->x == tochka.x && this->y == tochka.y;
    }
};

struct tochkaDouble {
    double x, y;
};

struct otrezok {
    tochkaDouble x1, x2;

    otrezok() {
        this->x1.x = 0;
        this->x1.y = 0;
        this->x2.x = 0;
        this->x2.y = 0;
    }

    otrezok(tochkaDouble a, tochkaDouble b) {
        this->x1.x = a.x;
        this->x1.y = a.y;
        this->x2.x = b.x;
        this->x2.y = b.y;
    }

    double poluch_y(double x) const {
        if (abs(x1.x - x2.x) < pogreshnost) {
            return x1.y;

        }

        return x1.y + (x2.y - x1.y) * (x - x1.x) / (x2.x - x1.x);
    }

    void getTochki() {
        cout << x1.x << " " << x1.y << " " << x2.x << " " << x2.y << "\n";
    }

    bool operator < (const otrezok& s) const {
        int x = max(min(this->x1.x, this->x2.x), min(s.x1.x, s.x2.x));
        return this->poluch_y(x) < s.poluch_y(x);

    }

};

enum { FIRST, SECOND, THIRD, FOURTH, POS_X, POS_Y, NEG_X, NEG_Y, CENTER };

int chetvert(tochka nul, tochka tchk) {
    if (tchk.x > 0 && tchk.y > 0) {
        return FIRST;
    }
    else if (tchk.x < 0 && tchk.y > 0) {
        return SECOND;
    }
    else if (tchk.x < 0 && tchk.y < 0) {
        return THIRD;
    }
    else if (tchk.x > 0 && tchk.y < 0) {
        return FOURTH;
    }
    else if (tchk.x == nul.x && tchk.y > nul.y) {
        return POS_Y;
    }
    else if (tchk.x == nul.x && tchk.y < nul.y) {
        return NEG_Y;
    }

    else if (tchk.y == nul.y && tchk.x > nul.x) {

        return POS_X;
    }

    /*else if (tchk.y == nul.y && tchk.x < nul.x) {
         return NEG_X;
    }*/

    else {
        return CENTER;
    }
}



double ugol(tochka one, tochka two) {
    if (chetvert(one, two) == FIRST || chetvert(one, two) == FOURTH)
        return atan(double(two.y - one.y) / double(two.x - one.x)) * (180 / pi);

    else if (chetvert(one, two) == POS_Y) {
        return 90;
    }
    else if (chetvert(one, two) == NEG_Y) {
        return -90;
    }

    else if (chetvert(one, two) == POS_X) {
        return 0;
    }

    else {
        return -(inf);
    }
}


int ProverkaTochkiC(tochka a, tochka b, tochka c) {
    return (b.x - a.x) * (c.y - b.y) - (b.y - a.y) * (c.x - b.x); // если данное значение < 0, то точка C лежит левее отрезка AB если < 0 - правее, = 0 - лежит на АВ
}

void Sliyanie(tochka* sets, tochka* sets1, int nachalo, int seredina, int konec) {
    int index = nachalo;
    int i = nachalo;
    int j = seredina + 1;

    while (i <= seredina && j <= konec) {
        if (sets[i] <= sets[j]) {
            sets1[index++] = sets[i++];
        }
        else {
            sets1[index++] = sets[j++];
        }
    }

    while (i <= seredina) {
        sets1[index++] = sets[i++];
    }

    for (int f = nachalo; f <= konec; f++) {
        sets[f] = sets1[f];
    }

}

void SortSliyaniem(tochka* sets, tochka* sets1, int nachalo, int konec) {
    if (konec == nachalo) {
        return;
    }

    int seredina = (nachalo + ((konec - nachalo) >> 1));

    SortSliyaniem(sets, sets1, nachalo, seredina);
    SortSliyaniem(sets, sets1, seredina + 1, konec);

    Sliyanie(sets, sets1, nachalo, seredina, konec);
}

void SliyaniePoUglu(tochka* sets, tochka* sets1, int nachalo, int seredina, int konec) {
    int index = nachalo;
    int i = nachalo;
    int j = seredina + 1;

    while (i <= seredina && j <= konec) {
        if (ugol(sets[0], sets[i]) < ugol(sets[0], sets[j])) {
            sets1[index++] = sets[i++];
        }
        else {
            sets1[index++] = sets[j++];
        }
    }

    while (i <= seredina) {
        sets1[index++] = sets[i++];
    }

    for (int f = nachalo; f <= konec; f++) {
        sets[f] = sets1[f];
    }

}

void SortSliyaniemPoUglu(tochka* sets, tochka* sets1, int nachalo, int konec) {
    if (konec == nachalo) {
        return;
    }

    int seredina = (nachalo + ((konec - nachalo) >> 1));

    SortSliyaniemPoUglu(sets, sets1, nachalo, seredina);
    SortSliyaniemPoUglu(sets, sets1, seredina + 1, konec);

    SliyaniePoUglu(sets, sets1, nachalo, seredina, konec);
}

inline bool peresechenieHelp(double x1, double y1, double x2, double y2) {
    if (x1 > y1) {
        swap(x1, y1);
    }
    if (x2 > y2) {
        swap(x2, y2);
    }
    return max(x1, x2) <= min(y1, y2) + pogreshnost;
}

inline int v(const tochkaDouble& a, const tochkaDouble& b, const tochkaDouble& c) {
    double s = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);

    if (abs(s) < pogreshnost) {
        return 0;
    }

    else {
        if (s > 0) {
            return +1;
        }

        else {
            return -1;
        }
    }
}

bool peresechenie(const otrezok& a, const otrezok& b) {
    return peresechenieHelp(a.x1.x, a.x2.x, b.x1.x, b.x2.x) && peresechenieHelp(a.x1.y, a.x2.y, b.x1.y, b.x2.y) && v(a.x1, a.x2, b.x1) * v(a.x1, a.x2, b.x2) <= 0 && v(b.x1, b.x2, a.x1) * v(b.x1, b.x2, a.x2) <= 0;
}

struct sobytie {
    double x;
    int tip, id;

    sobytie() {
        this->x = 0;
        this->tip = 0;
        this->id = 0;
    }
    sobytie(double x, int tip, int id)
    {
        this->x = x;
        this->tip = tip;
        this->id = id;
    }

    bool operator < (const sobytie& e) const {
        if (abs(x - e.x) > pogreshnost) {
            return
                x < e.x;
        }
        return tip > e.tip;
    }
};

set<otrezok> tree;
vector <set<otrezok>::iterator> iterators;

inline set<otrezok>::iterator predydushiy(set<otrezok>::iterator iterator_arg) {
    if (iterator_arg == tree.begin()) {
        return tree.end();
    }

    return --iterator_arg;
}

inline set<otrezok>::iterator sled(set<otrezok>::iterator iterator_arg) {
    return ++iterator_arg;
}

bool bentleyOttman(otrezok** sets, int count) {
    int n = count;
    vector<sobytie> events;
    for (int i = 0; i < n; ++i) {
        events.push_back(sobytie(min(sets[i]->x1.x, sets[i]->x2.x), +1, i));
        events.push_back(sobytie(max(sets[i]->x1.x, sets[i]->x2.x), -1, i));
    }
    sort(events.begin(), events.end());

    iterators.resize(count);
    for (size_t i = 0; i < events.size(); ++i) {
        int id = events[i].id;

        if (events[i].tip == +1) {
            set<otrezok>::iterator next = tree.lower_bound(*sets[id]);
            set<otrezok>::iterator previous = predydushiy(next);

            if (next != tree.end() && peresechenie(*next, *sets[id])) {
                return true;
            }
            if (previous != tree.end() && peresechenie(*previous, *sets[id])) {
                return true;
            }
            iterators[id] = tree.insert(next, *sets[id]);
        }
        else {
            set<otrezok>::iterator next = sled(iterators[id]);
            set<otrezok>::iterator previous = predydushiy(iterators[id]);

            if (next != tree.end() && previous != tree.end() && peresechenie(*next, *previous)) {
                return true;
            }
            tree.erase(iterators[id]);
        }
    }

    tree.clear();
    return false;
}

void colors_menu() {

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "||01||" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|" << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==============";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0||1|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|01|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|01|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|01|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|01|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|0|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|01|";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((2 << 0) | 7));
    cout << "|1|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==========";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "1000100";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "=|0|==|01|==|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "100100011" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=======";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10000100001000";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==|01|=";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "0110011010001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=====";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "101001100010011010";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "0110100011111010101" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "===";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "101011010100101001001001110010101001011001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10010010100100011001001100101001000101010001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "1010101010011101010011010001101010100101001001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10010110001000001011010110010110100100101011011" << "\n";

    cout << "100100001011010111100101010101110010110110101001" << "\n";

    cout << "101011000101100100010010101010101001011011110001" << "\n";

    cout << "100110100111001010001110010110010011100100101101" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "1001110100100010101010001111010011100111100101" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10001010100101011101101000101010101001110001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "====";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "1011000100100100010000110011000100101001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "100010101011001100100101100111001101" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=========";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "101101101001011000101010101101" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "============";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "101100111001101000100101" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==============";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10101100101011011001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "================";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "100110010101001" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "1010100101";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "11010111";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10101";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "110";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|===";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "101";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "==|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|==|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "10";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|==|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "==================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|==|";
    SetConsoleTextAttribute(color, ((4 << 0) | 0));
    cout << "01";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|==|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "===================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|======|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "====================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|====|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=====================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1|==|0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "======================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|1||0|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "=======================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "|10|" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 0));
    cout << "========================";
    SetConsoleTextAttribute(color, ((7 << 0) | 0));
    cout << "||" << "\n";

    SetConsoleTextAttribute(color, ((0 << 0) | 6));
    cout << "|----------------------------------------------------------------------------------------------------------------------|" << "\n\n";
    cout << "|";
    SetConsoleTextAttribute(color, ((0 << 0) | 2));
    cout << "=======================| Добро пожаловать, путник. Выбери свою дорогу, по которой будешь следовать всю следующую жизнь: " << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 6));
    cout << "|";
    SetConsoleTextAttribute(color, ((0 << 0) | 2));
    cout << " => 1. Дорога алгоритма нахождения пересечений отрезков на плоскости " << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 6));
    cout << "|";
    SetConsoleTextAttribute(color, ((0 << 0) | 2));
    cout << " => 2. Путь нахождения множества max / min элементов для множества точек на плоскости(по удалению от центра) " << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 6));
    cout << "|";
    SetConsoleTextAttribute(color, ((0 << 0) | 2));
    cout << " => 3. Трасса построения выпуклой оболочки множества точек на плоскости " << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 6));
    cout << "|";
    SetConsoleTextAttribute(color, ((0 << 0) | 2));
    cout << " => 4. Повернуть назад (выйти из меню) " << "\n";
    SetConsoleTextAttribute(color, ((0 << 0) | 6));
    cout << "|----------------------------------------------------------------------------------------------------------------------|" << "\n";
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
    //setlocale(LC_ALL, "russian");
    int exit = 0;

    while (!exit) {
        int num;
        //system("color 70");
        colors_menu();

        cin >> num;
        switch (num) {
        case 1: {
            srand(time(NULL));
            SetConsoleTextAttribute(color, ((0 << 0) | 3));

            int countOtr;

            cout << "Введите количество отрезков: ";
            cin >> countOtr;

            otrezok** sets = new otrezok*[countOtr];

            for (size_t i = 0; i < countOtr; i++) {
                tochkaDouble a, b;
                a.x = rand() % 55 - 25;
                a.y = rand() % 55 - 25;
                b.x = rand() % 55 - 25;
                b.y = rand() % 55 - 25;

                *(sets + i) = new otrezok(a, b);

            }

            for (size_t i = 0; i < countOtr; i++) {
                sets[i]->getTochki();
            }

            auto a = bentleyOttman(sets, countOtr);

            if (a) {
                cout << "Пересечения есть" << "\n";
            }

            else {
                cout << "Пересечений нет";
            }

            for (size_t i = 0; i < countOtr; i++) {
                delete[] * (sets + i);
            }

            delete[] sets;
            break;
        }

        case 2: {
            srand(time(NULL));
            int countTchk, countElem;

            SetConsoleTextAttribute(color, ((0 << 0) | 13));

            cout << "Введите количество точек: ";
            cin >> countTchk;
            cout << "\n";

            tochka* sets = new tochka[countTchk];
            tochka* sets1 = new tochka[countTchk];

            for (int i = 0; i < countTchk; i++) {
                sets[i].x = sets1[i].x = rand() % 55 - 25;
                sets[i].y = sets1[i].y = rand() % 55 - 25;
            }

            cout << "Начальный массив: \n";

            for (int i = 0; i < countTchk; i++) {
                cout << sets[i].x << " " << sets[i].y << "\n";
            }

            SortSliyaniem(sets, sets1, 0, countTchk - 1);

            cout << "\nКонечный массив: \n";

            for (int i = 0; i < countTchk; i++) {
                cout << sets[i].x << " " << sets[i].y << "\n";
            }

            cout << "Введите число нужных элементов: ";
            cin >> countElem;

            cout << "Минимальные элементы в массиве: " << "\n";

            for (int i = 0; i < countElem; i++) {
                cout << sets[i].x << " " << sets[i].y << "\n";
            }

            cout << "Максимальные элементы в массиве: " << "\n";

            for (int i = countTchk - 1; i > countTchk - countElem - 1; i--) {
                cout << sets[i].x << " " << sets[i].y << "\n";
            }

            delete[] sets;
            delete[] sets1;
            break;
        }

        case 3: {
            //system("color 70");
            srand(time(NULL));
            int countTchk;
            SetConsoleTextAttribute(color, ((0 << 0) | 3));

            cout << "Введите количество точек: ";
            cin >> countTchk;
            cout << "\n";

            tochka* sets = new tochka[countTchk];
            tochka* sets1 = new tochka[countTchk];

            for (int i = 0; i < countTchk; i++) {
                sets[i].x = sets1[i].x = rand() % 22 + 1;
                sets[i].y = sets1[i].y = rand() % 55 - 25;
            }

            // 1 точка-ориентир по которой будут отсортированы другие против часовой стрелки
            for (int i = 0; i < countTchk; i++) {
                if ((sets[i].x < sets[0].x) || (sets[i].x == sets[0].x && sets[i].y < sets[0].y)) {
                    swap(sets[0], sets[i]);
                }
            }

            cout << "До сортировки по полярному углу (1 элемент уже самый левый): " << "\n";

            for (int i = 0; i < countTchk; i++) {
                cout << sets[i].x << " " << sets[i].y << "\n";
            }

            SortSliyaniemPoUglu(sets, sets1, 1, countTchk - 1);

            for (int f = 1; f < countTchk; f++) {
                cout << "Угол точкм" << f << ": " << ugol(sets[0], sets[f]) << "\n";
            }
            cout << "\n";

            cout << "После сортировки по полярному углу: " << "\n";

            for (int i = 0; i < countTchk; i++) {
                cout << sets[i].x << " " << sets[i].y << "\n";
            }

            vector <tochka> vector_mvo;

            vector_mvo.push_back(sets[0]);
            vector_mvo.push_back(sets[1]);

            for (int i = 2; i < countTchk; i++) {
                while (ProverkaTochkiC(vector_mvo[vector_mvo.size() - 2], vector_mvo[vector_mvo.size() - 1], sets[i]) < 0) {
                    vector_mvo.pop_back();
                }
                vector_mvo.push_back(sets[i]);
            }

            cout << "Минимальная выпуклая оболочка из данных точек по алгоритму Грэхема: \n";

            for (size_t i = 0; i < vector_mvo.size(); i++) {
                cout << vector_mvo[i].x << " " << vector_mvo[i].y << "\n";
            }

            vector_mvo.clear();
            delete[] sets1;
            delete[] sets;
            break;
        }

        case 4: {
            unsigned int vybor;

            cout << "Ты точно хочешь сойти со своего пути и выйти в реальную жизнь: 0. Да. 1. Нет\n";
            cin >> vybor;

            if (!vybor) {
                exit = 1;
                cout << "Ты покинул свой путь, путник. Возвращайся ещё!" << "\n";
                break;
            }
            else {
                break;
            }
        }

        default: {
            cout << "Error 404" << "\n";
        }
        }
    }
}
