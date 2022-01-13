#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
const char figures[7] = { 'I', 'J', 'L', 'O', 'S', 'T', 'Z' };
const int bg_texture_pos_x = 0, bg_texture_pos_y = 0, bg_texture_width = 640, bg_texture_height = 640;
const float bg_texture_x_scale = 2.f, bg_texture_y_scale = 2.f;
const int field_texture_width = 250, field_texture_height = 500;
const int field_texture_pos_x = static_cast<int>(175 * bg_texture_x_scale),
field_texture_pos_y = static_cast<int>(52 * bg_texture_y_scale);
const int text_char_size = 24 * bg_texture_x_scale;
int high_score = 0, score = 0;
const int line_score = 100;
const int high_score_texture_width = 195, high_score_texture_height = 58;
const int high_score_texture_pos_x = static_cast<int>(437 * bg_texture_x_scale),
high_score_texture_pos_y = static_cast<int>(119 * bg_texture_y_scale);
const int score_texture_width = 195, score_texture_height = 58;
const int score_texture_pos_x = static_cast<int>(437 * bg_texture_x_scale),
score_texture_pos_y = static_cast<int>(218 * bg_texture_y_scale);
int total_lines_cleared = 0;
int lines_cleared = 0;
int level = 0;
const int total_num_of_levels = 30;
int level_threshold = 1;
const int level_texture_width = 122, level_texture_height = 88;
const int level_texture_pos_x = static_cast<int>(472 * bg_texture_x_scale),
level_texture_pos_y = static_cast<int>(384 * bg_texture_y_scale);
const int next_fig_texture_width = 100, next_fig_texture_height = 100;
const int next_fig_texture_pos_x = static_cast<int>(36 * bg_texture_x_scale),
next_fig_texture_pos_y = static_cast<int>(123 * bg_texture_y_scale);
const int tiles_texture_pos_x = 0, tiles_texture_pos_y = 640, tiles_texture_width = 175, tiles_texture_height = 25;
const int tiles_texture_part_width = 25, tiles_texture_part_height = 25;
const float tiles_texture_part_x_scale = 2.f, tiles_texture_part_y_scale = 2.f;
const int tiles_sprite_width = static_cast<int>(tiles_texture_part_width * tiles_texture_part_x_scale),
tiles_sprite_height = static_cast<int>(tiles_texture_part_height * tiles_texture_part_y_scale);
const int field_width = 10, field_height = 20;
const int left_bound = 0, right_bound = 9, upper_bound = 0, down_bound = 19;
int delay = 1000;
const int delay_boost = 30;
float sound_pitch = 1.0f;
const float sound_pitch_boost = 0.05f;
class Point {
public:
	sf::Sprite sprite;
	int x, y;
	bool empty;
	Point() {
		x = -1;
		y = -1;
		empty = true;
	};
	Point(const Point& copy_obj) {
		empty = copy_obj.empty;
		sprite = copy_obj.sprite;
		x = copy_obj.x;
		y = copy_obj.y;
	}
	void draw(sf::RenderWindow& window) {
		sf::Sprite temp_sprite = sprite;
		temp_sprite.setPosition(sf::Vector2f(static_cast<float>(+field_texture_pos_x + x * tiles_sprite_width),
			static_cast<float>(field_texture_pos_y + y * tiles_sprite_height))); // absolute position
		window.draw(temp_sprite);
	}
	void draw_next_fig(sf::RenderWindow& window, int alt_x, int alt_y, char fig_type) {
		sf::Sprite temp_sprite = sprite;
		switch (fig_type) {
		case 'I':
			temp_sprite.setPosition(sf::Vector2f(static_cast<float>(next_fig_texture_pos_x + alt_x * tiles_sprite_width),
				static_cast<float>(37.5 * bg_texture_y_scale + next_fig_texture_pos_y + alt_y * tiles_sprite_height))); // absolute position
			break;
		case 'O':
			temp_sprite.setPosition(sf::Vector2f(static_cast<float>(25 * bg_texture_x_scale + next_fig_texture_pos_x + alt_x * tiles_sprite_width),
				static_cast<float>(25 * bg_texture_y_scale + next_fig_texture_pos_y + alt_y * tiles_sprite_height))); // absolute position
			break;
		default:
			temp_sprite.setPosition(sf::Vector2f(static_cast<float>(12.5 * bg_texture_x_scale + next_fig_texture_pos_x + alt_x * tiles_sprite_width),
				static_cast<float>(25 * bg_texture_y_scale + next_fig_texture_pos_y + alt_y * tiles_sprite_height))); // absolute position
			break;
		}
		window.draw(temp_sprite);
	}
};
Point field[field_height][field_width] = { Point() };
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_int_distribution<> dist(0, tiles_texture_width / tiles_texture_part_width - 1);
std::uniform_int_distribution<> dist_fig(0, 6);
sf::Sprite gen_sprite(const sf::Texture& texture) {
	int sprite_num = dist(mt);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(tiles_texture_pos_x + tiles_texture_part_width * sprite_num, tiles_texture_pos_y,
		tiles_texture_part_width, tiles_texture_part_height));
	sprite.scale(sf::Vector2f(tiles_texture_part_x_scale, tiles_texture_part_y_scale)); // factor relative to the current scale
	return sprite;
}
class Figure {
	Point fig[4];
	char fig_type;
	float fig_origin_x, fig_origin_y;
	char gen_fig() {
		int fig_num = dist_fig(mt);
		return figures[fig_num];
	}
public:
	Figure(const sf::Sprite sprite)
	{
		fig_type = gen_fig();
		switch (fig_type) {
		case 'I':
			fig[0].x = 0, fig[0].y = 0;
			fig[1].x = 1, fig[1].y = 0;
			fig[2].x = 2, fig[2].y = 0;
			fig[3].x = 3, fig[3].y = 0;
			fig_origin_x = 2, fig_origin_y = 0;
			break;
		case 'J':
			fig[0].x = 0, fig[0].y = 0;
			fig[1].x = 0, fig[1].y = 1;
			fig[2].x = 1, fig[2].y = 1;
			fig[3].x = 2, fig[3].y = 1;
			fig_origin_x = 1, fig_origin_y = 1;
			break;
		case 'L':
			fig[0].x = 0, fig[0].y = 1;
			fig[1].x = 1, fig[1].y = 1;
			fig[2].x = 2, fig[2].y = 1;
			fig[3].x = 2, fig[3].y = 0;
			fig_origin_x = 1, fig_origin_y = 1;
			break;
		case 'O':
			fig[0].x = 0, fig[0].y = 0;
			fig[1].x = 0, fig[1].y = 1;
			fig[2].x = 1, fig[2].y = 1;
			fig[3].x = 1, fig[3].y = 0;
			fig_origin_x = -1, fig_origin_y = -1;
			break;
		case 'S':
			fig[0].x = 0, fig[0].y = 1;
			fig[1].x = 1, fig[1].y = 1;
			fig[2].x = 1, fig[2].y = 0;
			fig[3].x = 2, fig[3].y = 0;
			fig_origin_x = 1, fig_origin_y = 1;
			break;
		case 'T':
			fig[0].x = 0, fig[0].y = 1;
			fig[1].x = 1, fig[1].y = 1;
			fig[2].x = 1, fig[2].y = 0;
			fig[3].x = 2, fig[3].y = 1;
			fig_origin_x = 1, fig_origin_y = 1;
			break;
		case 'Z':
			fig[0].x = 0, fig[0].y = 0;
			fig[1].x = 1, fig[1].y = 0;
			fig[2].x = 1, fig[2].y = 1;
			fig[3].x = 2, fig[3].y = 1;
			fig_origin_x = 1, fig_origin_y = 1;
			break;
		default:
			break;
		}
		for (int i = 0; i < 4; ++i) {
			if (fig_type == 'O') {
				fig[i].x += 4; // center fig
			}
			else {
				fig[i].x += 3; // center fig
			}
			fig[i].empty = false;
			fig[i].sprite = sprite;
		}
		if (fig_type == 'O') {
			fig_origin_x += 4; // center fig
		}
		else {
			fig_origin_x += 3; // center fig
		}
	}
	void draw(sf::RenderWindow& window) {
		for (int i = 0; i < 4; ++i) {
			fig[i].draw(window);
		}
	}
	void draw_next_fig(sf::RenderWindow& window) {
		int min_x = fig[0].x, min_y = fig[0].y;
		for (int i = 0; i < 4; ++i) {
			if (fig[i].x < min_x) {
				min_x = fig[i].x;
			}
			if (fig[i].y < min_y) {
				min_y = fig[i].y;
			}
		}
		for (int i = 0; i < 4; ++i) {
			int alt_x = fig[i].x - min_x;
			int alt_y = fig[i].y - min_y;
			fig[i].draw_next_fig(window, alt_x, alt_y, fig_type);
		}
	}
	void draw_shadow(sf::RenderWindow& window) {
		Figure temp_fig = *this;
		temp_fig.move(3);
		for (int i = 0; i < 4; ++i) {
			sf::Color temp = temp_fig.fig[i].sprite.getColor();
			temp_fig.fig[i].sprite.setColor(sf::Color(temp.r, temp.g, temp.b, 128));
		}
		temp_fig.draw(window);
	}
	int move(int dir) {
		bool move_pass = true;
		int min_dist = down_bound;
		int max_y = fig[0].y;
		switch (dir) {
		case 0: // left
			for (int i = 0; i < 4; ++i) {
				if (fig[i].x > left_bound) {
					if (!field[fig[i].y][(fig[i].x - 1)].empty) {
						move_pass = false;
					}
				}
				else {
					move_pass = false;
				}
			}
			if (move_pass) {
				for (int i = 0; i < 4; ++i) {
					fig[i].x -= 1;
				}
				fig_origin_x -= 1;
			}
			break;
		case 1: // right
			for (int i = 0; i < 4; ++i) {
				if (fig[i].x < right_bound) {
					if (!field[fig[i].y][(fig[i].x + 1)].empty) {
						move_pass = false;
					}
				}
				else {
					move_pass = false;
				}
			}
			if (move_pass) {
				for (int i = 0; i < 4; ++i) {
					fig[i].x += 1;
				}
				fig_origin_x += 1;
			}
			break;
		case 2: // down
			for (int i = 0; i < 4; ++i) {
				if (fig[i].y < down_bound) {
					if (!field[(fig[i].y + 1)][fig[i].x].empty) {
						move_pass = false;
					}
				}
				else {
					move_pass = false;
				}
			}
			if (move_pass) {
				for (int i = 0; i < 4; ++i) {
					fig[i].y += 1;
				}
				fig_origin_y += 1;
			}
			else {
				for (int i = 0; i < 4; ++i) {
					if (field[fig[i].y][fig[i].x].empty) {
						return 1; // end_fig
					}
					else {
						return 2; // end_game
					}
				}
			}
			return 0; // normal move
			break;
		case 3: // hard down
			for (int i = 0; i < 4; ++i) {
				if (fig[i].y > max_y) {
					max_y = fig[i].y;
				}
				for (int j = fig[i].y; j < field_height; ++j) {
					if (!field[j][fig[i].x].empty) {
						if (j - 1 - fig[i].y < min_dist) {
							min_dist = j - 1 - fig[i].y;
						}
					}
				}
			}
			for (int i = 0; i < 4; ++i) {
				if (min_dist == down_bound) {
					fig[i].y += min_dist - max_y;
				}
				else {
					fig[i].y += min_dist;
				}
			}
			return 1; // end_fig
			break;
		default:
			break;
		}
		return 0;
	}
	void rotate(int rotate_type) {
		if (fig_type != 'O') {
			Point temp_fig[4];
			int min_x = left_bound, min_y = upper_bound, max_x = right_bound, max_y = down_bound;
			for (int i = 0; i < 4; ++i) {
				temp_fig[i] = fig[i];
				float temp_x = temp_fig[i].x - fig_origin_x;
				float temp_y = temp_fig[i].y - fig_origin_y;
				temp_y *= -1; // mirror OY
				temp_fig[i].x = static_cast<int>(temp_y);
				temp_fig[i].y = static_cast<int>(temp_x);
				if (rotate_type == 0) {
					temp_fig[i].y *= -1; // clockwise
				}
				else if (rotate_type == 1) {
					temp_fig[i].x *= -1; // anticlockwise
				}
				temp_fig[i].y *= -1; // mirror OY
				temp_fig[i].x += static_cast<int>(fig_origin_x);
				temp_fig[i].y += static_cast<int>(fig_origin_y);
				if (temp_fig[i].x < min_x) {
					min_x = temp_fig[i].x;
				}
				if (temp_fig[i].y < min_y) {
					min_y = temp_fig[i].y;
				}
				if (temp_fig[i].x > max_x) {
					max_x = temp_fig[i].x;
				}
				if (temp_fig[i].y > max_y) {
					max_y = temp_fig[i].y;
				}
			}
			bool rotate_pass = true;
			for (int i = 0; i < 4; ++i) {
				if (min_x < left_bound) {
					temp_fig[i].x += (left_bound - min_x);
				}
				if (min_y < upper_bound) {
					temp_fig[i].y += (upper_bound - min_y);
				}
				if (max_x > right_bound) {
					temp_fig[i].x += (right_bound - max_x);
				}
				if (max_y > down_bound) {
					temp_fig[i].y += (down_bound - max_y);
				}
				if (!field[temp_fig[i].y][temp_fig[i].x].empty) {
					rotate_pass = false;
				}
			}
			if (rotate_pass) {
				for (int i = 0; i < 4; ++i) {
					fig[i] = temp_fig[i];
				}
			}
		}
	}
	void end_fig() {
		for (int i = 0; i < 4; ++i) {
			field[fig[i].y][fig[i].x] = fig[i];
		}
	}
};
void down_move_operation(int move_type, Figure& current_fig, Figure& next_fig, sf::Texture& texture, sf::Music& music, sf::Sound& sound, std::fstream& output) {
	int move_code = current_fig.move(move_type);
	if (move_code == 1) {
		current_fig.end_fig();
		int line_erased = 0;
		int line_erased_index = -1;
		for (int i = field_height - 1; i > -1; --i) {
			int count = 0;
			for (int j = field_width - 1; j > -1; --j) {
				if (field[i][j].empty) {
					break;
				}
				else {
					++count;
				}
			}
			if (count == field_width) {
				for (int j = field_width - 1; j > -1; --j) {
					field[i][j] = Point();
				}
				++line_erased;
				if (line_erased_index == -1) {
					line_erased_index = i;
				}
			}
		}
		if (line_erased > 0) {
			for (int i = field_height - 1; i > -1; --i) {
				for (int j = field_width - 1; j > -1; --j) {
					if (i < line_erased_index && !field[i][j].empty) {
						field[i + line_erased][j] = field[i][j];
						field[i + line_erased][j].y = i + line_erased;
						field[i][j] = Point();
					}
				}
			}
			sound.play();
			score += (line_erased * line_score * (level + 1));
			total_lines_cleared += line_erased;
			lines_cleared += line_erased;
			if (level < total_num_of_levels && lines_cleared > level_threshold) {
				level += 1;
				level_threshold += 1;
				lines_cleared = 0;
				delay -= delay_boost;
				sound_pitch += sound_pitch_boost;
				music.setPitch(sound_pitch);
				sound.setPitch(sound_pitch);
			}
		}
	}if (move_code == 2) {
		for (int i = field_height - 1; i > -1; --i) {
			for (int j = field_width - 1; j > -1; --j) {
				field[i][j] = Point();
			}
		}
		if (score > high_score) {
			high_score = score;
			output.open("high_score.txt");
			output << std::to_string(score);
			output.close();
		}
	}
	if (move_code != 0) {
		current_fig = next_fig;
		next_fig = Figure(gen_sprite(texture));
	}
}
void draw_text(sf::RenderWindow& window, sf::Text& text) {
	text.setString(std::to_string(score));
	text.setPosition(sf::Vector2f(static_cast<float>((score_texture_width * bg_texture_x_scale - text.getGlobalBounds().width) / 2.0f + score_texture_pos_x),
		static_cast<float>((score_texture_height * bg_texture_y_scale - text_char_size) / 2.0f + score_texture_pos_y))); // absolute position
	window.draw(text);
	text.setString(std::to_string(high_score));
	text.setPosition(sf::Vector2f(static_cast<float>((high_score_texture_width * bg_texture_x_scale - text.getGlobalBounds().width) / 2.0f + high_score_texture_pos_x),
		static_cast<float>((high_score_texture_height * bg_texture_y_scale - text_char_size) / 2.0f + high_score_texture_pos_y))); // absolute position
	window.draw(text);
	text.setString(std::to_string(level));
	text.setPosition(sf::Vector2f(static_cast<float>((level_texture_width * bg_texture_x_scale - text.getGlobalBounds().width) / 2.0f + level_texture_pos_x),
		static_cast<float>((level_texture_height * bg_texture_y_scale - text_char_size) / 2.0f + level_texture_pos_y))); // absolute position
	window.draw(text);
}
int main() {
	try {
		/* auto monitor
		HWND hd = GetDesktopWindow();
		HMONITOR monitor = MonitorFromWindow(hd, MONITOR_DEFAULTTONEAREST);
		MONITORINFO info;
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
		int monitor_width = info.rcMonitor.right - info.rcMonitor.left;
		int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;
		float scale = 1.f;
		monitor_width *= static_cast<int>(scale);
		monitor_height *= static_cast<int>(scale);
		*/
		const int monitor_width = bg_texture_width * bg_texture_x_scale, monitor_height = bg_texture_height * bg_texture_y_scale;
		sf::RenderWindow window(sf::VideoMode(monitor_width, monitor_height), "Tetris", sf::Style::Titlebar | sf::Style::Close);
		std::fstream input("high_score.txt", std::istream::in);
		std::fstream output;
		if (!input) {
			output.open("high_score.txt", std::ostream::out);
			output << "0";
			output.close();
		}
		input.close();
		input.open("high_score.txt");
		std::string temp_input;
		std::getline(input, temp_input);
		input.close();
		high_score = std::stoi(temp_input);
		sf::Font font;
		if (!font.loadFromFile("Sabo-filled.otf"))
		{
			throw std::exception("Font error");
		}
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(text_char_size); // in pixels
		text.setFillColor(sf::Color::White);
		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile("tetris_line_clear_sound.wav")) {
			throw std::exception("Sound error");
		}
		sf::Sound sound;
		sound.setBuffer(buffer);
		sf::Music music;
		if (!music.openFromFile("tetris_theme.ogg"))
			throw std::exception("Music error");
		music.setVolume(50);
		music.setLoop(true);
		music.play();
		sf::Texture texture;
		if (!texture.loadFromFile("tetris.png"))
		{
			throw std::exception("Texture error");
		}
		sf::Sprite bg_sprite;
		bg_sprite.setTexture(texture);
		bg_sprite.setTextureRect(sf::IntRect(bg_texture_pos_x, bg_texture_pos_y,
			bg_texture_width, bg_texture_height));
		bg_sprite.scale(sf::Vector2f(bg_texture_x_scale, bg_texture_y_scale)); // factor relative to the current scale
		Figure current_fig(gen_sprite(texture));
		Figure current_fig_shadow = current_fig;
		Figure next_fig(gen_sprite(texture));
		std::chrono::time_point<std::chrono::system_clock> elapsed_time = std::chrono::system_clock::now();
		sf::Event event;
		while (window.isOpen()) {
			if (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					if (score > high_score) {
						high_score = score;
						output.open("high_score.txt");
						output << std::to_string(score);
						output.close();
					}
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Left)
					{
						current_fig.move(0);
					}
					else if (event.key.code == sf::Keyboard::Right)
					{
						current_fig.move(1);
					}
					else if (event.key.code == sf::Keyboard::Down)
					{
						down_move_operation(2, current_fig, next_fig, texture, music, sound, output);
					}
					else if (event.key.code == sf::Keyboard::Space)
					{
						down_move_operation(3, current_fig, next_fig, texture, music, sound, output);
					}
					else if (event.key.code == sf::Keyboard::Up)
					{
						current_fig.rotate(0);
					}
					else if (event.key.code == sf::Keyboard::Z)
					{
						current_fig.rotate(1);
					}
					else if (event.key.code == sf::Keyboard::P) {
						while (true) {
							if (window.waitEvent(event)) {
								if (event.type == sf::Event::KeyPressed) {
									if (event.key.code == sf::Keyboard::P) {
										break;
									}
								}
								else if (event.type == sf::Event::Closed) {
									if (score > high_score) {
										high_score = score;
										output.open("high_score.txt");
										output << std::to_string(score);
										output.close();
									}
									window.close();
									break;
								}
							}
						}
					}
					break;
				default:
					break;
				}
			}
			if (std::chrono::system_clock::now() - elapsed_time > std::chrono::milliseconds(delay)) {
				down_move_operation(2, current_fig, next_fig, texture, music, sound, output);
				elapsed_time = std::chrono::system_clock::now();
			}
			current_fig_shadow = current_fig;
			window.clear(sf::Color::Black);
			window.draw(bg_sprite);
			next_fig.draw_next_fig(window);
			draw_text(window, text);
			for (int i = field_height - 1; i > -1; --i) {
				for (int j = field_width - 1; j > -1; --j) {
					sf::RectangleShape rectangle;
					rectangle.setSize(sf::Vector2f(tiles_sprite_width, tiles_sprite_height));
					rectangle.setFillColor(sf::Color::Black);
					rectangle.setOutlineColor(sf::Color::White);
					rectangle.setOutlineThickness(1);
					rectangle.setPosition(field_texture_pos_x + j * tiles_sprite_width, field_texture_pos_y + i * tiles_sprite_height);
					window.draw(rectangle);
					if (!field[i][j].empty) {
						field[i][j].draw(window);
					}
				}
			}
			current_fig.draw(window);
			current_fig_shadow.draw_shadow(window);
			window.display();
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}