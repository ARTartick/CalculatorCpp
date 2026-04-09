#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

/*
====================================
КОНСТАНТЫ И ПЕРЕМЕННЫЕ
====================================
*/

const string version = "2.0.0"; // версия


//переменные
time_t session_start;
vector<string> History;
bool is_works;

// Перечисления
enum MassiveNumbers
{
	A, B, C, ANS, X2, DISCR
};
enum StatNumbers
{
	ADD, MIN, MULT, DIV, ERR, POW, SQRT, QUAD, PYTH
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
	cout << "Выход:                     [esc]\n";
	cout << "--------------------------------\n";
	cout << "Введите команду: ";
}

// класс
class Operator
{
private:
	// переменные
	string cmd;
	char debugCMD;
	bool inDebug;

	// массивы
	int stats[9];
	float mas[6];

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
	void AddHistory(string op)
	{
		History.push_back(op);
		if (History.size() > 10)
		{
			History.erase(History.begin());
		}
	}
	void DebugMode()
	{
		ClearCMD();
		sep();
		cout << "YOU ENTERED DEBUG-MODE\n";
		inDebug = true;

		while (inDebug)
		{
			sep();

			cout << "DEBUG MODE MENU";
			cout << "\n[1] Просмотр массива\n";
			cout << "[2] Статистика вычислений\n";
			cout << "[3] Общая статистика\n";
			cout << "[4] История вычислений\n";
			cout << "[0] Выход из debug mode\n";
			cout << "Введите команду: ";
			cin >> debugCMD;
			ClearCMD();

			switch (debugCMD)
			{
			case '0': QuitDebugMode(); break;
			case '1': ShowArray(); break;
			case '2': CalcStats(); break;
			case '3': GeneralStats(); break;
			case '4': ShowHistory(); break;
			default: cout << "\033[91mError: Incorrect cmd\033[0m\n"; stats[ERR]++;
			}
		}
	}

	// команды дебага
	void ShowArray()
	{
		cout << "\n=ShowArray\n";

		cout << "a: " << mas[A] << endl;
		cout << "b: " << mas[B] << endl;
		cout << "c: " << mas[C] << endl;
		cout << "Last Answer: " << mas[ANS] << endl;
		cout << "Last second Answer: " << mas[X2] << endl;
		cout << "Discr: " << mas[DISCR] << endl;
	}
	void CalcStats()
	{
		cout << "\n=CalcStats\n";

		cout << "Total operations: " << stats[ADD] + stats[MIN] + stats[MULT] + stats[DIV] + stats[POW] + stats[SQRT] + stats[QUAD] + stats[PYTH] << endl;
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
		cout << "\n=GeneralStats\n";

		time_t now = time(nullptr);
		double dur = difftime(now, session_start);

		int min = (int)dur / 60;
		int sec = (int)dur % 60;

		int total = stats[ADD] + stats[MIN] + stats[MULT] + stats[DIV] + stats[POW] + stats[SQRT] + stats[QUAD] + stats[PYTH];

		cout << "Programming language: C++\n";
		cout << "Name: CALCULATORcpp\n";
		cout << "Version: " << version << endl;
		cout << "Session duration: " << min << "min " << sec << "sec" << endl;
		cout << "Total operations: " << total << endl;
		cout << "Total errors: " << stats[ERR] << endl;
	}
	void ShowHistory()
	{
		cout << "\n=ShowHistory\n";

		if (History.empty())
		{
			cout << "\033[91mИстория операций пока что пуста...\033[0m" << endl;
		}
		int num = 1;
		for (auto it = History.rbegin(); it != History.rend(); ++it, ++num)
		{
			cout << "[" << num << "] " << *it << endl;
		}
	}
	void QuitDebugMode()
	{
		cout << "\n=======================\n";
		cout << "ВЫ ВЫШЛИ ИЗ DEBUG MODE";
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
	void PyTh()
	{
		ClearCMD();
		sep();
		char num;

		cout << "Теорема Пифагора: AB^^2 + BC^^2 = AC^^2\n";
		cout << "Катет - 1\n";
		cout << "Гипотенуза - 2\n";
		cout << "Выберете, что хотите найти: ";
		cin >> num;

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

public:
	// конструктор
	Operator()
	{
		is_works = true;
		session_start = time(nullptr);
	}
	// основная функция
	void Operations()
	{
		SayHello();
		cin >> cmd;

		// выход
		if (cmd == "esc" || cmd == "0")
		{
			sep();
			cout << "Выход из программы...\n";
			cout << "До встречи, пользователь!\n";
			sep();
			is_works = false;
		}
		// дебаг
		else if (cmd == "d")
			DebugMode();

		// прочие функции
		else if (cmd == "sum" || cmd == "1")
			Summa();
		else if (cmd == "sub" || cmd == "2")
			Minus();
		else if (cmd == "mult" || cmd == "3")
			Multiply();
		else if (cmd == "div" || cmd == "4")
			Divide();
		else if (cmd == "pow" || cmd == "5")
			Power();
		else if (cmd == "sqrt" || cmd == "6")
			Sqrt();
		else if (cmd == "quad" || cmd == "7")
			Quad();
		else if (cmd == "pyth" || cmd == "8")
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
	setlocale(LC_ALL, "RU"); // русский язык

	Operator op; // создание объекта

	ClearCMD();

	//основной цикл
	while (is_works)
	{
		op.Operations();
	}

	cout << "\nСпасибо за использование CalculatorCpp\n";
	return 0;
}
