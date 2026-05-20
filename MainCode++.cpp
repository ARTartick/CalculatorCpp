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

//==================================
// КОНСТАНТЫ
//==================================
const string version = "3.2.5";
const int STATS_COUNT = 13;
const double pi = 3.14159265f;

//==================================
// ПЕРЕМЕННЫЕ И МАССИВЫ
//==================================

time_t session_start;

int settings[3] = { 0 };
int stats[STATS_COUNT]{ 0 };
float mas[6] = { 0.0f };

vector<string> history;

//==================================
// ENUMS
//==================================
enum MassiveNums
{
	A, B, C, ANS, X2, DISCR
};
enum StatsNums
{
	ADD, SUB, MULT, DIV, POW, SQRT, QUAD, PERC, APR, GPR, PYTH, AREA, SIN
};
enum SettingsNums
{
	ENT, CALC, ERR
};

//==================================================
// КЛАССЫ
//==================================================

class Calculator
{
protected:
	// переменные
	string cmd;
	char getchCMD;
	bool works;


	// утилиты
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
	void GetCMD()
	{
		if (settings[ENT] == 0) // standart
		{
			cin >> cmd;
		}
		else if (settings[ENT] == 1) // fast
		{
			getchCMD = _getch();
			cmd = string(1, getchCMD);
		}

		if (cmd == "d")
			settings[CALC] = -10;
	}
	void ChangeCalc(char a)
	{
		if (a == '-')
			settings[CALC] -= 1;
		else if (a == '+')
			settings[CALC] += 1;

		if (settings[CALC] < 0)
			settings[CALC] = 3;
		else if (settings[CALC] > 3)
			settings[CALC] = 0;

		ClearCMD();
		sep();
	}
	void SwitchEnterMode()
	{
		if (settings[ENT] == 0)
		{
			settings[ENT] = 1;
			ClearCMD();
			sep();
			cout << "\033[92mБыстрый режим ввода\033[0m\n";
			cmd = " ";
			getchCMD = ' ';
		}
		else if (settings[ENT] == 1)
		{
			settings[ENT] = 0;
			ClearCMD();
			sep();
			cout << "\033[92mСтандартный режим ввода\033[0m\n";
			cmd = " ";
			getchCMD = ' ';
		}
	}
	float GetFloat()
	{
		float val;

		while (true)
		{
			cin >> val;

			if (cin.fail())
			{
				cout << "\033[91mОшибка: введите число\033[0m\n";
				settings[ERR]++;

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

	// история и сохрания/загрузка
	void Load()
	{
		// HISTORY
		{
			ifstream inFile("history.txt");
			if (inFile.is_open())
			{
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
		}

		//=====================================
		// STATS
		{
			ifstream inFile("stats.txt");
			if (inFile.is_open())
			{
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

		//========================================
		// SETTINGS
		{
			ifstream inFile("settings.txt");
			if (inFile.is_open())
			{
				for (int i = 0; i < 3; i++)
				{
					if (!(inFile >> settings[i]))
					{
						settings[i] = 0;
					}
				}
				inFile.close();
			}
		}
	}
	void AddHistory(string op)
	{
		history.push_back(op);
		if (history.size() > 10)
		{
			history.erase(history.begin());
		}
	}
	void Save(string outs)
	{
		// HISTORY
		ofstream outFileHist("history.txt");
		if (!outFileHist.is_open())
		{
			cerr << "\n\033[91mОшибка: не удалось открыть файл для записи истории\033[0m\n";
			return;
		}

		outFileHist << history.size() << "\n";

		for (const string& record : history)
		{
			outFileHist << record << "\n";
		}
		outFileHist.close();
		if (outs == "YES")
			cout << "\n\033[93mИстория сохранена!\033[0m\n";
		

		//================================================
		// STATS
		ofstream outFileSt("stats.txt");
		if (!outFileSt.is_open())
		{
			cerr << "\n\033[91mОшибка: не удалось открыть файл для записи статистики\033[0m\n";
			return;
		}

		for (int i = 0; i < STATS_COUNT; i++)
		{
			outFileSt << stats[i] << "\n";
		}
		outFileSt.close();
		if (outs == "YES")
			cout << "\033[93mСтатистика сохранена!\033[0m\n";
		

		//========================================================
		// SETTINGS
		ofstream outFileSet("settings.txt");
		if (!outFileSet.is_open())
		{
			cerr << "\n\033[91mОшибка: не удалось открыть файл для записи настроек\033[0m\n";
			return;
		}

		for (int i = 0; i < 3; i++)
		{
			outFileSet << settings[i] << "\n";
		}
		outFileSet.close(); 
		if (outs == "YES")
			cout << "\033[93mНастройки сохранены!\033[0m\n";
	}
};

class ArithmeticCalc : public Calculator
{
private:

	void CommandList()
	{
		cout << "\033[93mАРИФМЕТИЧЕСКИЙ КАЛЬКУЛЯТОР\033[0m\n\n";
		cout << "Список доступных комманд:\n\n";
		cout << "Сложение:                  [1]\n";
		cout << "Вычитание:                 [2]\n";
		cout << "Умножение:                 [3]\n";
		cout << "Деление:                   [4]\n";
		cout << "Степень:                   [5]\n";
		cout << "Квадратный корень:         [6]\n";

		cout << endl;
		cout << "Смена калькулятора:    [-]/[+]\n";
		cout << "Смена режима ввода:        [s]\n";
		cout << "Выход:                 [e]/[0]\n";
	}

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
	void Substraction()
	{
		ClearCMD();
		sep();

		cout << "Вычитание: а - b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		mas[ANS] = mas[A] - mas[B];
		cout << "Ответ: " << mas[A] << " - " << mas[B] << " = " << mas[ANS] << endl;

		stats[SUB]++;
		AddHistory(FormatFloat(mas[A]) + " - " + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void Multiply()
	{
		ClearCMD();
		sep();

		cout << "Умножение: а * b\n";
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
	void Division()
	{
		ClearCMD();
		sep();

		cout << "Деление: а / b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();
		while (mas[B] == 0)
		{
			cout << "Число b не может быть равным 0\n";
			cout << "Повторите ввод: ";
			mas[B] = GetFloat();
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

		cout << "Возведение в степень: а^b\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();
		while (mas[A] == 0 && mas[B] < 0)
		{
			cout << "\033[91mОшибка: 0 нельзя возводить в отрицательную степень\033[0m\n";
			cout << "Введите а снова: ";
			mas[A] = GetFloat();
			cout << "Введите b снова: ";
			mas[B] = GetFloat();
		}

		mas[ANS] = pow(mas[A], mas[B]);
		cout << "Ответ: " << mas[A] << "^" << mas[B] << " = " << mas[ANS] << endl;

		stats[POW]++;
		AddHistory(FormatFloat(mas[A]) + "^" + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void SqRt()
	{
		ClearCMD();
		sep();

		cout << "Корень: ?а\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		if (mas[A] < 0)
		{
			mas[ANS] = sqrt(abs(mas[A]));
			cout << "Ответ: ?" << mas[A] << " = " << mas[ANS] << "i\n";
			cout << "(Это не настоящее комплексное число, а всего лишь корень из модуля)\n";

			AddHistory("?" + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]) + "i");
		}
		else
		{
			mas[ANS] = sqrt(mas[A]);
			cout << "Ответ: ?" << mas[A] << " = " << mas[ANS] << endl;

			AddHistory("?" + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]));
		}

		stats[SQRT]++;
		sep();
	}

public:
	ArithmeticCalc()
	{
		Load();

		session_start = time(nullptr);
		cout << "\033[92mLoading done\033[0m\n";
		ClearCMD();
		sep();
	}
	~ArithmeticCalc()
	{
		Save("YES");

		sep();
		cout << "Выход из программы...\n";
		cout << "До встречи, пользователь!\n";
		sep();

		cout << "Спасибо за использование CalculatorCpp\n";
	}

	bool MainCode()
	{
		Save("NO");

		CommandList();
		cout << "\nВведите команду: ";
		GetCMD();
		if (cmd == "0" || cmd == "e")
			return false;
		else if (cmd == "s")
			SwitchEnterMode();

		else if (cmd == "1")
			Summa();
		else if (cmd == "2")
			Substraction();
		else if (cmd == "3")
			Multiply();
		else if (cmd == "4")
			Division();
		else if (cmd == "5")
			Power();
		else if (cmd == "6")
			SqRt();
		else if (cmd == "-")
			ChangeCalc('-');
		else if (cmd == "+")
			ChangeCalc('+');

		else
		{
			cout << "\033[91mОшибка: неизвестная команда\033[0m\n";
		}
		
		return true;
	}
};

class Algebra : public Calculator
{
private:
	void CommandList()
	{
		cout << "\033[93mАЛГЕБРАИЧЕСКИЙ КАЛЬКУЛЯТОР\033[0m\n\n";
		cout << "Список доступных комманд:\n\n";
		cout << "Квадратные уравнения:      [1]\n";
		cout << "Проценты:                  [2]\n";
		cout << "Арифметическая прогрессия: [3]\n";
		cout << "Геометрическая прогрессия: [4]\n";

		cout << endl;
		cout << "Смена калькулятора:    [-]/[+]\n";
		cout << "Смена режима ввода:        [s]\n";
		cout << "Выход:                 [e]/[0]\n";
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

				AddHistory("Нет действительных корней в уравнении: " + FormatFloat(mas[A]) + "x^2 + " + FormatFloat(mas[B]) + "x + " + FormatFloat(mas[C]) + " = 0");
				stats[QUAD]++;
				sep();
			}
		}
	}
	void Persentage()
	{
		ClearCMD();
		sep();

		cout << "b% от числа a\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();
		cout << "Введите b: ";
		mas[B] = GetFloat();

		mas[ANS] = (mas[A] / 100) * mas[B];
		cout << "Ответ: " << mas[B] << "% от числа " << mas[A] << " равняется " << mas[ANS] << endl;

		stats[PERC]++;
		AddHistory(FormatFloat(mas[B]) + "% от числа " + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}
	void ArProgress()
	{
		ClearCMD();
		sep();

		cout << "Арифметическая прогрессия:\n";
		cout << "[1] Найти n член\n";
		cout << "[2] Найти сумму первых n членов\n";

		cout << "Выберите действие: ";
		GetCMD();

		if (cmd == "1")
		{
			ClearCMD();
			sep();

			cout << "Формула: a_n = a_1 + d(n-1)\n";
			cout << "Введите первый член последовательности (a_1): ";
			mas[A] = GetFloat();
			cout << "Введите разность последовательности (d): ";
			mas[B] = GetFloat();
			cout << "Введите какой член нужно найти (n): ";
			mas[C] = GetFloat();

			if (round(mas[C]) != mas[C])
			{
				cout << "n (" << mas[C] << ") было округлено до " << round(mas[C]) << endl;
				mas[C] = round(mas[C]);
			}

			mas[ANS] = mas[A] + (mas[B] * (mas[C] - 1));
			stats[APR]++;
			cout << "Ответ: a_" << mas[C] << " = " << mas[A] << " + " << mas[B] << "(" << mas[C] << "-1) = " << mas[ANS];
			AddHistory("a_" + FormatFloat(mas[C]) + " = " + FormatFloat(mas[A]) + " + " + FormatFloat(mas[B]) + "(" + FormatFloat(mas[C]) + " - 1) = " + FormatFloat(mas[ANS]));
		}

		else if (cmd == "2")
		{
			ClearCMD();
			sep();

			cout << "Формула: S_n = ((2а_1 + d(n-1)) / 2) * n\n";

			cout << "Введите первый член последовательности (а_1): ";
			mas[A] = GetFloat();
			cout << "Введите разность последовательности (d): ";
			mas[B] = GetFloat();
			cout << "Введите n: ";
			mas[C] = GetFloat();

			if (round(mas[C]) != mas[C])
			{
				cout << "n (" << mas[C] << ") было округлено до " << round(mas[C]) << endl;
				mas[C] = round(mas[C]);
			}

			mas[ANS] = (((2*mas[A]) + (mas[B]*(mas[C] - 1))) / 2) * mas[C];
			stats[APR]++;
			cout << "Ответ: S_" << mas[C] << " = ((2*" << mas[A] << " + " << mas[B] << "(" << mas[C] << " - 1)) / 2) * " << mas[C] << " = " << mas[ANS] << endl;
			AddHistory("S_" + FormatFloat(mas[C]) + " = ((2 * " + FormatFloat(mas[A]) + " + " + FormatFloat(mas[B]) + "(" + FormatFloat(mas[C]) + " - 1)) / 2) * " + FormatFloat(mas[C]) + " = " + FormatFloat(mas[ANS]));
		}
		else
		{
			cout << "\033[91mОшибка: неправильный ввод!\033[0m\n";
			stats[ERR]++;
		}

		sep();
	}
	void GeoProg()
	{
		ClearCMD();
		sep();

		cout << "Геометрическая прогрессия:\n";
		cout << "[1] Найти n член\n";
		cout << "[2] Найти сумму первых n членов\n";

		cout << "Выберите действие: ";
		GetCMD();

		if (cmd == "1")
		{
			ClearCMD();
			sep();

			cout << "Формула: b_n = b_1 * q^(n-1)\n";
			cout << "Введите первый член последовательности (b_1): ";
			mas[A] = GetFloat();
			cout << "Введите знаменатель последовательности (q): ";
			mas[B] = GetFloat();
			cout << "Введите какой член нужно найти (n): ";
			mas[C] = GetFloat();

			if (round(mas[C]) != mas[C])
			{
				cout << "n (" << mas[C] << ") было округлено до " << round(mas[C]) << endl;
				mas[C] = round(mas[C]);
			}

			mas[ANS] = mas[A] * pow(mas[B], (mas[C]-1));
			stats[GPR]++;
			cout << "Ответ: b_" << mas[C] << " = " << mas[A] << " * " << mas[B] << "^(" << mas[C] << "-1) = " << mas[ANS] << endl;
			AddHistory("b_" + FormatFloat(mas[C]) + " = " + FormatFloat(mas[A]) + " * " + FormatFloat(mas[B]) + "^(" + FormatFloat(mas[C]) + " - 1) = " + FormatFloat(mas[ANS]));
		}

		else if (cmd == "2")
		{
			ClearCMD();
			sep();

			cout << "Формула: S_n = (b_1 * (q^n - 1)) / (q - 1)\n";

			cout << "Введите первый член последовательности (b_1): ";
			mas[A] = GetFloat();
			cout << "Введите знаменатель последовательности (q): ";
			mas[B] = GetFloat();

			while (mas[B] == 1)
			{
				cout << "\033[91mq не может равняться 1\033[0m\n";
				mas[B] = GetFloat();
			}
			cout << "Введите n: ";
			mas[C] = GetFloat();

			if (round(mas[C]) != mas[C])
			{
				cout << "n (" << mas[C] << ") было округлено до " << round(mas[C]) << endl;
				mas[C] = round(mas[C]);
			}

			mas[ANS] = (mas[A] * (pow(mas[B], mas[C]) - 1)) / (mas[B] - 1);
			stats[GPR]++;
			cout << "Ответ: S_" << mas[C] << " = (" << mas[A] << " * " << "(" << mas[B] << "^" << mas[C] << " - 1)) / (" << mas[B] << " - 1) = " << mas[ANS] << endl;
			AddHistory("S_" + FormatFloat(mas[C]) + " = (" + FormatFloat(mas[A]) + " * (" + FormatFloat(mas[B]) + "^" + FormatFloat(mas[C]) + " - 1)) / (" + FormatFloat(mas[B]) + " - 1) = " + FormatFloat(mas[ANS]));
		}
		else
		{
			cout << "\033[91mОшибка: неправильный ввод!\033[0m\n";
			stats[ERR]++;
		}
		sep();
	}

public:
	bool MainCode()
	{
		Save("NO");

		CommandList();

		cout << "\nВведите команду: ";
		GetCMD();

		if (cmd == "0" || cmd == "e")
			return false;
		else if (cmd == "s")
			SwitchEnterMode();

		else if (cmd == "-")
			ChangeCalc('-');
		else if (cmd == "+")
			ChangeCalc('+');

		else if (cmd == "1")
			Quad();
		else if (cmd == "2")
			Persentage();
		else if (cmd == "3")
			ArProgress();
		else if (cmd == "4")
			GeoProg();

		else
		{
			cout << "\033[91mОшибка: неизвестная команда\033[0m\n";
		}

		return true;
	}
};

class Geometry : public Calculator
{
private:
	void CommandList()
	{
		cout << "\033[93mГЕОМЕТРИЧЕСКИЙ КАЛЬКУЛЯТОР\033[0m\n\n";
		cout << "Список доступных комманд:\n\n";
		cout << "Теорема Пифагора:          [1]\n";
		cout << "Многоугольники:            [2]\n";
		cout << "Окружности, дуги:          [3]\n";

		cout << endl;
		cout << "Смена калькулятора:    [-]/[+]\n";
		cout << "Смена режима ввода:        [s]\n";
		cout << "Выход:                 [e]/[0]\n";
	}

	void PyTh()
	{
		ClearCMD();
		sep();
		char num;

		cout << "Теорема Пифагора: AB^2 + BC^2 = AC^2\n";
		cout << "Катет - 1\n";
		cout << "Гипотенуза - 2\n";
		cout << "Выберете, что хотите найти: ";
		num = _getch();

		if (num == '1')
		{
			ClearCMD();
			sep();
			cout << "Теорема Пифагора: BC^2 = AC^2 - AB^2\n";
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

			cout << "BC^2 = " << pow(mas[C], 2) << " - " << pow(mas[A], 2) << " = " << mas[ANS] << endl;
			cout << "BC = √" << mas[ANS] << " или же " << sqrt(mas[ANS]);

			stats[PYTH]++;
			AddHistory("BC = √" + FormatFloat(mas[ANS]) + " или же " + FormatFloat(sqrt(mas[ANS])));
		}
		else if (num == '2')
		{
			ClearCMD();
			sep();
			cout << "Теорема Пифагора: AB^2 + BC^2 = AC^2\n";
			cout << "Выберете, что хотите найти: " << num << endl;

			cout << "Введите длину катета: ";
			mas[A] = abs(GetFloat());

			cout << "Введите длину второго катета: ";
			mas[B] = abs(GetFloat());

			mas[ANS] = pow(mas[A], 2) + pow(mas[B], 2);

			cout << "AC^2 = " << pow(mas[A], 2) << " + " << pow(mas[B], 2) << " = " << mas[ANS] << endl;
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
	void Polygon()
	{
		ClearCMD();
		sep();

		cmd = "null";

		cout << "Площади:\n";
		cout << "паралелограмма:  [1]\n";
		cout << "треугольника:    [2]\n";
		cout << "трапеции:        [3]\n\n";

		cout << "Вернуться:       [0]\n";

		cout << "Ввод: ";
		GetCMD();

		if (cmd == "0")
		{
			ClearCMD();
			sep();

			return;
		}
		else if (cmd == "1")
		{
			ClearCMD();
			sep();

			cout << "Площадь паралелограмма: a*h\n";
			cout << "Введите а: ";
			mas[A] = abs(GetFloat());
			cout << "Введите h: ";
			mas[B] = abs(GetFloat());

			mas[ANS] = mas[A] * mas[B];
			cout << "Ответ: " << mas[A] << "*" << mas[B] << " = " << mas[ANS] << endl;
			AddHistory(FormatFloat(mas[A]) + "*" + FormatFloat(mas[B]) + " = " + FormatFloat(mas[ANS]));
		}
		else if (cmd == "2")
		{
			ClearCMD();
			sep();

			cout << "Площадь треугольника: (a*h) / 2\n";
			cout << "Введите а: ";
			mas[A] = abs(GetFloat());
			cout << "Введите h: ";
			mas[B] = abs(GetFloat());

			mas[ANS] = (mas[A]*mas[B])/2;
			cout << "Ответ: (" << mas[A] << "*" << mas[B] << ") / 2 = " << mas[ANS] << endl;
			AddHistory("("+FormatFloat(mas[A]) + "*" + FormatFloat(mas[B]) + ") / 2 = " + FormatFloat(mas[ANS]));
		}
		else if (cmd == "3")
		{
			ClearCMD();
			sep();

			cout << "Площадь трапеции: ((a+b)/2) * h\n";
			cout << "Введите а: ";
			mas[A] = abs(GetFloat());
			cout << "Введите b: ";
			mas[B] = abs(GetFloat());
			cout << "Введите h: ";
			mas[C] = abs(GetFloat());

			mas[ANS] = ((mas[A] + mas[B]) / 2) * mas[C];
			cout << "Ответ: ((" << mas[A] << "+" << mas[B] << ")/2) * " << mas[C] << " = " << mas[ANS] << endl;
			AddHistory("((" + FormatFloat(mas[A]) + "+" + FormatFloat(mas[B]) + ")/2) * " + FormatFloat(mas[C]) + " = " + FormatFloat(mas[ANS]));
		}
		stats[AREA]++;
	}
	void Rounds()
	{

		ClearCMD();
		sep();

		cmd = "null";

		cout << "Окружности и дуги:\n";
		cout << "Периметр круга:  [1]\n";
		cout << "Площадь круга:   [2] (in progress...)\n";
		cout << "Длина дуги:      [3] (in progress...)\n";
		cout << "Площадь сектора: [4] (in progress...)\n";

		cout << "\nВернуться:       [0]\n";

		cout << "Ввод: ";
		GetCMD();

		if (cmd == "0")
		{
			ClearCMD();
			sep();

			return;
		}
		else if (cmd == "1")
		{
			ClearCMD();
			sep();

			cout << "Периметр круга: P = 2*pi*r\npi = ~3,14\n";
			cout << "Введите r (радиус): ";
			mas[A] = GetFloat();

			mas[ANS] = 2*mas[A]*pi;
			cout << "Ответ: 2 * 3,14 * " << mas[A] << " = " << mas[ANS] << endl;
			AddHistory("2 * 3,14 * " + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]));
		}
	}

public:
	bool MainCode()
	{
		Save("NO");
		CommandList();
		cout << "\nВведите команду: ";
		GetCMD();
		if (cmd == "0" || cmd == "e")
			return false;
		else if (cmd == "s")
			SwitchEnterMode();

		else if (cmd == "-")
			ChangeCalc('-');
		else if (cmd == "+")
			ChangeCalc('+');

		else if (cmd == "1")
			PyTh();
		else if (cmd == "2")
			Polygon();
		else if (cmd == "3")
			Rounds();

		else
		{
			cout << "\033[91mОшибка: неизвестная команда\033[0m\n";
		}

		return true;
	}
};

class Trigonometry : public Calculator
{
	void CommandList()
	{
		cout << "\033[93mТРИГОНОМЕТРИЧЕСКИЙ КАЛЬКУЛЯТОР\033[0m\n\n";
		cout << "Список доступных комманд:\n\n";
		cout << "Синус:                     [1]\n";

		cout << endl;
		cout << "Смена калькулятора:    [-]/[+]\n";
		cout << "Смена режима ввода:        [s]\n";
		cout << "Выход:                 [e]/[0]\n";
	}

	void Sinus()
	{
		ClearCMD();
		sep();

		cout << "Синус: sin a\n";
		cout << "Введите а: ";
		mas[A] = GetFloat();

		mas[ANS] = sin(mas[A]);
		cout << "Ответ: sin" << mas[A] << " = " << mas[ANS] << endl;

		stats[SIN]++;
		AddHistory("sin" + FormatFloat(mas[A]) + " = " + FormatFloat(mas[ANS]));
		sep();
	}

public:
	bool MainCode()
	{
		Save("NO");
		CommandList();
		cout << "\nВведите команду: ";
		GetCMD();
		if (cmd == "0" || cmd == "e")
			return false;
		else if (cmd == "s")
			SwitchEnterMode();

		else if (cmd == "-")
			ChangeCalc('-');
		else if (cmd == "+")
			ChangeCalc('+');

		else if (cmd == "1")
			Sinus();

		else
		{
			cout << "\033[91mОшибка: неизвестная команда\033[0m\n";
		}

		return true;
	}
};

class DebugMode : public Calculator
{
private:

	bool inDebug = true;

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
			a = a + stats[i];
		}

		cout << "Total operations: " << a << endl;
		cout << "Addition: " << stats[ADD] << endl;
		cout << "Substraction: " << stats[SUB] << endl;
		cout << "Multiplication: " << stats[MULT] << endl;
		cout << "Division: " << stats[DIV] << endl;
		cout << "Power: " << stats[POW] << endl;
		cout << "Square Root: " << stats[SQRT] << endl << endl;
		
		cout << "Quadratic Equation: " << stats[QUAD] << endl;
		cout << "Percentage: " << stats[PERC] << endl;
		cout << "Arithmetic Progression: " << stats[APR] << endl;
		cout << "Geometrical Progression: " << stats[GPR] << endl << endl;
		
		cout << "Pythagorean Theorem: " << stats[PYTH] << endl;
		cout << "Areas: " << stats[AREA] << endl; 
		
		cout << "Sinus: " << stats[SIN] << endl;
	}
	void GeneralStats()
	{
		cout << "\n\033[93m=GeneralStats\033[0m\n";

		time_t now = time(nullptr);
		double durat = difftime(now, session_start);

		int minutes = (int)durat / 60;
		int secondes = (int)durat % 60;
		int total = 0;
		for (int i = 0; i < STATS_COUNT; i++)
		{
			total = total + stats[i];
		}

		cout << "Programming language: C++\n";
		cout << "Name: CALCULATORcpp\n";
		cout << "Version: " << version << endl;
		cout << "Session duration: " << minutes << "min " << secondes << "sec" << endl;
		cout << "Enter mode: ";
		switch (settings[ENT])
		{
		case 0: cout << "standart\n"; break;
		case 1: cout << "fast\n"; break;
		}
		cout << "Total operations: " << total << endl;
		cout << "Total errors: " << settings[ERR] << endl;
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

public:
	void MainCode()
	{
		ClearCMD();
		sep();
		cout << "\033[93mYOU ENTERED DEBUG-MODE\033[0m\n";
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
			default: cout << "\033[91mError: Incorrect cmd\033[0m\n"; settings[ERR]++;
			}
		}

		settings[CALC] = 0;
	}
};

//====================================================
// ФУНКЦИИ
//====================================================

int main()
{
	cout << "\033[92mLoading has started...\033[0m\n";

	setlocale(LC_ALL, "RU");

	cout << "\033[92mLoading...\033[0m\n";
	bool is_works = true;

	Algebra al;
	Geometry gy;
	Trigonometry ty;
	DebugMode dm;
	ArithmeticCalc ac;

	while (is_works)
	{
		if (settings[CALC] == 0)
			is_works = ac.MainCode();
		else if (settings[CALC] == 1)
			is_works = al.MainCode();
		else if (settings[CALC] == 2)
			is_works = gy.MainCode();
		else if (settings[CALC] == 3)
			is_works = ty.MainCode();
		else if (settings[CALC] == -10)
			dm.MainCode();
	}

	return 0;
}
