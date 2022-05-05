#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

struct my_time {
    int h, m;
    double s;

    void scan() {
        printf("time 'hh mm ss' : ");
        cin >> h >> m >> s;
    }

    void fscan(ifstream& fin) {
        char temp;
        fin >> h >> temp >> m >> temp >> s;
    }

    void print() const {
        printf("%02d:%02d:%02.0lf", h, m, s);
    }


    //функция сравнения с другим временем, для сортировки
    int compare(const my_time& other) const {
        if (h > other.h) { return  1; }
        if (h < other.h) { return -1; }
        if (m > other.m) { return  1; }
        if (m < other.m) { return -1; }
        if (s > other.s) { return  1; }
        if (s < other.s) { return -1; }
        return 0;
    }
};


struct customer {
    string brand; //марка машины
    my_time time;  //время
    int duration; //длительность в минутах

    //функция считывает с клавиатуры
    void scan() {
        printf("brand: ");
        cin >> brand;

        time.scan();
        printf("duration: ");
        cin >> duration;
    }

    //функция выводит информацию о структуре в консоль
    void print() {
        cout << setw(10) << brand << '\t';
        time.print();
        cout << '\t' << setw(3) << duration;
        
    }

    //функция считывает информацию о структуре в формате
    void fscan(ifstream& fin) {
        fin >> brand;
        time.fscan(fin);
        fin >> duration;
    }

    //функция записывает в файл
    void fprint(ofstream& fout) {
        fout << brand << ' ' << time.h << ' ' << time.m << ' ' << time.s << ' ' << duration;
    }
};

//функция сортирует первые size элементы массива структур
void customers_sort_by_time(customer* a, int size) {

    //сортировка пзырьком
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            if (a[j].time.compare(a[j + 1].time) == 1) {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

//функция считает по массиву согласно заданию
string calc_brand(customer* a, int size) {
    string ans; //строка ответ
    double mxAvrg = -1; //максимальное время

    for (int i = 0; i < size; ++i) {

        //проверяем, что мы еще не считали время для i-ой марки
        int check = 1;
        for (int j = 0; j < i; ++j) {
            if (a[i].brand == a[j].brand) {
                check = 0;
                break;
            }
        }

        //если не считали
        if (check) {
            double avrg = 0;
            int cnt = 0;
            for (int j = 0; j < size; ++j) {
                if (a[i].brand == a[j].brand) {
                    avrg += a[j].duration;
                    ++cnt;
                }
            }
            avrg /= cnt; //делим сумму на количество

            //сравниваем с максимумом и обновляем ответ при необходимости
            if (avrg > mxAvrg) {
                ans = a[i].brand;
                mxAvrg = avrg;
            }
        }
    }

    return ans;//возвращаем ответ
}

int main() {
    setlocale(LC_ALL, "Russian");
    customer customers[100];

    ifstream fin("data.in");

    if (!fin.is_open()) {
        cout << "Не удалось открыть файл.\n";
        return 0;
    }

    int n = 0;
    while (!fin.eof()) {
        customers[n++].fscan(fin);
    } fin.close();

    string header = "0- Выход из программы\n";
    header += "1- Перезаписать массив\n";
    header += "2- Посмотреть все записи\n";
    header += "3- Отсортировать массив по времени\n";
    header += "4- Посмотреть под номером\n";
    header += "5- Изменить под номером\n";
    header += "6- Отфильтровать по марке\n";
    header += "7- Посчитать по массиву\n";

    while (true) {
        cout << '\n' << header << '\n';

        int cmd;
        cout << "Введите команду: "; cin >> cmd;

        if (cmd == 1) {
            int newN = 0, temp = 1;
            cout << "Перезапись массива структур:\n";
            while (temp != 0) {
                customers[newN++].scan();
                cout << "0/1 - закночить/продолжить ввод: "; cin >> temp;
            }
            n = newN;

        }
        else if (cmd == 2) {
            cout << "Весь массив:\n";
            for (int i = 0; i < n; ++i) {
                cout << '\n';
                cout << setw(3) << i+1 << '\t';
                customers[i].print();
            }
            cout << '\n';

        }
        else if (cmd == 3) {
            cout << "Сортировка массива...\n";
            customers_sort_by_time(customers, n);
            cout << "Массив успешно отсортирован\n";

        }
        else if (cmd == 4 || cmd == 5) {
            int i = 0;
            while (true) {
                cout << "Введите номер: ";
                cin >> i;
                if (i < 1 || i > n) {
                    cout << "Неверный номер!\n";
                    continue;
                }
                --i;
                break;
            }

            if (cmd == 4) {
                cout << '\n';
                printf("Информация о %d-ом номере:\n", i+1);
                customers[i].print();
                cout << '\n';
            }
            else {
                cout << '\n';
                printf("Изменение информации i-ого элемента...\n");
                customers[i].scan();
            }

        }
        else if (cmd == 6) {
            cout << "\nФильтрация по марке:\n";
            string brand;
            cout << "Введите марку: "; cin >> brand;

            for (int i = 0; i < n; ++i) {
                if (brand == customers[i].brand) {
                    cout << setw(3) << i + 1 << '\t';
                    customers[i].print();
                    cout << '\n';
                }
            }

        }
        else if (cmd == 7) {
            cout << "Марка машины на которую требуется в среднем больше времени: ";
            cout << calc_brand(customers, n) << '\n';
        }

        if (cmd == 0) {
            break;
        }
    }
}
