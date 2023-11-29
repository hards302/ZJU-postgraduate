#ifndef DRAW_HPP
#define DRAW_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
using std::vector;
using std::string;

bool drawpic(string path, vector<vector<int>> frameBuffer, int screenWidth, int screenHeight);

#endif