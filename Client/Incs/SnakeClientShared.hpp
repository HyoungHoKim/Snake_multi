#ifndef SNAKECLIENTSHARED_HPP
#define SNAKECLIENTSHARED_HPP

# include <bits/stdc++.h>
# include <unistd.h>
# include <vector>
# include <iostream>
# include <string>
# include <ncurses.h>
# include <fstream>
# include <regex>

using namespace std;

struct food
{
	int x, y;
	char foodChar;
};

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

# include "Client.hpp"
# include "Game.hpp"
# include "Snake.hpp"
# include "Utils.hpp"

#endif