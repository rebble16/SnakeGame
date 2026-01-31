// SnakeGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<vector>
#include<ctime>
#include<fstream>
#include<string>
const int WIDTH = 24;
const int HEIGHT = 15;
using namespace std;
bool IsRunning = true;
int idx(int x, int y)
{
	return y * WIDTH+ x;
}
int showRecord(const string& filename)
{
	int record = 0;
	ifstream out(filename);
	if (out.is_open() && (out>> record)) {
		out.close();
	}
	return record;
}
void saveRecord(const string& filename,int score)
{
	string rec = to_string(score);
	ofstream file(filename);
	if (file.is_open())
	{
		file << rec;
		file.close();
	}
	else cout << "Error!This file wasn't found!" << endl;
}
const char map[] =
"#######################\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#                     #\n"
"#######################\n";
struct Pos
{
	int x;
	int y;
	bool operator!=(Pos a)
	{
		if (this->x != a.x && this->y != a.y)return true;
		else return false;
	}
};
vector<Pos> Snake = { {10, 5 } };
//СПАВН ЯБЛОКА
Pos appleSpawn(vector<Pos>& snake)
{
	Pos applePos;
	bool collision = false;
	do
	{
		collision = false;
		for (auto& k : Snake)
		{
			applePos.x= rand() % (WIDTH - 3) + 1;
			applePos.y = rand() % (HEIGHT - 2) +1;
			if (k.x == applePos.x&&k.y==applePos.y)
			{
				collision = true;
				break;
			}
		}

	} while (collision);
	return applePos;
}
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//попытка реализации отображения элементов на карте разным цветом
/*void display()
{
	char TempMap[sizeof(map)];
	memcpy(TempMap, map, sizeof(TempMap));
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; j++)
		{
			int n = idx(i, j);
			char ch = TempMap[n];
			if (ch == '\n') continue;

			if (ch == '*')
			{
				SetConsoleTextAttribute(hConsole, 14);
			}
			else if (ch == '0' || ch == 'o')
			{
				SetConsoleTextAttribute(hConsole, 10);
			}
			else SetConsoleTextAttribute(hConsole, 8);
			cout << ch;
		}
	}
	SetConsoleTextAttribute(hConsole, 7);
	cout << '\n';
}*/
char reset_map[sizeof(map)];
int main()
{
	srand(time(NULL));
	string file = "MyRecord.txt";//файл куда сохраняется рекорд
	COORD position = { 0,0 }; //позиция курсора
	SetConsoleCursorPosition(hConsole, position);//установка курсора в начало
	Pos newHead;//координаты головы змейки
	int lenght = 1;//длина змейки
	char dir = ' ';//направление змейки, считанное с клавиатуры
	char apple = '*';//обозначение яблока на карте
	//НЕОБХОДИМО СОХРАНЯТЬ ЗМЕЙКУ И КАРТУ В ОТДЕЛЬНЫХ ПЕРЕМЕННЫХ
	bool apple_exist = false;//переменная для генерации яблока на карте
	Pos appleCOORD = { 0,0 };//кооринаты яблока на карте
	bool grow = false;//переменная для отслеживания роста змейки
	char dir_past = ' ';//направление змейки до считывания с клавиши, чтобы змейка не могла ползти в противоположную сторону
	memcpy(reset_map, map, sizeof(reset_map));//Скопируем карту,чтобы обновлятьs её для каждого кадра
	while (IsRunning)//основной цикл игры
	{
		//поедание яблока
		if (Snake[0].x == appleCOORD.x && Snake[0].y == appleCOORD.y)
		{
			lenght++;
			apple_exist = false;
			grow = true;
		}
		//считывание с клавиатуры
		if (_kbhit())
		{
			char key = _getch();
			if (key == 0 || key == 224) {
				key = _getch();
				if (key == 75 && dir != 'd') dir = 'a';
				if (key == 72 && dir != 's') dir = 'w';
				if (key == 77 && dir != 'a') dir = 'd';
				if (key == 80 && dir != 'w') dir = 's';
			}
			else {
				if ((key == 'a' || key == 'A') && dir != 'd') dir = 'a';
				if ((key == 'w' || key == 'W') && dir != 's') dir = 'w';
				if ((key == 's' || key == 'S') && dir != 'w') dir = 's';
				if ((key == 'd' || key == 'D') && dir != 'a') dir = 'd';
			}
		}
		//передвижениие змейки по карте
		newHead = Snake[0];
		if (dir == 'w')newHead.y--;
		if (dir == 's')newHead.y++;
		if (dir == 'a')newHead.x--;
		if (dir == 'd')newHead.x++;
		//проверка на достижение границ змейкой(если врезается,то игра окончена)
		if (reset_map[idx(newHead.x, newHead.y)] == '#')
			{
				IsRunning = false;
				break;
			}
		//увеличение тела змейки после поедания яблока
		Snake.insert(Snake.begin(), newHead);//обновление позиции головы на карте
		if (!grow)
		{
			Snake.pop_back();
		}//УДАЛИТЬ хвост ТОЛЬКО если НЕ растём
			else grow = false;
			//ПРОВЕРКА НА СТОЛКНОВЕНИЕ С ТЕЛОМ
			for (int k = 1; k < Snake.size() - 1; k++)
			{

				if (idx(Snake[k].x, Snake[k].y) == idx(Snake[0].x, Snake[0].y))
				{
					IsRunning = false;
					break;
				}
			}
			memcpy(reset_map, map, sizeof(reset_map));//Скопируем карту,чтобы обновлять её для каждого кадра
			reset_map[idx(Snake[0].x, Snake[0].y)] = '0';//привязывание головы змейки к координатам

			if (!apple_exist)//если на карте нет яблока,то спавним его
			{
				appleCOORD = appleSpawn(Snake);//считывание координат яблока с функции, которая рандомизирует его положение
				apple_exist = true;
			}
			for (int i = 0; i < WIDTH - 1; i++)
			{
				for (int j = 0; j < HEIGHT - 1; j++)
				{
					if (reset_map[idx(i, j)] != '#')reset_map[idx(appleCOORD.x, appleCOORD.y)] = '*';//привязывание яблока к координатам
				}
			}
			for (int k = 1; k < Snake.size(); k++)reset_map[idx(Snake[k].x, Snake[k].y)] = 'o';//привязывание тела змейки к координатам(без головы,голвоу мы отобразили отдельно)

			
			cout << reset_map;//отображение всех элементов на карте
			cout << "Score:" << lenght;//отображение очков игрока
			Sleep(100);
			system("cls");
		}
		if(showRecord(file)<lenght)saveRecord(file,lenght);//сохранение рекорда
		Sleep(200);
		cout << "Game over!" << endl;//окончание игры
		cout << "Your final score:" << lenght << endl;//отображение выбитых очков игроком
		cout<<"Your record:"<<showRecord(file);//отображение рекорда
		return 0;
	}
