#ifndef SNAKEGAMESHARED_HPP
#define SNAKEGAMESHARED_HPP

# include <bits/stdc++.h>
# include <vector>
# include <iostream>
# include <string>
# include <ncurses.h>
# include <fstream>
# include <regex>

# include "Server.hpp"
# include "Game.hpp"
# include "Snake.hpp"

# define RED 7
# define GREEN 2
# define YELLOW 3
# define BLUE 1
# define BLACK 0
# define CYAN 4
# define WHITE 8
# define MAGENTA 5

# define coloron(a) attron(COLOR_PAIR(a))
# define coloroff(a) attroff(COLOR_PAIR(a))

using namespace std;

class snake;
class Game;

int g_max_x = 0, g_max_y = 0;
ofstream logfile;

std::ostringstream ss;

struct food
{
	int x, y;
	char foodChar;
};

Game GameObj(1);


#endif