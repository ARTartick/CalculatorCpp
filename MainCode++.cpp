#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
#include <fstream>
#include <conio.h>

using namespace std;

/*
====================================
КОНСТАНТЫ И ПЕРЕМЕННЫЕ
====================================
*/

const string version = "2.3.1"; // версия
const int STATS_COUNT = 10;

// Перечисления
enum MassiveNumbers
{
	A, B, C, ANS, X2, DISCR
};
enum StatNumbers
{
	ADD, MIN, MULT, DIV, ERR, POW, SQRT, QUAD, PYTH, MODE
};

/*
====================================
ФУНКЦИИ И КЛАССЫ
====================================
*/

// функции и утилиты
void sep()
{
	cout << "\n------------------------------------------\n\n";
}
void ClearCMD()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
	cout << "++======================================++\n";
	cout << "||      CalculatorCpp версии " << version << "      || \n";
	cout << "++======================================++\n";
}
void SayHello()
{
	cout << "\nДоступные комманды:\n";
	cout << "Сложение:                  [sum]\n";
	cout << "Вычитание:                 [sub]\n";
	cout << "Умножение:                [mult]\n";
	cout << "Деление:                   [div]\n";
	cout << "Возведение в степень:      [pow]\n";
	cout << "Квадратный корень:        [sqrt]\n";
	cout << "Квадратное уравнение:     [quad]\n";
	cout << "Теорема Пифагора:         [pyth]\n";
	cout << "Сменить метод ввода:    [switch]\n";
	cout << "Помощь:                      [h]\n";
	cout << "Выход:                     [esc]\n";
	cout << "--------------------------------\n";
	cout << "Введите команду: ";
}

// классы

class Operator
{
private:
	// переменные
	string cmd;
	time_t session_start;
	char getchCMD;
	bool inDebug;
	bool is_works;

	// массивы
	int stats[STATS_COUNT];
	float mas[6];
	// векторы и т.д.
	vector<string> history;

	// утилиты
	float GetFloat()
	{
		float val;

		while (true)
		{
			cin >> val;

			if (cin.fail())
			{
				cout << "\033[91mОшибка: введите число\033[0m\n";
				stats[ERR]++;

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Попробуйте снова: ";
				continue;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return val;
		}
	}
	string FormatFloat(float val)
	{
		stringstream ss;
		ss << fixed << setprecision(2) << val;
		return ss.str();
	}
	void DebugMode()
	{
		ClearCMD();
		sep();
		cout << "\033[92mYOU ENTERED DEBUG-MODE\033[0m\n";
		inDebug = true;

		while (inDebug)
		{
			sep();

			cout << "\033[96mDEBUG MODE MENU\033[0m";
			cout << "\n[1] Просмотр массива\n";
			cout << "[2] Статистика вычислений\n";
			cout << "[3] Общая статистика\n";
			cout << "[4] История вычислений\n";
			cout << "[5] Очистить историю и статистику\n";
			cout << "[0] Выход из debug mode\n";
			cout << "Введите команду: ";
			getchCMD = _getch();
			ClearCMD();

			switch (getchCMD)
			{
			case '0': QuitDebugMode(); break;
			case '1': ShowArray(); break;
			case '2': CalcStats(); break;
			case '3': GeneralStats(); break;
			case '4': ShowHistory(); break;
			case '5': ClearStats(); break;
			default: cout << "\033[91mError: Incorrect cmd\033[0m\n"; stats[ERR]++;
			}
		}
	}
	void SetData()
	{
		is_works = true;
		session_start = time(nullptr);
		getchCMD = ' ';
		inDebug = true;
		for (int i = 0; i < 6; i++)
		{
			mas[i] = 0;
		}
		for (int i = 0; i < STATS_COUNT; i++)
		{
			stats[i] = 0;
		}
	}
	void GetCMD()
	{
		if (stats[MODE] == 0) // standart
		{
			cin >> cmd;
		}
		else if (stats[MODE] == 1) // fast
		{
			getchCMD = _getch();
		}
	}
	void HelpMePls()
	{
		ClearCMD();
		cout << "\nКраткая сводка по калькулятору:\n";

		cout << "--------------------------------\n";
		cout << "Основные команды: \n";
		cout << "Сложение:                  [sum]\n";
		cout << "Вычитание:                 [sub]\n";
		cout << "Умножение:                [mult]\n";
		cout << "Деление:                   [div]\n";
		cout << "Возведение в степень:      [pow]\n";
		cout << "Квадратный корень:        [sqrt]\n";
		cout << "Квадратное уравнение:     [quad]\n";
		cout << "Теорема Пифагора:         [pyth]\n";

		cout << "\nСменить метод ввода:    [switch]\n";
		cout << "Помощь:                      [h]\n";
		cout << "Дебаг-мод:                   [d]\n";
		cout << "Выход:                     [esc]\n";
		cout << "--------------------------------\n";

		cout << "P.S. я хуй его знает зачем добавил эту бесполезнейшую функцию :|\n";
	}

	// команды дебага
	void ShowArray()
	{
		cout << "\n\033[93m=ShowArray\033[0m\n";

		cout << "a: " << mas[A] << endl;
		cout << "b: " << mas[B] << endl;
		cout << "c: " << mas[C] << endl;
		cout << "Last Answer: " << mas[ANS] << endl;
		cout << "Last second Answer: " << mas[X2] << endl;
		cout << "Discr: " << mas[DISCR] << endl;
	}
	void CalcStats()
	{
		cout << "\n\033[93m=CalcStats\033[0m\n";
		int a = 0;
		for (int i = 0; i < STATS_COUNT; i++)
		{
			if (i == 9)
				continue;
			a = a + stats[i];
		}

		cout << "Total operations: " << a << endl;
		cout << "Addition: " << stats[ADD] << endl;
		cout << "Substraction: " << stats[MIN] << endl;
		cout << "Multiplication: " << stats[MULT] << endl;
		cout << "Division: " << stats[DIV] << endl;
		cout << "Power: " << stats[POW] << endl;
		cout << "Square Root: " << stats[SQRT] << endl;
		cout << "Quadratic Equation: " << stats[QUAD] << endl;
		cout << "Pythagorean Theorem: " << stats[PYTH] << endl;
	}
	void GeneralStats()
	{
		cout << "\n\033[93m=GeneralStats\033[0m\n";

		time_t now = time(nullptr);
		double durat = difftime(now, session_start);

		int minutes = (int)durat / 60;
		int secondes = (int)durat % 60;

		int total = stats[ADD] + stats[MIN] + stats[MULT] + stats[DIV] + stats[POW] + stats[SQRT] + stats[QUAD] + stats[PYTH];

		cout << "Programming language: C++\n";
		cout << "Name: CALCULATORcpp\n";
		cout << "Version: " << version << endl;
		cout << "Session duration: " << minutes << "min " << secondes << "sec" << endl;
		cout << "Enter mode: ";
		switch (stats[MODE])
		{
		case 0: cout << "standart\n"; break;
		case 1: cout << "fast\n"; break;
		}
		cout << "Total operations: " << total << endl;
		cout << "Total errors: " << stats[ERR] << endl;
	}
	void ShowHistory()
	{
		cout << "\n\033[93m=ShowHistory\033[0m\n";

		if (history.empty())
		{
			cout << "\033[91mИстория операций пока что пуста...\033[0m" << endl;
		}
		int num = 1;
		for (auto it = history.rbegin(); it != history.rend(); ++it, ++num)
		{
			cout << "[" << num << "] " << *it << endl;
		}
	}
	void ClearStats()
	{
		cout << "\n\033[93m=ClearStats\033[0m\n";

		history.clear();
		cout << "История успешно очищена\n";

		for (int i = 0; i < STATS_COUNT; i++)
		{
			stats[i] = 0;
		}
		cout << "Статистика успешно очищена\n";

	}
	void QuitDebugMode()
	{
		cout << "\n=======================\n";
		cout << "\033[93mВЫ ВЫШЛИ ИЗ DEBUG MODE\033[0m";
		cout << "\n=======================\n\n";
		inDebug = false;
	}

	// основные команды
	void Summa()
	{
		ClearCMD();
		sep();

		cout << "Сложение: a + b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		mas[ANS] = mas[A] + mas[B];
		cout << "Ответ: " << mas[A] << " + " << mas[B] << " = " << mas[ANS] << endl;

		stats[ADD]++;
		AddHistory(FormatFloat(mas[A]) + " + " + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void Minus()
	{
		ClearCMD();
		sep();

		cout << "Вычитание: a - b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		mas[ANS] = mas[A] - mas[B];
		cout << "Ответ: " << mas[A] << " - " << mas[B] << " = " << mas[ANS] << endl;

		stats[MIN]++;
		AddHistory(FormatFloat(mas[A]) + " - " + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void Multiply()
	{
		ClearCMD();
		sep();

		cout << "Умножение: a * b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		mas[ANS] = mas[A] * mas[B];
		cout << "Ответ: " << mas[A] << " * " << mas[B] << " = " << mas[ANS] << endl;

		stats[MULT]++;
		AddHistory(FormatFloat(mas[A]) + " * " + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void Divide()
	{
		ClearCMD();
		sep();

		cout << "Деление: a / b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		if (mas[B] == 0)
		{
			cout << "\033[91mОшибка: деление на ноль невозможно!\033[0m\n";
			AddHistory(FormatFloat(mas[A]) + "/0 = ERR");
			stats[ERR]++;
			sep();
			return;
		}

		mas[ANS] = mas[A] / mas[B];
		cout << "Ответ: " << mas[A] << " / " << mas[B] << " = " << mas[ANS] << endl;

		stats[DIV]++;
		AddHistory(FormatFloat(mas[A]) + " / " + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void Power()
	{
		ClearCMD();
		sep();

		cout << "Возведение в степень: a^b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		mas[ANS] = pow(mas[A], mas[B]);
		cout << "Ответ: " << mas[A] << "^" << mas[B] << " = " << mas[ANS] << endl;

		stats[POW]++;
		AddHistory(FormatFloat(mas[A]) + "^" + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void Sqrt()
	{
		ClearCMD();
		sep();

		cout << "Квадратный корень: √а\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();

		if (mas[A] < 0)
		{
			mas[ANS] = sqrt(abs(mas[A]));
			cout << "Результат: √" << mas[A] << " = " << mas[ANS] << "i" << endl;
			stats[SQRT]++;
			AddHistory("√" + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]) + "i");
		}
		else
		{
			mas[ANS] = sqrt(mas[A]);
			cout << "Ответ: √" << mas[A] << " = " << mas[ANS] << endl;

			stats[SQRT]++;
			AddHistory("√" + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]));
		}
		sep();
	}
	void Quad()
	{
		ClearCMD();
		sep();

		cout << "Стандартный вид: ax^2 + bx + c = 0\n";
		cout << "Введите a: ";
		mas[A] = GetFloat();

		while (mas[A] == 0)
		{
			cout << "\033[91mОшибка: коэффициент а не может быть равен нулю\033[0m\n";
			stats[ERR]++;
			cout << "Введите а снова: ";
			mas[A] = GetFloat();
		}
		if (mas[A] != 0)
		{
			cout << "Введите b: ";
			mas[B] = GetFloat();
			cout << "Введите c: ";
			mas[C] = GetFloat();

			mas[DISCR] = pow(mas[B], 2) - (4 * mas[A] * mas[C]);
			cout << "Дискриминант равен " << mas[DISCR] << endl;
			AddHistory(FormatFloat(mas[B]) + "^2 - 4 * " + FormatFloat(mas[A]) + " * " + FormatFloat(mas[C]) + " = " + FormatFloat(pow(mas[B], 2) - (4 * mas[A] * mas[C])));

			if (mas[DISCR] == 0)
			{
				cout << "Следовательно 1 действительный корень" << endl;
				mas[ANS] = (-1 * mas[B]) / (2 * mas[A]);
				cout << "(-1 * " << mas[B] << ") / (2 * " << mas[A] << ") = " << mas[ANS] << endl;
				cout << "x1 = " << mas[ANS] << endl;
				AddHistory("(-1 * " + FormatFloat(mas[B]) + ") / (2 * " + FormatFloat(mas[A]) + ") = " + FormatFloat(mas[ANS]));
				stats[QUAD]++;
				sep();
			}
			else if (mas[DISCR] > 0)
			{
				cout << "Следовательно уравнение имеет 2 действительных корня\n";

				mas[ANS] = ((-1 * mas[B]) + sqrt(mas[DISCR])) / (2 * mas[A]);
				cout << "(-1 * " << mas[B] << ") + √" << mas[DISCR] << " / (2 * " << mas[A] << ") = " << mas[ANS] << endl;
				mas[X2] = ((-1 * mas[B]) - sqrt(mas[DISCR])) / (2 * mas[A]);
				cout << "(-1 * " << mas[B] << ") - √" << mas[DISCR] << " / (2 * " << mas[A] << ") = " << mas[X2] << endl;

				cout << "x1 = " << mas[ANS] << endl;
				cout << "x2 = " << mas[X2] << endl;

				AddHistory("(-1 * " + FormatFloat(mas[B]) + ") + √" + FormatFloat(mas[DISCR]) + " / (2 * " + FormatFloat(mas[A]) + ") = " + FormatFloat(mas[ANS]));
				AddHistory("(-1 * " + FormatFloat(mas[B]) + ") - √" + FormatFloat(mas[DISCR]) + " / (2 * " + FormatFloat(mas[A]) + ") = " + FormatFloat(mas[X2]));
				stats[QUAD]++;
				sep();
			}
			else
			{
				cout << "Следовательно уравнение не имеет действительных корней\n";

				AddHistory("Нет действительных корней в уравнении: " + FormatFloat(mas[A]) + "x^^ + " + FormatFloat(mas[B]) + "x + " + FormatFloat(mas[C]) + " = 0");
				stats[QUAD]++;
				sep();
			}
		}
	}
	void PyTh()
	{
		ClearCMD();
		sep();
		char num;

		cout << "Теорема Пифагора: AB^^2 + BC^^2 = AC^^2\n";
		cout << "Катет - 1\n";
		cout << "Гипотенуза - 2\n";
		cout << "Выберете, что хотите найти: ";
		num = _getch();

		if (num == '1')
		{
			ClearCMD();
			sep();
			cout << "Теорема Пифагора: BC^^2 = AC^^2 - AB^^2\n";
			cout << "Выберете, что хотите найти: " << num << endl;


			cout << "Введите длину гипотенузы: ";
			mas[C] = abs(GetFloat());

			cout << "Введите длину катета: ";
			mas[A] = abs(GetFloat());

			while (mas[A] > mas[C])
			{
				cout << "Катет не может быть больше гипотенузы, введите заново: ";
				mas[A] = abs(GetFloat());
			}

			mas[ANS] = (pow(mas[C], 2) - pow(mas[A], 2));

			cout << "BC^^2 = " << pow(mas[C], 2) << " - " << pow(mas[A], 2) << " = " << mas[ANS] << endl;
			cout << "BC = √" << mas[ANS] << " или же " << sqrt(mas[ANS]);

			stats[PYTH]++;
			AddHistory("BC = √" + FormatFloat(mas[ANS]) + " или же " + FormatFloat(sqrt(mas[ANS])));
		}
		else if (num == '2')
		{
			ClearCMD();
			sep();
			cout << "Теорема Пифагора: AB^^2 + BC^^2 = AC^^2\n";
			cout << "Выберете, что хотите найти: " << num << endl;

			cout << "Введите длину катета: ";
			mas[A] = abs(GetFloat());

			cout << "Введите длину второго катета: ";
			mas[B] = abs(GetFloat());

			mas[ANS] = pow(mas[A], 2) + pow(mas[B], 2);

			cout << "AC^^2 = " << pow(mas[A], 2) << " + " << pow(mas[B], 2) << " = " << mas[ANS] << endl;
			cout << "AC = √" << mas[ANS] << " или же " << sqrt(mas[ANS]);

			AddHistory("AC = √" + FormatFloat(mas[ANS]) + " или же " + FormatFloat(sqrt(mas[ANS])));
			stats[PYTH]++;
		}
		else
		{
			cout << "\033[91mОшибка: неправильный ввод!\033[0m\n";
			stats[ERR]++;
		}
	}

	// история
	void AddHistory(string op)
	{
		history.push_back(op);
		if (history.size() > 10)
		{
			history.erase(history.begin());
		}
	}
	void Save(string cmd, string outs)
	{
		if (cmd == "history")
		{
			ofstream outFile("history.txt");
			if (!outFile.is_open())
			{
				cerr << "\n\033[91mОшибка: не удалось открыть файл для записи истории\033[0m\n";
				return;
			}

			outFile << history.size() << "\n";

			for (const string& record : history)
			{
				outFile << record << "\n";
			}
			outFile.close();
			if (outs == "YES")
				cout << "\n\033[93mИстория сохранена!\033[0m\n";
		}
		else if (cmd == "stats")
		{
			ofstream outFile("stats.txt");
			if (!outFile.is_open())
			{
				cerr << "\n\033[91mОшибка: не удалось открыть файл для записи статистики\033[0m\n";
				return;
			}

			for (int i = 0; i < STATS_COUNT; i++)
			{
				outFile << stats[i] << "\n";
			}
			outFile.close();
			if (outs == "YES")
				cout << "\033[93mСтатистика сохранена!\033[0m\n";
		}
	}
	void Load(string cmd)
	{
		if (cmd == "history")
		{
			ifstream inFile("history.txt");
			if (!inFile.is_open())
			{
				return;
			}

			size_t count;
			inFile >> count;
			inFile.ignore();

			history.clear();
			history.reserve(count);

			string line;
			for (size_t i = 0; i < count; ++i)
			{
				if (getline(inFile, line))
				{
					history.push_back(line);
				}
			}
			inFile.close();
		}
		else if (cmd == "stats")
		{
			ifstream inFile("stats.txt");
			if (!inFile.is_open())
			{
				return;
			}

			for (int i = 0; i < STATS_COUNT; i++)
			{
				if (!(inFile >> stats[i]))
				{
					stats[i] = 0;
				}
			}
			inFile.close();
		}
	}

public:
	// конструктор
	Operator()
	{
		SetData();

		Load("history");
		Load("stats");

		ClearCMD();
	}
	// деструктор
	~Operator()
	{
		Save("history", "YES");
		Save("stats", "YES");

		sep();
		cout << "Выход из программы...\n";
		cout << "До встречи, пользователь!\n";
		sep();

		cout << "Спасибо за использование CalculatorCpp\n";
	}

	// is_works
	bool isRunning() const
	{
		return is_works;
	}

	// основная функция
	void Operations()
	{
		Save("history", "NO");
		Save("stats", "NO");

		SayHello();
		GetCMD();

		// выход
		if (cmd == "esc" || cmd == "0" || getchCMD == '0')
			is_works = false;
		// дебаг
		else if (cmd == "d" || cmd == "в" || getchCMD == 'd' || getchCMD == 'в')
			DebugMode();
		else if (cmd == "switch" || cmd == "s" || cmd == "ы" || getchCMD == 's' || getchCMD == 'ы')
		{
			if (stats[MODE] == 0)
			{
				stats[MODE] = 1;
				cmd = " ";
				ClearCMD();
				cout << "\033[93mРежим ввода сменён на быстрый\033[0m\n";
			}
			else if (stats[MODE] == 1)
			{
				stats[MODE] = 0;
				cmd = " ";
				ClearCMD();
				cout << "\033[93mРежим ввода сменён на стандартный\033[0m\n";
			}
		}
		else if (cmd == "h" || getchCMD == 'р' || getchCMD == 'h')
			HelpMePls();

		// прочие функции
		else if (cmd == "sum" || cmd == "1" || getchCMD == '1')
			Summa();
		else if (cmd == "sub" || cmd == "2" || getchCMD == '2')
			Minus();
		else if (cmd == "mult" || cmd == "3" || getchCMD == '3')
			Multiply();
		else if (cmd == "div" || cmd == "4" || getchCMD == '4')
			Divide();
		else if (cmd == "pow" || cmd == "5" || getchCMD == '5')
			Power();
		else if (cmd == "sqrt" || cmd == "6" || getchCMD == '6')
			Sqrt();
		else if (cmd == "quad" || cmd == "7" || getchCMD == '7')
			Quad();
		else if (cmd == "pyth" || cmd == "8" || getchCMD == '8')
			PyTh();
		// исключение
		else
		{
			ClearCMD();
			cout << "\033[91mОшибка: неизвестная команда\033[0m\n";
			stats[ERR]++;
		}
	}
};

/*
====================================
INT MAIN() {...}
====================================
*/

int main()
{
	cout << "\033[92mLoading...\033[0m\n";

	setlocale(LC_ALL, "RU"); // русский язык
	Operator op; // создание объекта

	//основной цикл
	while (op.isRunning())
	{
		op.Operations();
	}

	return 0;
}
