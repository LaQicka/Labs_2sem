#include "MyList.h"
#include "MyList.cpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>

using namespace std;

int c = 800;

struct Option {
	int WindowLength;
	int WindowWidth;
	int maxLength;
	double scale;
	int displacement;
	int step;
	sf::Color color;
};

void info() { // Функция, показывающая информацию о интерфейсе программы
	system("cls");
	cout << "There are 4 buttons on the screen:\n";
	cout << "Green - save your art or options\n";
	cout << "Red - cancel last step\n";
	cout << "Blue - load your art or options\n";
	cout << "Yellow - restart your art\n";
}

Option option; // Переменная, хранящая главные настройки сеанса программы

int main()
{
	sf::Font font;
	font.loadFromFile("arial.ttf");
	MyList <pair<int, int>> ar;
	option.WindowLength = c, option.WindowWidth = c;
	option.color = sf::Color::Blue;
	option.scale = 1;
	option.step = 50;
	cout << "Enter max length of stitch\n";
	cin >> option.maxLength;
	info();

	sf::RenderWindow window(sf::VideoMode(option.WindowLength, option.WindowWidth), "Your art");
	pair<int,int> start(make_pair(0, option.WindowWidth)), it(make_pair(0, option.WindowWidth));

	sf::RectangleShape save(sf::Vector2f(100, 50)); // Фигура, которая будет выполнять функцию кнопки по сохранению рисунка или настроек в файл
	save.setPosition(sf::Vector2f(0, 0));
	save.setFillColor(sf::Color::Green);

	sf::RectangleShape cancel(sf::Vector2f(100, 50)); // Фигура, которая будет выполнять функцию кнопки по отмене последнего действия
	cancel.setPosition(sf::Vector2f(150, 0));
	cancel.setFillColor(sf::Color::Red);

	sf::RectangleShape load(sf::Vector2f(100, 50)); // Фигура, которая будет выполнять функцию кнопки по загрузке рисунка или настроек из файла
	load.setPosition(sf::Vector2f(300, 0));
	load.setFillColor(sf::Color::Blue);

	sf::RectangleShape restart(sf::Vector2f(100, 50)); // Фигура, которая будет выполнять функцию кнопки для рестарта рисунка
	restart.setPosition(sf::Vector2f(450, 0));
	restart.setFillColor(sf::Color::Yellow);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
				sf::Vector2i cur_pos = sf::Mouse::getPosition(window); // Получение позиции мыши относительно окна рисунка

				if (cur_pos.x <= 100 && cur_pos.y <= 50) {
					cout << "If you want to save file enter 1, else if you want to save options enter 0\n";
					bool flag;
					cin >> flag;
					if (flag) {
						cout << "Enter name of file\n";
						string name;
						cin >> name;
						name += ".txt";
						ofstream out;
						out.open(name);
						for (int i = 0; i < ar.get_size(); i++)out << ar[i].first << " " << ar[i].second << '\n';
						out.close();
						cout << "Your file saved as " << name << '\n';
					}
					else {
						ofstream out;
						out.open("config.txt");
						out << option.WindowLength << " " << option.WindowWidth << " " << option.maxLength << " " << option.scale;
					}
					info();
				}  // Клик был по "зеленой" кнопке

				else if (cur_pos.x >= 150 && cur_pos.x <= 250 && cur_pos.y <= 50) {
					it.first -= ar[ar.get_size() - 1].first, it.second -= ar[ar.get_size() - 1].second;
					if (ar.get_size() > 0)ar.pop_back();
				} // Клик был по "красной" кнопке

				else if (cur_pos.x >= 300 && cur_pos.x <= 400 && cur_pos.y <= 50) {
					cout << "If you want to load file enter 1, else if you want to load options enter 0\n";
					bool flag;
					cin >> flag;
					if (flag) {
						cout << "Enter name of file which you want to load\n";
						string name;
						cin >> name;
						ifstream in(name);
						it = start;
						if (in.is_open()) {
							ar.clear();
							while (!in.eof()) {
								int x, y;
								in >> x >> y;
								it.first += x, it.second += y;
								ar.push_back(make_pair(x, y));
							}
						}
						else cout << "trouble with openning " << name << '\n';
					}
					else {
						ifstream in("config.txt");
						if (in.is_open()) {
							for (int i = 1; i < ar.get_size(); i++) {
								ar[i].first /= option.scale;
								ar[i].second /= option.scale;
							}
							in >> option.WindowLength >> option.WindowWidth >> option.maxLength >> option.scale;
							int dx = start.first + ar[0].first, dy = start.second + ar[0].second;
							double t = dx / dy;
							start.first -= option.displacement;
							start.second += start.first*t;
							for (int i = 1; i < ar.get_size(); i++) {
								ar[i].first *= option.scale;
								ar[i].second *= option.scale;
							}
							option.maxLength *= option.scale;
						}
						else cout << "trouble with openning\n";
					}
					info();
				} // Клик был по "синей" кнопке

				else if (cur_pos.x >= 450 && cur_pos.x <= 550 && cur_pos.y <= 50) ar.clear(); // Клик был по "желтой" кнопке

				else if (ar.get_size() && pow(cur_pos.x - it.first,2) + pow(cur_pos.y - it.second,2) > pow(option.maxLength,2)) cout << "Too large distance for stitch\n"; // Проверка на длину стежка
				
				else {
					if (ar.get_size()) {
						ar.push_back(make_pair(cur_pos.x - it.first, cur_pos.y - it.second));
						it.first = cur_pos.x, it.second = cur_pos.y;
					}
					else {
						ar.push_back(make_pair(cur_pos.x, cur_pos.y));
						it.first = cur_pos.x, it.second = cur_pos.y;
					}

				} // Добавление новой точки в список координат

			}
			if (event.type == sf::Event::KeyPressed && ar.get_size()) {
				if (event.key.code == sf::Keyboard::Equal) {
					option.scale *= 2;
					it = ar[0];
					option.maxLength *= 2;
					for (int i = 1; i < ar.get_size(); i++) {
						ar[i].first *= 2;
						ar[i].second *= 2;
						it.first += ar[i].first, it.second += ar[i].second;
					}
				}
				else if (event.key.code == sf::Keyboard::Dash) {
					option.scale /= 2;
					it = ar[0];
					option.maxLength /= 2;
					for (int i = 1; i < ar.get_size(); i++) {
						ar[i].first /= 2;
						ar[i].second /= 2;
						it.first += ar[i].first, it.second += ar[i].second;
					}
				}
				else if(event.key.code == sf::Keyboard::Left){
					ar[0].first -= option.step;
					it.first -= option.step;
				}
				else if (event.key.code == sf::Keyboard::Right) {
					ar[0].first += option.step;
					it.first += option.step;
				}
				else if (event.key.code == sf::Keyboard::Up) {
					ar[0].second -= option.step;
					it.second -= option.step;
				}
				else if (event.key.code == sf::Keyboard::Down) {
					ar[0].second += option.step;
					it.second += option.step;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close(); // Закрытие программы после нажатия пользователем кнопки Escape

		sf::VertexArray line(sf::LinesStrip, ar.get_size()+1); // Массив узлов ломанной линии

		pair<int, int> temp;

		sf::Text coord("", font, 20);
		sf::Vector2i cur_pos = sf::Mouse::getPosition(window);

		sf::Text scale("", font, 20);
		scale.setFillColor(sf::Color::White);
		scale.setString("Scale is " + to_string(option.scale));
		scale.setPosition(10, 55);


		if (ar.get_size()) {
			temp = ar[0];
			line[0].position = sf::Vector2f(ar[0].first, ar[0].second);
			line[0].color = option.color;

			for (int i = 1; i < ar.get_size(); i++) {
				temp.first += ar[i].first, temp.second += ar[i].second;
				line[i].position = sf::Vector2f(temp.first, temp.second);
				line[i].color = option.color;
			}
			if (pow(cur_pos.x - it.first, 2) + pow(cur_pos.y - it.second, 2) <= pow(option.maxLength, 2)) {
				line[ar.get_size()].position = sf::Vector2f(cur_pos.x, cur_pos.y);
				line[ar.get_size()].color = option.color;
				coord.setFillColor(sf::Color::White);
			}
			else {
				line[ar.get_size()].position = sf::Vector2f(temp.first, temp.second);
				line[ar.get_size()].color = option.color;
				coord.setFillColor(sf::Color::Red);
			}
			coord.setString(to_string(cur_pos.x - it.first) + ":" + to_string(cur_pos.y - it.second));
			coord.setPosition(10, 80);
		}

		window.clear();
		window.draw(coord);
		window.draw(scale);
		window.draw(restart);
		window.draw(load);
		window.draw(cancel);
		window.draw(save);
		window.draw(line);
		window.display();
	}
	for (int i = 0; i < ar.get_size(); i++)cout << ar[i].first << " " << ar[i].second << '\n';
}