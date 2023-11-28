#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <thread>
#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/Ghost.hpp"
#include "Headers/GhostManager.hpp"

GhostManager::GhostManager() :
	current_wave(0),
	wave_timer(LONG_SCATTER_DURATION),
	ghosts({Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),Ghost(0), Ghost(1),Ghost(2)})
	/*ghosts({ Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),
	Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3) })*/
{

}

void GhostManager::draw(bool i_flash, sf::RenderWindow& i_window)
{
	for (Ghost& ghost : ghosts)
	{
		ghost.draw(i_flash, i_window);
	}
}

void GhostManager::threadSetPosition(int i, const std::array<Position, ghostnum>& i_ghost_positions)
{
	printf("inside threadSetPosition! by thread %d\n", i);
	ghosts[i].set_position(i_ghost_positions[i].x, i_ghost_positions[i].y);
}

void GhostManager::threadReset(int i, const std::array<Position, ghostnum>& i_ghost_positions)
{
	printf("inside threadReset! by thread %d\n", i);
	//We use the blue ghost to get the location of the house and the red ghost to get the location of the exit.
	ghosts[i].reset(ghosts[2].get_position(), ghosts[0].get_position());
}

void GhostManager::threadSwitchMode(int i)
{
	printf("inside threadSwitchMode! by thread %d\n", i);
	ghosts[i].switch_mode();
}

void GhostManager::threadUpdate(int i, unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman& i_pacman)
{
	printf("inside threadSwitchMode! by thread %d\n", i);
	ghosts[i].update(i_level, i_map, ghosts[0], i_pacman);
}

void GhostManager::reset(unsigned char i_level, const std::array<Position, ghostnum>& i_ghost_positions)
{
	current_wave = 0;

	//This is how we're increasing the difficulty.
	wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, i_level));

	for (unsigned char a = 0; a < ghostnum; a++)
	{
		ghosts[a].set_position(i_ghost_positions[a].x, i_ghost_positions[a].y);
	}


	std::thread myThreads[ghostnum];
	for (int i = 0; i < ghostnum; i++) {
		//threadSetPosition(i, i_ghost_positions);
		myThreads[i] = std::thread(&GhostManager::threadSetPosition, this, i, i_ghost_positions);
	}
	for (int i = 0; i < ghostnum; i++) {
		myThreads[i].join();
	}

	for (int i = 0; i < ghostnum; i++) {
		//threadSetPosition(i, i_ghost_positions);
		myThreads[i] = std::thread(&GhostManager::threadReset, this, i, i_ghost_positions);
	}
	for (int i = 0; i < ghostnum; i++) {
		myThreads[i].join();
	}

	for (Ghost& ghost : ghosts)
	{
		//We use the blue ghost to get the location of the house and the red ghost to get the location of the exit.
		/*ghost.reset(ghosts[2].get_position(), ghosts[0].get_position());*/
	}
}

void GhostManager::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, Pacman& i_pacman)
{
	std::thread myThreads[ghostnum];
	if (0 == i_pacman.get_energizer_timer()) //We won't update the wave timer when Pacman is energized.
	{
		if (0 == wave_timer)
		{
			if (7 > current_wave)
			{
				current_wave++;

				for (int i = 0; i < ghostnum; i++) {
					myThreads[i] = std::thread(&GhostManager::threadSwitchMode, this, i);
				}
				for (int i = 0; i < ghostnum; i++) {
					myThreads[i].join();
				}
				/*for (Ghost& ghost : ghosts)
				{
					ghost.switch_mode();
				}*/
			}

			//I took the rules from the website.
			if (1 == current_wave % 2)
			{
				wave_timer = CHASE_DURATION;
			}
			else if (2 == current_wave)
			{
				wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, i_level));
			}
			else
			{
				wave_timer = static_cast<unsigned short>(SHORT_SCATTER_DURATION / pow(2, i_level));
			}
		}
		else
		{
			wave_timer--;
		}
	}
	for (int i = 0; i < ghostnum; i++) {
		myThreads[i] = std::thread(&GhostManager::threadUpdate, this, i, i_level, std::ref(i_map), std::ref(i_pacman));
	}
	for (int i = 0; i < ghostnum; i++) {
		myThreads[i].join();
	}
	/*
	for (Ghost& ghost : ghosts)
	{
		ghost.update(i_level, i_map, ghosts[0], i_pacman);
	}*/
}
