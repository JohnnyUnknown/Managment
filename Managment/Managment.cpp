#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <string>

using namespace std;

class Bank;
int endGame();

// Заявка на покупку ЕСМ
struct buyESM
{
    int quantityESM, priceESM;
};
// Заявка на продажу ЕГП
struct sellEGP
{
    int quantityEGP, priceEGP;
};
// Заявка на взятие ссуды
struct takeLoan
{
    int sumLoan, monthsBeforePayment;
};
// Заявка на строительство фабрики
struct factoryConstr
{
    int sumDebt;                        // сумма второго платежа по строительству
    int monthsBeforeEnd;                // месяцев до окончания строительства
    int monthsBeforePayment;            // месяцев до снятия второго платежа
    int type;                           // тип работ. 1 - фабрика, 2 - автомат, 3 - автоматизир-е обычной фабрики
};
// Защита "от дурака" для вводимых данных 
int foolProof()
{
    int digit;
    cin >> digit;
    if (!digit || digit < 0)
    {
        cout << "\nБудет принято значение 0 (если это возможно).\n";
        // Очистка буфера в случае ввода знаков
        cin.clear();
        while (cin.get() != '\n')
        {
            continue;
        }
        return 0;
    }
    // Очистка буфера в случае ввода дробного числа
    while (cin.get() != '\n')
    {
        continue;
    }
    return digit;
}
// Функция вывода правил игры
void Rules()
{
    system("cls");
    const char* path = "F:\\c#\\My projects\\С++\\Managment\\Rules.txt";
    string line;
    ifstream in(path); // окрываем файл для чтения
    if (in.is_open())
    {
        while (getline(in, line))
        {
            cout << line << endl;
        }
    }
    in.close();
    _getch();
    endGame();
}
// Ввод количества игроков в начале игры
int numberOfPlayers()
{
    int num;
    do
    {
        cout << "Введите количество игроков (от 2 до 4): ";
        num = foolProof();
    } while (!(num >= 1 && num <= 4));
    cout << endl;
    return num;
}
// Выбор условия окончания игры игроком
int endGame()
{
    int input, num;
    do
    {
        system("cls");
        cout << "Выберите режим игры:\n\n1 - До последнего игрока;\n2 - По количеству месяцев;\n"
            << "3 - Правила игры.\n\n";
        input = foolProof();
    } while (input < 1 || input > 3);
    if (input == 2)
    {
        do
        {
            system("cls");
            cout << "Введите количество месяцев (от 13 до 60): ";
            num = foolProof();
        } while (num < 13 || num > 60);
        return num;
    }
    else if (input == 3) Rules();
    else return 1000;
}


class Gamer
{
    /*ESM - материалы, EGP - гот. продукция, factory - фабрики, avtoFactory - автоматиз. фабрики, cashMoney - наличные, 
    underConstructionFactory - строящиеся фабрики, underAvtoFactory - фабрики, проходящие автоматизацию,
    allLoan - сумма всех ссуд, numLoans - общее кол-во ссуд, capital - общий капитал*/
    int ESM, EGP, factory, avtoFactory, cashMoney, underConstructionFactory, underAvtoFactory, allLoan, numLoans, capital;
    takeLoan* loans;                        // взятые ссуды
    factoryConstr* construction;            // строящиеся фабрики
    bool bankrot;                           // если bankrot == true, то игрок банкрот
    buyESM myOfferESM;                      // Временное хранилище для предложения о покупке ЕСМ
    sellEGP myOfferEGP;                     // Временное хранилище для предложения о продаже ЕГП
    takeLoan request;                       // Временное хранилище для зарпроса о взятии ссуды
    factoryConstr construct;                // Временное хранилище для зарпроса на строительство (модернизацию) фабрики

    // Конструктор создания нового игрока
    Gamer();

    // Вывод состояния игрока
    void printGamer();
    // Предложение банку о покупке ЕСМ
    void offerESM();
    // Предложение банку о продаже ЕГП
    void offerEGP();
    // Функция производства ЕГП
    void manufactureEGP();
    // Функция банкротства игрока
    void crash();
    // Запрос банку о взятии ссуды
    void requestLoan();
    // Функция погашения ссуды
    void repaymentLoan();
    // Списание ежемесячного процента по ссудам и уменьшение сроков до погашения
    void percentLoan();
    // Функция выдачи ссуды 
    void paymentLoan();
    // Функция начала строительства фабрик
    void requestConstruction();
    // Функция окончания строительства, закрытие заказа
    void paymentConstr();
    // Добавление новой стройки в массив строек
    void addNewConstr();
    // Удаление стройки из массива строек
    void delConstr();
    // Проверка на банкротство
    void bankrotGamer();

    // Объявление класса Bank дружественным для доступа к закрытым полям
    friend Bank;        
    
    ~Gamer();
};


class Bank
{
    /*atFirstGamers - изначальное кол-во игроков, quantityGamers - текущее колич-во игроков, round - круг игры, 
    numSenior - номер старшего игрока, levelPrice - текущий уровень цен, soldESM - текущее кол-во продаваемых ЕСМ, 
    priceESM - текущая min цена ЕСМ, buyEGP - текущее кол-во покупаемых ЕГП, priceEGP - текущая max цена ЕГП*/
    int atFirstGamers, quantityGamers, round, numSenior, levelPrice, soldESM, priceESM, buyEGP, priceEGP;
    Gamer* allGamers;               // Массив хранения всех игроков

    Bank();
    // Указатель на единственный объект класса Bank
    static Bank* bank;      
    // Вывод информации на экран
    void printBank(int i);
    // Функция списания ежемесячных расходов со всех игроков
    void monthlyCosts();
    // Определение текущего уровня цен
    void priceLevel();
    // Переход уровня цен с определенной вероятностью
    void transitionLevel();
    // Проверка и публикация всех предложений о покупке ЕСМ 
    void publicTenderESM();
    // Проверка и публикация всех предложений о продаже ЕГП 
    void publicTenderEGP();
    // Проверка на банкротство
    void bankrotGamer();
    // Контроль месяцев игры и нахождение старшего игрока
    void gameControl();
    // Функция продажи ЕСМ игрокам
    void sellESM();
    // Функция покупки ЕГП игроками
    void buyEgp();
    // Поиск победителя
    void searchWinner();

public:

    // Функция для создания единственного объекта класса Bank
    static Bank* getInstance();
    // Главная функция игры
    void game();
    ~Bank() { delete[] allGamers; }
};

//--------------------------------------------------------------------------------------------

int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));

    Bank* bank = Bank::getInstance();
    bank->game();

}

//--------------------------------------------------------------------------------------------

// Конструктор создания нового игрока
Gamer::Gamer() : ESM{ 4 }, EGP{ 2 }, factory{ 2 }, avtoFactory{ 0 }, cashMoney{ 10000 }, underConstructionFactory{ 0 },
underAvtoFactory{ 0 }, allLoan{ 0 }, loans{ nullptr }, construction{ nullptr }, bankrot{ false }, myOfferESM{ 0,0 },
myOfferEGP{ 0,0 }, request{ 0,0 }, numLoans{ 0 }, capital{ 0 } {}
// Вывод состояния игрока
void Gamer::printGamer()
{
    cout << "Средства и материалы:\t" << ESM << " шт.\nГотовая продукция:\t" << EGP << " шт.\nОбычные фабрики:\t"
        << factory << " шт.\nАвтоматизир. фабрики:\t" << avtoFactory << " шт.\nНаличные деньги:\t" << cashMoney
        << " $\nСтроящиеся фабрики:\t" << underConstructionFactory + underAvtoFactory
        << " шт.\nСумма всех ссуд:\t" << allLoan << " $\nОбщее кол-во ссуд:\t" << numLoans << " шт.\n\n";
    cout << "--------------------------------------------------------------------------------------------\n";
}
// Предложение банку о покупке ЕСМ
void Gamer::offerESM()
{
    cout << "\t\t\t\tЗаявка на покупку ЕСМ:\n";
    cout << "\t(Если количество необходимых ЕСМ < 1 или предложенная вами цена ниже банковской,\n\t\t\t\tто вы исключаетесь из торгов).\n\n";
    int price, quantity;

    cout << "Введите количество ЕСМ, которое хотите купить: ";
    quantity = foolProof();
    if (quantity > 0)
    {
        cout << "Введите вашу цену за одну ЕСМ: ";
        price = foolProof();
    }
    else price = 0;
    cout << endl;
    myOfferESM.priceESM = price;
    myOfferESM.quantityESM = quantity;
}
// Предложение банку о продаже ЕГП
void Gamer::offerEGP()
{
    cout << "\t\t\t\tЗаявка на продажу ЕГП:\n";
    cout << "\t(Если предложенная вами цена выше банковской, то вы исключаетесь из торгов).\n\n";
    int price, quantity;
    cout << "Введите количество ЕГП, которое хотите продать: ";
    quantity = foolProof();
    if (quantity > EGP) quantity = EGP;
    if (quantity > 0)
    {
        cout << "Введите вашу цену за одну ЕГП: ";
        price = foolProof();
    }
    else price = 0;
    cout << endl;
    myOfferEGP.priceEGP = price;
    myOfferEGP.quantityEGP = quantity;
}
// Функция производства ЕГП
void Gamer::manufactureEGP()
{
    cout << "\t\t\t\tПроизводство продукции.\n\n";
    int num, tempFactory = factory, tempAvtoFactory = avtoFactory;
    cout << "Сколько ЕСМ необходимо переработать? ";
    num = foolProof();
    cout << endl;
    while (num)
    {
        // Если есть автоматизир. фабрики и необходимо переработать > 1 ЕСМ, делаем по 2 шт.
        if (tempAvtoFactory && num > 1 && ESM > 1 && cashMoney >= 3000)
        {
            num -= 2;
            ESM -= 2;
            EGP += 2;
            cashMoney -= 3000;
            tempAvtoFactory--;
        }
        // Обычный режим переработки
        else if ((tempFactory || tempAvtoFactory) && ESM > 0 && cashMoney >= 2000)
        {
            num--;
            ESM--;
            EGP++;
            cashMoney -= 2000;
            tempFactory--;
        }
        else break;
    }
    bankrotGamer();
    printGamer();
}
// Функция банкротства игрока
void Gamer::crash()
{
    ESM = { 0 }, EGP = { 0 }, factory = { 0 }, avtoFactory = { 0 }, cashMoney = { 0 },
        underConstructionFactory = { 0 }, allLoan = { 0 }, loans = { nullptr }, construction = { nullptr },
        bankrot = { true }, myOfferESM = { 0,0 }, myOfferEGP = { 0,0 };
}
// Запрос банку о взятии ссуды
void Gamer::requestLoan()
{
    int sum;
    cout << "\t\t\t\tЗапрос о взятии ссуды:\n\n";
    cout << "Введите желаемую сумму ссуды ( нажмите 0, чтобы отказаться ): ";
    sum = foolProof();
    request.sumLoan = sum;
    request.monthsBeforePayment = 2;
}
// Функция погашения ссуды
void Gamer::repaymentLoan()
{
    // Проход по всем ссудам
    for (int i = 0; i < numLoans; i++)
    {
        // Если подошел срок, выплачиваем ссуду
        if (loans[i].monthsBeforePayment == 0)
        {
            cashMoney -= loans[i].sumLoan;
            allLoan -= loans[i].sumLoan;
            loans[i].sumLoan = 0;

            // Объявление временного массива и копирование в него всех ссуд игрока
            takeLoan* temp = new takeLoan[numLoans];
            for (int j = 0; j < numLoans; j++)
                temp[j] = loans[j];
            // Удаление старого массива, уменьшение кол-ва ссуд игрока на 1, выделение памяти под новое кол-во ссуд
            delete[] loans;
            numLoans--;
            loans = new takeLoan[numLoans];
            // Копирование из временного массива значения всех ссуд игрока кроме первой (оплаченной)
            for (int j = 1; j <= numLoans; j++)
            {
                loans[j - 1] = temp[j];
            }
            // Удаление временного массива
            delete[] temp;
        }
    }
    bankrotGamer();
}
// Списание ежемесячного процента по ссудам и уменьшение сроков до погашения
void Gamer::percentLoan()
{
    cashMoney -= allLoan / 100;
    for (int i = 0; i < numLoans; i++)
    {
        loans[i].monthsBeforePayment--;
    }
    bankrotGamer();
}
// Функция выдачи ссуды 
void Gamer::paymentLoan()
{
    int capital = 0;        // гарантированный капитал игрока
    // Если у игрока запрос на ссуду != 0
    if (request.sumLoan)
    {
        capital += factory * 5000 + avtoFactory * 10000;
        // Общая сумма непогашенных ссуд не может превышать половины гарантированного капитала
        if (allLoan + request.sumLoan <= capital / 2)
        {
            // Объявление временного массива и копирование в него всех ссуд игрока
            takeLoan* temp = new takeLoan[numLoans];
            for (int j = 0; j < numLoans; j++)
                temp[j] = loans[j];
            // Удаление старого массива, увеличение кол-ва ссуд игрока на 1, выделение памяти под новое кол-во ссуд
            delete[] loans;
            numLoans++;
            loans = new takeLoan[numLoans];
            // Копирование из временного массива значения всех ссуд игрока и добавление новой
            for (int j = 0; j < numLoans; j++)
            {
                loans[j] = temp[j];
                if (j == numLoans - 1) loans[j] = request;
            }
            // Удаление временного массива
            delete[] temp;

            // Взятие ссуды
            allLoan += request.sumLoan;
            cashMoney += request.sumLoan;
        }
        // Запрос обнуляется
        request = { 0,0 };
    }
}
// Функция начала строительства фабрик
void Gamer::requestConstruction()
{
    if ((factory + avtoFactory + underConstructionFactory) < 6)      // не более 6 фабрик
    {
        int num;
        cout << "\t\t\t\tЗапрос на строительство:\n\t(Половина стоимости будет списана сразу,"
            << " половина по окончанию строительства)\n\n";
        do
        {
            cout << "Введите тип необходимой фабрики (max 6 фабрик):\n1 - обычная фабрика\t\t\t(5000$; 5 мес.);\n"
                << "2 - автоматизированная фабрика\t\t(10000$; 7 мес.);\n"
                << "3 - автоматизировать обычную фабрику\t(7000$; 9 мес.);\n"
                << "0 - выход;\n";
            num = foolProof();
        } while (!(num >= 0 && num <= 3));
        // Требования для постройки фабрики
        switch (num)
        {
        case 1:
            construct.sumDebt = 5000;
            construct.monthsBeforeEnd = 5;
            construct.monthsBeforePayment = construct.monthsBeforeEnd - 1;
            construct.type = 1;
            break;
        case 2:
            construct.sumDebt = 10000;
            construct.monthsBeforeEnd = 7;
            construct.monthsBeforePayment = construct.monthsBeforeEnd - 1;
            construct.type = 2;
            break;
        case 3:
            if (factory > underAvtoFactory)
            {
                construct.sumDebt = 7000;
                construct.monthsBeforeEnd = 9;
                construct.monthsBeforePayment = construct.monthsBeforeEnd - 1;
                construct.type = 3;
                break;
            }
            else
            {
                num = 0;
                break;
            }
        default:
            break;
        }
        // Проверка наличия средств для строительства
        if (cashMoney >= construct.sumDebt)
        {
            if (((num == 1 || num == 3) && construct.sumDebt >= 5000) || (num == 2 && construct.sumDebt == 10000))
            {
                // Списание первой половины стоимости и добавление одной строящейся фабрики
                construct.sumDebt /= 2;
                cashMoney -= construct.sumDebt;
                if (num == 1 || num == 2) underConstructionFactory++;
                else if (num == 3) underAvtoFactory++;
                addNewConstr();
            }
        }
        else cout << "\nНедостаточно средств!!!\n\n";
    }
}
// Функция окончания строительства, закрытие заказа
void Gamer::paymentConstr()
{
    // Цикл по всем строящимся и автоматизируемым фабрикам
    for (int i = 0; i < underConstructionFactory + underAvtoFactory; i++)
    {
        construction[i].monthsBeforeEnd--;
        // Если срок вышел, то производим строительство фабрики
        if (!construction[i].monthsBeforeEnd)
        {
            switch (construction[i].type)
            {
            case 1:
                factory++;
                underConstructionFactory--;
                break;
            case 2:
                avtoFactory++;
                underConstructionFactory--;
                break;
            case 3:
                factory--;
                avtoFactory++;
                underAvtoFactory--;
                break;
            }
            // Удаление заявки
            construction[i] = { 0,0,0,0 };
            delConstr();
        }
        // Если месяцев до окончания строительства > 1. 
        if (construction[i].monthsBeforeEnd > 0)
        {
            construction[i].monthsBeforePayment--;
            // Если срок вышел, то оплачиваем вторую часть стоимости строительства
            if (!construction[i].monthsBeforePayment)
            {
                cashMoney -= construction[i].sumDebt;
                construction[i].sumDebt = 0;
            }
        }
    }
    bankrotGamer();
}
// Добавление новой стройки в массив строек
void Gamer::addNewConstr()
{
    // Объявление временного массива и копирование в него всех строительных работ
    factoryConstr* temp = new factoryConstr[underConstructionFactory + underAvtoFactory - 1];
    for (int j = 0; j < underConstructionFactory + underAvtoFactory - 1; j++)
        temp[j] = construction[j];
    // Удаление старого массива, увеличение кол-ва строительных работ игрока на 1, выделение памяти под новое кол-во строек
    delete[] construction;
    construction = new factoryConstr[underConstructionFactory + underAvtoFactory];
    // Копирование из временного массива значения всех строек игрока и добавление новой
    for (int j = 0; j < underConstructionFactory + underAvtoFactory; j++)
    {
        construction[j] = temp[j];
        if (j == underConstructionFactory + underAvtoFactory - 1) construction[j] = construct;
    }
    // Удаление временного массива
    delete[] temp;
}
// Удаление стройки из массива строек
void Gamer::delConstr()
{
    // Объявление временного массива и копирование в него всех строительных работ
    factoryConstr* temp = new factoryConstr[underConstructionFactory + underAvtoFactory + 1];
    for (int j = 0; j < underConstructionFactory + underAvtoFactory + 1; j++)
        temp[j] = construction[j];
    // Удаление старого массива, уменьшение кол-ва строительных работ игрока на 1, выделение памяти под новое кол-во строек
    delete[] construction;
    construction = new factoryConstr[underConstructionFactory + underAvtoFactory];
    // Копирование из временного массива значения всех действующих строек игрока
    for (int j = 0, i = 0; j < underConstructionFactory + underAvtoFactory + 1; j++, i++)
    {
        if (temp[j].monthsBeforeEnd == 0)
        {
            i--;
            continue;
        }
        else construction[i] = temp[j];
    }
    // Удаление временного массива
    delete[] temp;
}
// Проверка на банкротство
void Gamer::bankrotGamer()
{
    if (cashMoney < 0) crash();
}

Gamer::~Gamer()
{
    delete[] loans;
    delete[] construction;
}


// Указатель на единственный объект класса Bank
Bank* Bank::bank = nullptr;
// Функция для создания единственного объекта класса Bank
Bank* Bank::getInstance()
{
    if (bank == nullptr)
    {
        bank = new Bank();
    }
    return bank;
}
// Конструктор класса Bank
Bank::Bank()
{
    atFirstGamers = numberOfPlayers();
    quantityGamers = atFirstGamers;
    round = 0;
    numSenior = 1;
    levelPrice = rand() % 5 + 1;
    priceLevel();
    allGamers = new Gamer[atFirstGamers];
}
// Вывод информации на экран
void Bank::printBank(int i)
{
    system("cls");
    cout << "\t\t\t\tИ Г Р А   М Е Н Е Д Ж Е Р\n\n";
    cout << "\tМесяц: " << round << "\tНомер старшего игрока: " << numSenior
        << "\tТекущее количество игроков: " << quantityGamers << "\n";
    cout << "--------------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\tТекущие уровни цен:\n\n";
    cout << "Уровень\t\tКол-во ЕСМ\tmin цена за ЕСМ\t\tКол-во ЕГП\tmax цена за ЕГП\n";
    cout << "" << levelPrice << "\t\t" << soldESM << "\t\t" << priceESM << "\t\t\t"
        << buyEGP << "\t\t" << priceEGP << "\n\n";
    cout << "============================================================================================\n";
    cout << "\t\t\t\tХод игрока номер " << i + 1 << ":\n\n";
    allGamers[i].printGamer();
}
// Функция списания ежемесячных расходов со всех игроков
void Bank::monthlyCosts()
{
    for (int i = 0; i < atFirstGamers; i++)
    {
        if (!allGamers[i].bankrot)      // если игрок не банкрот
        {
            allGamers[i].cashMoney -= allGamers[i].ESM * 300 + allGamers[i].EGP * 500
                + allGamers[i].factory * 1000 + allGamers[i].avtoFactory * 1500;
            // проверка оставшегося баланса игрока. если < 0, то игрок - банкрот
            bankrotGamer();
        }
        else continue;                  // Если игрок банкрот, он пропускается
    }
}
// Определение текущего уровня цен
void Bank::priceLevel()
{
    switch (levelPrice)
    {
    case 1:
        soldESM = quantityGamers;
        priceESM = 800;
        buyEGP = 3 * quantityGamers;
        priceEGP = 6500;
        break;
    case 2:
        soldESM = ceil(1.5 * quantityGamers);
        priceESM = 650;
        buyEGP = ceil(2.5 * quantityGamers);
        priceEGP = 6000;
        break;
    case 3:
        soldESM = 2 * quantityGamers;
        priceESM = 500;
        buyEGP = 2 * quantityGamers;
        priceEGP = 5500;
        break;
    case 4:
        soldESM = ceil(2.5 * quantityGamers);
        priceESM = 400;
        buyEGP = ceil(1.5 * quantityGamers);
        priceEGP = 5000;
        break;
    case 5:
        soldESM = 3 * quantityGamers;
        priceESM = 300;
        buyEGP = quantityGamers;
        priceEGP = 4500;
        break;
    }
}
// Переход уровня цен с определенной вероятностью
void Bank::transitionLevel()
{
    int levelNext;
    int random = rand() % 12 + 1;
    switch (levelPrice)
    {
    case 1:
        if (random > 11) levelNext = 5;
        else if (random > 10) levelNext = 4;
        else if (random > 8) levelNext = 3;
        else if (random > 4) levelNext = 2;
        else if (random > 0) levelNext = 1;
    case 2:
        if (random > 11) levelNext = 5;
        else if (random > 10) levelNext = 4;
        else if (random > 7) levelNext = 3;
        else if (random > 3) levelNext = 2;
        else if (random > 0) levelNext = 1;
    case 3:
        if (random > 11) levelNext = 5;
        else if (random > 8) levelNext = 4;
        else if (random > 4) levelNext = 3;
        else if (random > 1) levelNext = 2;
        else if (random > 0) levelNext = 1;
    case 4:
        if (random > 9) levelNext = 5;
        else if (random > 5) levelNext = 4;
        else if (random > 2) levelNext = 3;
        else if (random > 1) levelNext = 2;
        else if (random > 0) levelNext = 1;
    case 5:
        if (random > 8) levelNext = 5;
        else if (random > 4) levelNext = 4;
        else if (random > 2) levelNext = 3;
        else if (random > 1) levelNext = 2;
        else if (random > 0) levelNext = 1;
    }
    levelPrice = levelNext;
}
// Проверка и публикация всех предложений о покупке ЕСМ 
void Bank::publicTenderESM()
{
    cout << "============================================================================================\n";
    cout << "\t\t\tПубликация результатов тендера о продаже ЕСМ:\n\n";
    for (int i = 0; i < atFirstGamers; i++)     // цикл по первоначальному кол-ву игроков
    {
        // проверка на банкротство и проверка самого предложения
        if (!allGamers[i].bankrot && allGamers[i].myOfferESM.quantityESM > 0
            && allGamers[i].myOfferESM.priceESM >= priceESM)
        {
            cout << "Игрок номер " << i + 1 << " хочет купить " << allGamers[i].myOfferESM.quantityESM
                << " штук ЕСМ по " << allGamers[i].myOfferESM.priceESM << " $ за штуку.\n";
        }
        // если игрок банкрот и предложение еще не обнулено, то его предложение обнуляется
        else if (allGamers[i].bankrot && allGamers[i].myOfferESM.priceESM != 0)
            allGamers[i].myOfferESM = { 0,0 };
    }
    cout << endl;
}
// Проверка и публикация всех предложений о продаже ЕГП 
void Bank::publicTenderEGP()
{
    cout << "============================================================================================\n";
    cout << "\t\t\tПубликация результатов тендера о покупке ЕГП:\n\n";
    for (int i = 0; i < atFirstGamers; i++)     // цикл по первоначальному кол-ву игроков
    {
        // проверка на банкротство и проверка самого предложения
        if (!allGamers[i].bankrot && allGamers[i].myOfferEGP.quantityEGP > 0
            && allGamers[i].myOfferEGP.priceEGP <= priceEGP)
        {
            cout << "Игрок номер " << i + 1 << " хочет продать " << allGamers[i].myOfferEGP.quantityEGP
                << " штук ЕГП по " << allGamers[i].myOfferEGP.priceEGP << " $ за штуку.\n";
        }
        // если игрок банкрот и предложение еще не обнулено, то его предложение обнуляется
        else if (allGamers[i].bankrot && allGamers[i].myOfferEGP.quantityEGP != 0)
            allGamers[i].myOfferEGP = { 0,0 };
    }
    cout << endl;
}
// Проверка на банкротство
void Bank::bankrotGamer()
{
    // Если cashMoney игрока после торгов < 0, игрок объявляется банкротом
    for (int i = 0; i < atFirstGamers; i++)
    {
        if (allGamers[i].cashMoney < 0)
        {
            allGamers[i].crash();
            quantityGamers--;
        }
    }
}
// Контроль месяцев игры и нахождение старшего игрока
void Bank::gameControl()
{
    round++;                // + месяц игры
    if (quantityGamers > 1)
    {
        // Нахождение номера старшего игрока по порядку, если игроков > 1
        int tempSenior;
        tempSenior = (round - 1) % quantityGamers + 1;
        // Нахождение индекса старшего игрока
        for (int i = 0; i < atFirstGamers; i++)
        {
            if (!allGamers[i].bankrot && tempSenior) tempSenior--;
            else continue;
            if (!allGamers[i].bankrot && tempSenior == 0)
            {
                numSenior = i + 1;
                break;
            }
        }
        transitionLevel();
        priceLevel();
    }
}
// Функция продажи ЕСМ игрокам
void Bank::sellESM()
{
    int maxPrice = 0, num = 0;       // max предложенная цена, кол-во участников
    bool buyers = true;              // наличие покупателей
    // Нахождение кол-во покупателей
    for (int i = 0; i < atFirstGamers; i++)
    {
        if (allGamers[i].myOfferESM.priceESM >= priceESM)
            num++;
    }
    // Цикл продажи пока есть ЕСМ и покупатели
    while (soldESM > 0 && buyers && num)
    {
        maxPrice = 0;
        // Нахождение max предложенной цены
        for (int i = 0; i < atFirstGamers; i++)
        {
            if (allGamers[i].myOfferESM.priceESM > maxPrice && allGamers[i].myOfferESM.priceESM >= priceESM)
                maxPrice = allGamers[i].myOfferESM.priceESM;
        }
        int i = numSenior - 1;
        // Покупка ЕСМ покупателем с самым высоким предложением 
        for (int j = 0; j < atFirstGamers; j++)
        {
            // Покупка ЕСМ покупателем с самым высоким предложением, если старший игрок предложил цену меньше
            if (allGamers[i].myOfferESM.priceESM == maxPrice && soldESM)
            {
                // Продажа ЕСМ банком
                allGamers[i].cashMoney -= allGamers[i].myOfferESM.priceESM * allGamers[i].myOfferESM.quantityESM;
                // Если запрашиваемое кол-во <= предложению банка
                if (allGamers[i].myOfferESM.quantityESM <= soldESM)
                {
                    allGamers[i].ESM += allGamers[i].myOfferESM.quantityESM;
                    soldESM -= allGamers[i].myOfferESM.quantityESM;
                }
                else
                {
                    // Продажа ЕСМ банком, если запрос превысил предложение
                    allGamers[i].ESM += soldESM;
                    soldESM = 0;
                }
                // Обнуление предложения игрока
                allGamers[i].myOfferESM.priceESM = 0;
                allGamers[i].myOfferESM.quantityESM = 0;
                num--;
                break;
            }
            else
            {
                // Смена индекса игрока от старшего по порядку. Для обеспечения преимущества на торгах
                if (i < atFirstGamers - 1) i++;
                else i = 0;
            }
        }
        // Если покупатели отсутствуют
        if (!num)  buyers = false;
    }
    bankrotGamer();
}
// Функция покупки ЕГП игроками
void Bank::buyEgp()
{
    int minPrice = 0, num = 0;       // min предложенная цена, кол-во участников
    bool sellers = true;             // наличие продавцов
    // Нахождение кол-во продавцов
    for (int i = 0; i < atFirstGamers; i++)
    {
        if (allGamers[i].myOfferEGP.priceEGP <= priceEGP && allGamers[i].myOfferEGP.priceEGP > 0)
            num++;
    }
    // Цикл покупки ЕГП пока есть предложение о покупке и покупатели
    while (buyEGP > 0 && sellers && num)
    {
        minPrice = priceEGP;
        // Нахождение min предложенной цены
        for (int i = 0; i < atFirstGamers; i++)
        {
            if (allGamers[i].myOfferEGP.priceEGP < minPrice && allGamers[i].myOfferEGP.priceEGP <= priceEGP
                && allGamers[i].myOfferEGP.priceEGP > 0)
                minPrice = allGamers[i].myOfferEGP.priceEGP;
        }
        int i = numSenior - 1;
        // Продажа ЕГП покупателем с самым низким предложением 
        for (int j = 0; j < atFirstGamers; j++)
        {
            // Продажа ЕГП покупателем с самым низким предложением
            if (allGamers[i].myOfferEGP.priceEGP == minPrice && buyEGP)
            {
                // Если запрашиваемое кол-во <= предложению банка
                if (allGamers[i].myOfferEGP.quantityEGP <= buyEGP)
                {
                    // Покупка ЕГП банком
                    allGamers[i].cashMoney += allGamers[i].myOfferEGP.priceEGP * allGamers[i].myOfferEGP.quantityEGP;
                    allGamers[i].EGP -= allGamers[i].myOfferEGP.quantityEGP;
                    buyEGP -= allGamers[i].myOfferEGP.quantityEGP;
                }
                else
                {
                    // Покупка ЕГП банком, если запрос превысил предложение
                    allGamers[i].cashMoney += allGamers[i].myOfferEGP.priceEGP * buyEGP;
                    allGamers[i].EGP -= buyEGP;
                    buyEGP = 0;
                }
                // Обнуление предложения игрока
                allGamers[i].myOfferEGP.priceEGP = 0;
                allGamers[i].myOfferEGP.quantityEGP = 0;
                num--;
                break;
            }
            else
            {
                // Смена индекса игрока от старшего по порядку. Для обеспечения преимущества на торгах
                if (i < atFirstGamers - 1) i++;
                else i = 0;
            }
        }
        // Если покупатели отсутствуют
        if (!num)  sellers = false;
    }
}
// Поиск победителя
void Bank::searchWinner()
{
    int Winner = 0, capitalWin = 0;
    for (int i = 0; i < atFirstGamers; i++)
    {
        if (allGamers[i].bankrot) continue;
        else
        {
            // Нахождение всех неоплаченных игроком строек
            int allConstrSum = 0;
            for (int j = 0; j < allGamers[i].underConstructionFactory + allGamers[i].underAvtoFactory; j++)
            {
                allConstrSum += allGamers[i].construction->sumDebt;
            }
            // Подсчет общего капитала игрока
            allGamers[i].capital = allGamers[i].factory * 5000 + allGamers[i].avtoFactory * 10000
                + allGamers[i].ESM * priceESM + allGamers[i].EGP * priceEGP + allGamers[i].cashMoney
                - allGamers[i].allLoan - allConstrSum;
            // Нахождение индекса игрока с самым большим капиталом
            if (capitalWin < allGamers[i].capital)
            {
                capitalWin = allGamers[i].capital;
                Winner = i;
            }
        }
    }
    system("cls");
    cout << "\n\n\t\t\t\tИгрок номер " << Winner + 1
        << " победил. Общий капитал: " << allGamers[Winner].capital
        << "\n\n\t\t\t\tП О З Д Р А В Л Я Е М !!!\n\n\n";
}
// Главная функция игры
void Bank::game()
{
    int limit = endGame();
    for (int i = 0; i < limit && quantityGamers > 1; i++)
    {

        gameControl();      // Контроль уровней игры
        int j = numSenior - 1;
        for (int k = 0; k < atFirstGamers; k++)
        {
            if (!allGamers[j].bankrot)
            {
                allGamers[j].percentLoan();         // Списание процентов по ссудам
                printBank(j);

                allGamers[j].offerESM();            // Предложение игрока о покупке ЕСМ
                printBank(j);

                allGamers[j].manufactureEGP();      // Решение игрока о производстве ЕГП
                printBank(j);

                allGamers[j].offerEGP();            // Предложение игрока о продаже ЕГП
                printBank(j);

                allGamers[j].repaymentLoan();       // Проверка и погашение ссуды игрока
                printBank(j);

                allGamers[j].requestLoan();         // Запрос ссуды игроком
                allGamers[j].paymentLoan();         // Получение ссуды игроком, если прошел проверку
                printBank(j);

                allGamers[j].requestConstruction(); // Решение игрока о посторойке новых фабрик
                allGamers[j].paymentConstr();       // Окончание строительства

                system("cls");
            }
            else continue;
            // Смена индекса игрока от старшего по порядку. Для обеспечения преимущества на торгах
            if (j < atFirstGamers - 1) j++;
            else j = 0;
        }
        publicTenderESM();                          // Публикация тендера о покупке ЕСМ
        sellESM();                                  // Покупка игроками ЕСМ
        publicTenderEGP();                          // Публикация тендера о продаже ЕГП
        buyEgp();                                   // Продажа игроками ЕГП
        monthlyCosts();                             // Списание ежемесячных платежей со всех игроков

        _getch();
    }
    searchWinner();
}