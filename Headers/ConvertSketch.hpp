#pragma once
#include "Global.hpp"
std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& i_map_sketch, std::array<Position, ghostnum>& i_ghost_positions, Pacman& i_pacman);