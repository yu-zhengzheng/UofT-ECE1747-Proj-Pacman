#include <array>
#include <string>
#include <SFML/Graphics.hpp>
#include <cctype>

#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/ConvertSketch.hpp"

std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& i_map_sketch, std::array<Position, ghostnum>& i_ghost_positions, std::array<Pacman, pacnum>& i_pacman)
{
	//Is it okay if I put {} here? I feel like I'm doing something illegal.
	//But if I don't put it there, Visual Studio keeps saying "lOcAl vArIaBlE Is nOt iNiTiAlIzEd".
	std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> output_map{};
	int pacid = 0;
	for (unsigned char a = 0; a < MAP_HEIGHT; a++)
	{

		int commanum = 0;
		int doublenum = 0;
		bool indoublenum = false;
		for (unsigned char b = 0; b < MAP_WIDTH; b++)
		{
			if (indoublenum)
			{
				indoublenum = false;
				continue;
			}
			//By default, every cell is empty.
			output_map[b][a] = Cell::Empty;

			if (isdigit(i_map_sketch[a][b]))
			{
				int ghostid = 0;
				std::string realnum(1, i_map_sketch[a][b]);
				if (isdigit(i_map_sketch[a][b + 1]))
				{
					realnum.append(1, i_map_sketch[a][b + 1]);
					ghostid = std::stoi(realnum);
					indoublenum = true;
					doublenum++;

				}

				else
				{
					ghostid = i_map_sketch[a][b] - '0';
				}
				if (ghostid < 10)
				{
					i_ghost_positions[ghostid].x = CELL_SIZE * (b - commanum - doublenum);
				}
				else
				{
					i_ghost_positions[ghostid].x = CELL_SIZE * (b - commanum - doublenum + 1);
				}

				i_ghost_positions[ghostid].y = CELL_SIZE * (a);
			}
			else
			{
				char symbol = i_map_sketch[a][b];
				switch (symbol)
				{
					//#wall #obstacle #youcantgothroughme
				case '#':
				{
					output_map[b - commanum - doublenum][a] = Cell::Wall;

					break;
				}
				/*case '=':
				{
					output_map[b - commanum - doublenum][a] = Cell::Door;

					break;
				}*/
				case '.':
				{
					output_map[b - commanum - doublenum][a] = Cell::Pellet;

					break;
				}
				//Red ghost
				//case '0'://not the num on the map
				//{
				//	i_ghost_positions[0].x = CELL_SIZE * b;
				//	i_ghost_positions[0].y = CELL_SIZE * a;

				//	break;
				//}
				////Pink ghost
				//case '1':
				//{
				//	//if there's no comma
				//	i_ghost_positions[1].x = CELL_SIZE * b;
				//	i_ghost_positions[1].y = CELL_SIZE * a;

				//	break;
				//}
				////Blue (cyan) ghost
				//case '2':
				//{
				//	i_ghost_positions[2].x = CELL_SIZE * b;
				//	i_ghost_positions[2].y = CELL_SIZE * a;

				//	break;
				//}
				////Orange ghost
				//case '3':
				//{
				//	i_ghost_positions[3].x = CELL_SIZE * b;
				//	i_ghost_positions[3].y = CELL_SIZE * a;

				//	break;
				//}
				//case '4':
				//{
				//	i_ghost_positions[4].x = CELL_SIZE * b;
				//	i_ghost_positions[4].y = CELL_SIZE * a;

				//	break;
				//}
				//case '5':
				//{
				//	i_ghost_positions[5].x = CELL_SIZE * b;
				//	i_ghost_positions[5].y = CELL_SIZE * a;

				//	break;
				//}
				//case '6':
				//{
				//	i_ghost_positions[6].x = CELL_SIZE * b;
				//	i_ghost_positions[6].y = CELL_SIZE * a;

				//	break;
				//}
				//case '7':
				//{
				//	i_ghost_positions[7].x = CELL_SIZE * b;
				//	i_ghost_positions[7].y = CELL_SIZE * a;

				//	break;
				//}
				case ',':
				{
					commanum++;
					break;
				}

				//Pacman!
				case 'P':
				{
					/*for (; pacid < pacnum; pacid++) {*/
					if (pacid < pacnum)
					{
						printf("pacman %d: b:%d a:%d\n", pacid, b, a);
						i_pacman[pacid].set_position(CELL_SIZE * b, CELL_SIZE * a);
						pacid++;
					}

					/*}*/
					break;
				}
				//This looks like a surprised face.
				case 'o':
				{
					output_map[b][a] = Cell::Energizer;
				}
				}


			}
		}
	}

	return output_map;
}