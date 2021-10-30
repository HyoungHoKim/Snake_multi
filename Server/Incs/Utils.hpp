#ifndef UTILS_HPP
#define UTILS_HPP

# include "SnakeGameShared.hpp"

void DrawBorderWindow(int max_x, int max_y);
void PrintSpeed(Snake snk);
void HANDLE_EVERYTHING_TILL_EVENT_LOOP();
void SignalHandler(int code);

#endif