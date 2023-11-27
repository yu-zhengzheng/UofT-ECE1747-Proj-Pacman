#include <array>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Headers/Global.hpp"
#include "Headers/DrawText.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/Ghost.hpp"
#include "Headers/GhostManager.hpp"
#include "Headers/ConvertSketch.hpp"
#include "Headers/DrawMap.hpp"
#include "Headers/MapCollision.hpp"

int main()
{
	//Is the game won?
	bool game_won = 0;

	//Used to make the game framerate-independent.
	unsigned lag = 0;

	unsigned char level = 0;

	//Similar to lag, used to make the game framerate-independent.
	std::chrono::time_point<std::chrono::steady_clock> previous_time;

	//It's not exactly like the map from the original Pac-Man game, but it's close enough.
	std::array<std::string, MAP_HEIGHT> map_sketch = {
"     ###################  ################### ###################  ################### ,,,,",
"     #........#........#  #........#........# #........#........#  #........#........# ,,,,",
"     #o##.###.#.###.##o#  #o##.###.#.###.##o# #o##.###.#.###.##o#  #o##.###.#.###.##o# ,,,,",
"     #.................#  #.................# #.................#  #.................# ,,,,",
"     #.##.#.#####.#.##.#  #.##.#.#####.#.##.# #.##.#.#####.#.##.#  #.##.#.#####.#.##.# ,,,,",
"     #....#...#...#....#  #....#...#...#....# #....#...#...#....#  #....#...#...#....# ,,,,",
"     ####.### # ###.####  ####.### # ###.#### ####.### # ###.####  ####.### # ###.#### ,,,,",
"        #.#   0   #.#        #.#   0   #.#       #.#   4   #.#        #.#   4   #.#    ,,,,",
"#########.# ##=## #.##########.# ##=## #.#########.# ##=## #.##########.# ##=## #.#########",
"         .  #5,6,7#  .          .  #1,2,3#  .         .  #1,2,3#  .          .  #5,6,7#  .     ",
"#########.# ##### #.##########.# ##### #.#########.# ##### #.##########.# ##### #.#########",
"        #.#       #.#        #.#       #.#       #.#       #.#        #.#       #.#    ,,,,",
"     ####.# ##### #.####  ####.# ##### #.#### ####.# ##### #.####  ####.# ##### #.#### ,,,,",
"     #........#........#  #........#........# #........#........#  #........#........# ,,,,",
"     #.##.###.#.###.##.#  #.##.###.#.###.##.# #.##.###.#.###.##.#  #.##.###.#.###.##.# ,,,,",
"     #o.#.....P.....#.o#  #o.#.....P.....#.o# #o.#.....P.....#.o#  #o.#.....P.....#.o# ,,,,",
"     ##.#.#.#####.#.#.##  ##.#.#.#####.#.#.## ##.#.#.#####.#.#.##  ##.#.#.#####.#.#.## ,,,,",
"     #....#...#...#....#  #....#...#...#....# #....#...#...#....#  #....#...#...#....# ,,,,",
"     #.######.#.######.####.######.#.######.###.######.#.######.####.######.#.######.# ,,,,",
"     #......#...#......    ......#...#......   ......#...#......    ......#...#......# ,,,,",
"     #.####...#...####.####.####...#...####.###.####...#...####.####.####...#...####.# ,,,,",
"     #....#########....#  #....#########....# #....#########....#  #....#########....# ,,,,",
"     #.#......#......#.#  #.#......#......#.# #.#......#......#.#  #.#......#......#.# ,,,,",
"     #o##.###.#.###.##o#  #o##.###.#.###.##o# #o##.###.#.###.##o#  #o##.###.#.###.##o# ,,,,",
"     #.................#  #.................# #.................#  #.................# ,,,,",
"     #.##.#.#####.#.##.#  #.##.#.#####.#.##.# #.##.#.#####.#.##.#  #.##.#.#####.#.##.# ,,,,",
"     #....#...#...#....#  #....#...#...#....# #....#...#...#....#  #....#...#...#....# ,,,,",
"     ####.### # ###.####  ####.### # ###.#### ####.### # ###.####  ####.### # ###.#### ,,,,",
"        #.#   4   #.#        #.#   8   #.#       #.#   4   #.#        #.#   0   #.#    ,,,,",
"#########.# ##=## #.##########.# ##=## #.#########.# ##=## #.##########.# ##=## #.#########",
"         .  #5,6,7#  .          .  #1,2,3#  .         .  #5,6,7#  .          .  #1,2,3#  .     ",
"#########.# ##### #.##########.# ##### #.#########.# ##### #.##########.# ##### #.#########",
"        #.#       #.#        #.#       #.#       #.#       #.#        #.#       #.#    ,,,,",
"     ####.# ##### #.####  ####.# ##### #.#### ####.# ##### #.####  ####.# ##### #.#### ,,,,",
"     #........#........#  #........#........# #........#........#  #........#........# ,,,,",
"     #.##.###.#.###.##.#  #.##.###.#.###.##.# #.##.###.#.###.##.#  #.##.###.#.###.##.# ,,,,",
"     #o.#.....P.....#.o#  #o.#.....P.....#.o# #o.#.....P.....#.o#  #o.#.....P.....#.o# ,,,,",
"     ##.#.#.#####.#.#.##  ##.#.#.#####.#.#.## ##.#.#.#####.#.#.##  ##.#.#.#####.#.#.## ,,,,",
"     #....#...#...#....#  #....#...#...#....# #....#...#...#....#  #....#...#...#....# ,,,,",
"     #.######.#.######.#  #.######.#.######.# #.######.#.######.#  #.######.#.######.# ,,,,",
"     #.................#  #.................# #.................#  #.................# ,,,,",
"     ###################  ################### ###################  ################### ,,,,"

	};

	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map{};

	//Initial ghost positions.
	std::array<Position, ghostnum> ghost_positions;

	//SFML thing. Stores events, I think.
	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close | sf::Style::Resize);
	//Resizing the window.
	window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

	GhostManager ghost_manager;

	//Pacman pacman;
	std::array<Pacman, pacnum> pacman;

	//Generating a random seed.
	srand(static_cast<unsigned>(time(0)));

	map = convert_sketch(map_sketch, ghost_positions, pacman);

	ghost_manager.reset(level, ghost_positions);

	//Get the current time and store it in a variable.
	previous_time = std::chrono::steady_clock::now();

	while (1 == window.isOpen())
	{
		//Here we're calculating the lag.
		unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
		//std::cout<<delta_time<<"\n";
		lag += delta_time;

		previous_time += std::chrono::microseconds(delta_time);

		//While the lag exceeds the maximum allowed frame duration.
		while (FRAME_DURATION <= lag)
		{
			//We decrease the lag.
			lag -= FRAME_DURATION;

			while (1 == window.pollEvent(event))
			{
				switch (event.type)
				{
					case sf::Event::Closed:
					{
						//Making sure the player can close the window.
						window.close();
					}
				}
			}
			//if game is not won and there are pacmen still alive
			bool allPacmanDead = true;
			int alivepacs = 0;
			int last = 0;
			for (int i = 0; i < pacnum; i++) {
				if (pacman[i].get_dead() == 0) {
					allPacmanDead = false;
					alivepacs++;
					last = i;
					break;
				}
			}

			if (0 == game_won && !allPacmanDead)
			{
				game_won = 1;

				for (int i = 0; i < pacnum; i++) {
					if (pacman[i].get_dead() == 0)
					{
						pacman[i].update(level, map);
						//printf("pac %d updated!\n",i);
					}
					
				}
				for (int i = 0; i < pacnum; i++) {
					if (!allPacmanDead) {
						ghost_manager.update(level, map, pacman[i]);
					}
				}


				//We're checking every cell in the map.
				for (const std::array<Cell, MAP_HEIGHT>& column : map)
				{
					for (const Cell& cell : column)
					{
						if (Cell::Pellet == cell) //And if at least one of them has a pellet.
						{
							game_won = 0; //The game is not yet won.

							break;
						}
					}

					if (0 == game_won)
					{
						break;
					}
				}

				if (1 == game_won)
				{
					//for (int i = 0; i < pacnum; i++) {
						
						pacman[last].set_animation_timer(0);
						
					//}
					
				}
			}
			//if game is won or lost, and enter is presses
			else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //Restarting the game.
			{
				game_won = 0;

				if (1 == allPacmanDead)
				{
					level = 0;
				}
				else
				{
					//After each win we reduce the duration of attack waves and energizers.
					level++;
				}

				map = convert_sketch(map_sketch, ghost_positions, pacman);

				ghost_manager.reset(level, ghost_positions);
				for (int i = 0; i < pacnum; i++) {
					pacman[i].reset();
				}
			}

			//I don't think anything needs to be explained here.
			if (FRAME_DURATION > lag)
			{
				window.clear();

				if (0 == game_won && !allPacmanDead)
				{
					draw_map(map, window);
					int maxenergizer = 0;
					for (int i = 0; i < pacnum; i++) {
						if(pacman[i].get_energizer_timer()>maxenergizer)
						{
							maxenergizer = pacman[i].get_energizer_timer();
						};

					}
					ghost_manager.draw(GHOST_FLASH_START >= maxenergizer, window);

					draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);
				}

				for (int i = 0; i < pacnum; i++) {
					if (pacman[i].get_dead() == 0)
					{
						pacman[i].draw(game_won, window);
					}
					
				}
				bool allanimover = true;
				for (int i = 0; i < pacnum; i++) {
					if (!pacman[i].get_animation_over())
					{
						allanimover = false;
					}
				}
				if ((!allanimover && allPacmanDead) || game_won)
				{
					if (1 == game_won)
					{
						draw_text(1, 0, 0, "Next level!", window);
					}
					else
					{
						draw_text(1, 0, 0, "Game over", window);
					}
				}

				window.display();
			}
		}
	}
}