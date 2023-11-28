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
	//ghosts({Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),Ghost(0), Ghost(1)})
	ghosts({ Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),
			 Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3),Ghost(0), Ghost(1), Ghost(2), Ghost(3),Ghost(0), Ghost(1),Ghost(2),Ghost(3) })

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
	for (int j = 0; j < 8; j++) {
		ghosts[i * 8 + j].set_position(i_ghost_positions[i * 8 + j].x, i_ghost_positions[i * 8 + j].y);
	}
}

void GhostManager::threadReset(int i, const std::array<Position, ghostnum>& i_ghost_positions)
{
	//We use the blue ghost to get the location of the house and the red ghost to get the location of the exit.
	for (int j = 0; j < 8; j++) {
		ghosts[i * 8 + j].reset(ghosts[i * 8+2+j/4*4].get_position(), ghosts[i * 8+j / 4 * 4].get_position());
	}
}

void GhostManager::threadSwitchMode(int i)
{
	for (int j = 0; j < 8; j++) {
		ghosts[i * 8 + j].switch_mode();
	}
}

void GhostManager::threadUpdate(int i, unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, std::array<Pacman, pacnum>& i_pacman)
{
	for (int j = 0; j < 8; j++) {
		int shortestid = 0;
		int shortestdis = pow(ghosts[i * 8 + j].get_position().x - i_pacman[0].get_position().x, 2) + pow(ghosts[i * 8 + j].get_position().y - i_pacman[0].get_position().y, 2);
		for(int n = 1; n<pacnum; n++)
		{
			int currentdis = pow(ghosts[i * 8 + j].get_position().x - i_pacman[n].get_position().x, 2) + pow(ghosts[i * 8 + j].get_position().y - i_pacman[n].get_position().y,2);
			if(currentdis<shortestdis && !i_pacman[n].get_dead())
			{
				shortestdis = currentdis;
				shortestid = n;
			}
			
		}
		ghosts[i * 8 + j].update(i_level, i_map, ghosts[i * 8], i_pacman[shortestid]);
	}
}

void GhostManager::reset(unsigned char i_level, const std::array<Position, ghostnum>& i_ghost_positions)
{
	current_wave = 0;

	//This is how we're increasing the difficulty.
	wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, i_level));

	for (int i = 0; i < ghostnum / 8; i++) {
		threadArray[i] = std::thread(&GhostManager::threadSetPosition, this, i, i_ghost_positions);
	}
	for (int i = 0; i < ghostnum / 8; i++) {
		threadArray[i].join();
	}


	for (int i = 0; i < ghostnum / 8; i++) {
		threadArray[i] = std::thread(&GhostManager::threadReset, this, i, i_ghost_positions);
	}
	for (int i = 0; i < ghostnum / 8; i++) {
		threadArray[i].join();
	}
}

void GhostManager::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, std::array<Pacman, pacnum>& i_pacman)
{
	unsigned short maxtimer = 0;
	for(Pacman&pacs: i_pacman)
	{
		if(pacs.get_energizer_timer()>maxtimer)
		{
			maxtimer = pacs.get_energizer_timer();
		}
	}
	if (0 == maxtimer) //We won't update the wave timer when Pacman is energized.
	{
		if (0 == wave_timer)
		{
			if (7 > current_wave)
			{
				current_wave++;

				for (int i = 0; i < ghostnum / 8; i++) {
					threadArray[i] = std::thread(&GhostManager::threadSwitchMode, this, i);
				}
				for (int i = 0; i < ghostnum / 8; i++) {
					threadArray[i].join();
				}
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
	for (int i = 0; i < ghostnum / 8; i++) {
		threadArray[i] = std::thread(&GhostManager::threadUpdate, this, i, i_level, std::ref(i_map), std::ref(i_pacman));
	}
	for (int i = 0; i < ghostnum / 8; i++) {
		threadArray[i].join();
	}
}
