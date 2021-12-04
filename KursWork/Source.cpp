#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
//commit
using namespace std;

int windowL = 800, windowH = 800;
int field1X = 195, field1Y = 295, field2X = 600, field2Y = 300, field3X = 195, field3Y = 700, field4X = 195, field4Y = 300;
int field1L = 410, field1H = 5, field2L = 5, field2H = 400, field3L = 410, field3H = 5, field4L = 5, field4H = 400;


struct node {
	pair<int, int> pos; // координаты расположения звена змейки
	pair<int, int> dir; // координаты направления звена змейки
};

vector<node> snake; 

pair<int,int> RandDigit() { // функция, генерирующая расположение для еды
	int x = 200 + (rand() % 400);
	int y = 300 + (rand() % 400);
	x -= (x % 10);
	y -= (y % 10);
	return make_pair(x, y);
}

int main() {
	vector<pair<int, int>> food;
	bool game = 1; // 1 - playing, 0 - lose

	int n;
	cout << "Enter the number of food\n";
	cin >> n;

	food.resize(n);
	srand(static_cast<unsigned int>(time(0)));
	for (int i = 0; i < food.size(); i++)food[i] = RandDigit();

	sf::RenderWindow window(sf::VideoMode(windowL, windowH), "Snake");

	sf::RectangleShape field1(sf::Vector2f(field1L, field1H));
	field1.setPosition(sf::Vector2f(field1X, field1Y));
	field1.setFillColor(sf::Color::Green);

	sf::RectangleShape field2(sf::Vector2f(field2L, field2H));
	field2.setPosition(sf::Vector2f(field2X, field2Y));
	field2.setFillColor(sf::Color::Green);

	sf::RectangleShape field3(sf::Vector2f(field3L, field3H));
	field3.setPosition(sf::Vector2f(field3X, field3Y));
	field3.setFillColor(sf::Color::Green);

	sf::RectangleShape field4(sf::Vector2f(field4L, field4H));
	field4.setPosition(sf::Vector2f(field4X, field4Y));
	field4.setFillColor(sf::Color::Green);

	sf::RectangleShape block(sf::Vector2f(10, 10));
	block.setOutlineColor(sf::Color::Blue);
	block.setFillColor(sf::Color::White);
	block.setOutlineThickness(2.f);

	sf::RectangleShape meal(sf::Vector2f(10, 10));
	meal.setFillColor(sf::Color::Yellow);

	node temp;
	temp.pos = make_pair(400, 500);
	temp.dir = make_pair(0, -10);
	snake.push_back(temp);

	sf::Font font;
	font.loadFromFile("arial.ttf");

	sf::Text restart("Restart", font, 20);
	restart.setFillColor(sf::Color::Cyan);
	restart.setPosition(sf::Vector2f(10, 5));

	sf::Text lose("You lose", font, 50);
	lose.setFillColor(sf::Color::Red);
	lose.setPosition(sf::Vector2f(300, 50));

	sf::Text score("", font, 30);
	score.setFillColor(sf::Color::White);
	score.setPosition(sf::Vector2f(300, 150));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up)snake[0].dir=make_pair(0,-10);
				if (event.key.code == sf::Keyboard::Right)snake[0].dir = make_pair(10, 0);
				if (event.key.code == sf::Keyboard::Down)snake[0].dir = make_pair(0, 10);
				if (event.key.code == sf::Keyboard::Left)snake[0].dir = make_pair(-10, 0);
			}
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
				sf::Vector2i cur_pos = sf::Mouse::getPosition(window);
				if (cur_pos.x <= 100 && cur_pos.y <= 50) {
					snake.clear();
					snake.push_back(temp);
					game = 1;
				}
			}
		}

		if (game) {

			for (int i = 0; i < snake.size(); i++) { // Сдвиг змейки
				snake[i].pos.first += snake[i].dir.first;
				snake[i].pos.second += snake[i].dir.second;
			}
			for (int i = snake.size()-1; i > 0; i--) { // Изменение направления в каждом элементе
				snake[i].dir = snake[i - 1].dir;
			}

			if (snake[0].pos.first < 200 || snake[0].pos.first >= 600 || snake[0].pos.second < 300 || snake[0].pos.second >= 700) game = 0;
			for (int i = 1; i < snake.size(); i++) if (snake[0].pos == snake[i].pos) game = 0;
			
			for (int i = 0; i < food.size(); i++) {
				if (snake[0].pos == food[i]) {
					node t;
					t.pos.first = snake.back().pos.first - snake.back().dir.first;
					t.pos.second = snake.back().pos.second - snake.back().dir.second;
					t.dir = snake.back().dir;
					snake.push_back(t);
					srand(static_cast<unsigned int>(time(0)));
					pair<int, int> ch;
					bool flag = 1;
					while (flag) {
						ch = RandDigit();
						flag = 0;
						for (int j = 0; j < food.size(); j++) if (ch == food[j])flag = 1;
					}
					food[i] = ch;
				}
			} // Проверка на то, съела ли змейка еду
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))window.close();
		window.clear();

		if (!game) window.draw(lose);
		score.setString("Your score is " + to_string(snake.size()));
		window.draw(score);

		window.draw(restart);
		window.draw(field1);
		window.draw(field2);
		window.draw(field3);
		window.draw(field4);

		for (int i = 0; i < food.size(); i++) { // Отрисовка еды на карте
			meal.setPosition(sf::Vector2f(food[i].first, food[i].second));
			window.draw(meal);
		}

		for (int i = 0; i < snake.size(); i++) { // Отрисовка змейки на карте
			block.setPosition(sf::Vector2f(snake[i].pos.first, snake[i].pos.second));
			window.draw(block);
		}

		window.display();
		this_thread::sleep_for(100ms); // Задержка
	}
}