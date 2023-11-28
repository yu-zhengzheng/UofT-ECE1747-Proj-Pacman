#pragma once
#include <thread>
class GhostManager
{
	//The ghosts will switch between the scatter mode and the chase mode before permanently chasing Pacman.
	//So we need this to keep track of the waves.
	unsigned char current_wave;

	//Damn, I really used a lot of timers.
	unsigned short wave_timer;

	std::array<Ghost, ghostnum> ghosts;
	std::array<std::thread, ghostnum> threadArray;
public:
	GhostManager();

	void draw(bool i_flash, sf::RenderWindow& i_window);
	void reset(unsigned char i_level, const std::array<Position, ghostnum>& i_ghost_positions);

	void threadSetPosition(int i, const std::array<Position, ghostnum>& i_ghost_positions);
	void threadReset(int i, const std::array<Position, ghostnum>& i_ghost_positions);
	void threadSwitchMode(int i);
	void threadUpdate(int i, unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, std::array<Pacman, pacnum>& i_pacman);

	void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, std::array<Pacman, pacnum>& i_pacman);
};