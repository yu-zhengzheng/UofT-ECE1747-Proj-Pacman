#include <array>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <string>

#include "Headers/Global.hpp"
#include "Headers/Pacman.hpp"
#include "Headers/MapCollision.hpp"
#include "Headers/GhostManager.hpp"
#include <iostream>

Pacman::Pacman() :
	animation_over(0),
	dead(0),
	direction(0),
	energizer_timer(0),
	position({0, 0})
{
	//I just realized that I already explained everything in the Ghost class.
	//And I don't like to repeat myself.
}

/*std::vector<Pacman::Point> Pacman::findPath(const Pacman::Point& start, const Pacman::Point& goal, const std::vector<std::vector<int>>& grid) {
	auto heuristic = [](const Pacman::Point& a, const Pacman::Point& b) {
		return std::abs(a.x - b.x) + std::abs(a.y - b.y);
		};

	auto isValid = [&grid](const Pacman::Point& point) {
		return point.x >= 0 && point.x < grid.size() && point.y >= 0 && point.y < grid[0].size() && grid[point.x][point.y] != 1;
		};
	std::priority_queue<Pacman::Node, std::vector<Pacman::Node>, Pacman::CompareNode> openSet;
	std::unordered_map<Pacman::Point, Pacman::Point, Pacman::PointHash> cameFrom;
	std::unordered_map<Pacman::Point, int, Pacman::PointHash> gScore;

	openSet.push({ start, 0, heuristic(start, goal), Pacman::Point{-1, -1} });
	gScore[start] = 0;

	while (!openSet.empty()) {
		Pacman::Point current = openSet.top().position;
		if (current == goal) {
			std::vector<Pacman::Point> path;
			while (current != Pacman::Point{ -1, -1 }) {
				path.push_back(current);
				current = cameFrom[current];
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		openSet.pop();

		const std::vector<std::pair<int, int>> directions = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
		for (const auto& dir : directions) {
			Pacman::Point neighbor{ current.x + dir.first, current.y + dir.second };
			if (isValid(neighbor)) {
				int tentative_gScore = gScore[current] + 1;
				if (!gScore.count(neighbor) || tentative_gScore < gScore[neighbor]) {
					cameFrom[neighbor] = current;
					gScore[neighbor] = tentative_gScore;
					int h = heuristic(neighbor, goal);
					openSet.push({ neighbor, tentative_gScore, h, current });
				}
			}
		}
	}

	return {};
}

Pacman::Point Pacman::findSafestPoint(const Pacman::Point& pacmanPosition, const std::vector<Position>& ghostPositions, const std::vector<std::vector<int>>& grid) {
	int bestScore = std::numeric_limits<int>::min();
	Pacman::Point targetPosition = pacmanPosition;
	for (int x = 0; x < grid.size(); ++x) {
		for (int y = 0; y < grid[x].size(); ++y) {
			if (grid[x][y] == 0) {
				Pacman::Point point = { y, x };
				int score = 0;
				for (const auto& ghostPos : ghostPositions) {
					score += std::abs(point.y - ghostPos.y) + std::abs(point.x - ghostPos.x);
				}
				if (score > bestScore) {
					bestScore = score;
					targetPosition = point;
				}
			}
		}
	}
	return targetPosition;
}*/


bool Pacman::get_animation_over()
{
	return animation_over;
}

bool Pacman::get_dead()
{
	return dead;
}

unsigned char Pacman::get_direction()
{
	return direction;
}

unsigned short Pacman::get_energizer_timer()
{
	return energizer_timer;
}

void Pacman::draw(bool i_victory, sf::RenderWindow& i_window)
{
	unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sf::Sprite sprite;

	sf::Texture texture;

	sprite.setPosition(position.x, position.y);

	if (1 == dead || 1 == i_victory)
	{
		if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
		{
			animation_timer++;

			texture.loadFromFile("Resources/Images/PacmanDeath" + std::to_string(CELL_SIZE) + ".png");

			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

			i_window.draw(sprite);
		}
		else
		{
			//You can only die once.
			animation_over = 1;
		}
	}
	else
	{
		texture.loadFromFile("Resources/Images/Pacman" + std::to_string(CELL_SIZE) + ".png");

		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));

		i_window.draw(sprite);

		animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
	}
}

void Pacman::reset()
{
	animation_over = 0;
	dead = 0;

	direction = 0;

	animation_timer = 0;
	energizer_timer = 0;
}

void Pacman::set_animation_timer(unsigned short i_animation_timer)
{
	animation_timer = i_animation_timer;
}

void Pacman::set_dead(bool i_dead)
{
	dead = i_dead;

	if (1 == dead)
	{
		//Making sure that the animation starts from the beginning.
		animation_timer = 0;
	}
}

void Pacman::set_position(short i_x, short i_y)
{
	position = {i_x, i_y};
}

unsigned char Pacman::getOppositeDirection(unsigned char dir) const {
	switch (dir) {
	case 0: return 2; // 右的相反方向是左
	case 1: return 3; // 上的相反方向是下
	case 2: return 0; // 左的相反方向是右
	case 3: return 1; // 下的相反方向是上
	default: return 255; // 无效方向
	}
}


unsigned char Pacman::getANYDirection(unsigned char dir) const {
	std::vector<unsigned char> directions = { 0, 1, 2, 3 }; 
	directions.erase(std::remove(directions.begin(), directions.end(), dir), directions.end());

	int randomIndex = rand() % directions.size();
	return directions[randomIndex];
}


bool Pacman::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, std::vector<Position> ghostPositions, bool allowTurnBack, std::vector<bool> ghostFriten)
{
	bool mustturn = false;
	std::array<bool, 4> walls{};
	walls[0] = map_collision(0, 0, 1 + position.x, position.y, i_map);
	walls[1] = map_collision(0, 0, position.x, position.y - 1, i_map);
	walls[2] = map_collision(0, 0, position.x - 1, position.y, i_map);
	walls[3] = map_collision(0, 0, position.x, 1 + position.y, i_map);

	const std::vector<Position> directions = { {PACMAN_SPEED, 0}, {0, -PACMAN_SPEED}, {-PACMAN_SPEED, 0}, {0, PACMAN_SPEED} };
	unsigned char bestDirection = rand()%4;
	long mindistance = 99999999;

	for (int i = 0; i < 4; ++i) {
		if ( !allowTurnBack && i == getOppositeDirection(lastDirection)) {
			/*allowTurnBack = true;*/
			continue; 
		}
		float newX = position.x + directions[i].x;
		float newY = position.y + directions[i].y;
		if (0 == walls[i]) {
			std::vector<int> allDistances;
			int id = 0;
			for (const auto& ghost : ghostPositions) {
				if (!ghostFriten[id])
				{
					if (std::abs(static_cast<int>(newX) - static_cast<int>(ghost.x)) + std::abs(static_cast<int>(newY) - static_cast<int>(ghost.y)) < 500)
					{
						unsigned int distanceFromOneOfGhost = (pow(std::abs(static_cast<int>(newX) - static_cast<int>(ghost.x)), 2) + pow(std::abs(static_cast<int>(newY) - static_cast<int>(ghost.y)), 2)) / 10;
						if (distanceFromOneOfGhost > 0)
						{
							if (distanceFromOneOfGhost > 1000)
							{
								distanceFromOneOfGhost *= 4;
							}

							distanceFromOneOfGhost = 1000000 / distanceFromOneOfGhost;
						}
						else
						{
							distanceFromOneOfGhost = 1000000;
						}
						allDistances.push_back(distanceFromOneOfGhost);
					}
				}
				else
				{
					if (std::abs(static_cast<int>(newX) - static_cast<int>(ghost.x)) + std::abs(static_cast<int>(newY) - static_cast<int>(ghost.y)) < 500)
					{
						int distanceFromOneOfGhost = (pow(std::abs(static_cast<int>(newX) - static_cast<int>(ghost.x)), 2) + pow(std::abs(static_cast<int>(newY) - static_cast<int>(ghost.y)), 2)) / 10;
						if (distanceFromOneOfGhost > 0)
						{
							if (distanceFromOneOfGhost > 500)
							{
								distanceFromOneOfGhost *= 3;
								distanceFromOneOfGhost = 1000000 / distanceFromOneOfGhost;
							}
							else if (distanceFromOneOfGhost < 50)
							{
								distanceFromOneOfGhost = 4000000 / distanceFromOneOfGhost;
							}
							
						}
						else
						{
							distanceFromOneOfGhost = 10000000;
						}
						allDistances.push_back(distanceFromOneOfGhost*( - 1));
					}
				}
				
				++id;
			}
			/*std::nth_element(allDistances.begin(), allDistances.begin() + 1, allDistances.end());*/
			int nearestDistanceSum = std::accumulate(allDistances.begin(), allDistances.end(), 0u);

			if (nearestDistanceSum < mindistance) {
				mindistance = nearestDistanceSum;
				bestDirection = i;
				std::cout << int(i) << std::endl;
			}
			
			if(nearestDistanceSum > 10000)
			{
				allowTurnBack = true;
			}
			
		}
	}
	
	
	//std::cout << ghostPositions[0].x << ghostPositions[0].x << std::endl;

		direction = bestDirection;
		if(lastDirection == direction)
		{
			allowTurnBack = false;
		}
		lastDirection = bestDirection;
	
	//std::cout << std::to_string(direction) << std::endl;
	

	if (0 == walls[direction])
	{
		switch (direction)
		{
			case 0:
			{
				position.x += PACMAN_SPEED;

				break;
			}
			case 1:
			{
				position.y -= PACMAN_SPEED;

				break;
			}
			case 2:
			{
				position.x -= PACMAN_SPEED;

				break;
			}
			case 3:
			{
				position.y += PACMAN_SPEED;
			}
			
		}
	}

	if (-CELL_SIZE >= position.x)
	{
		position.x = CELL_SIZE * MAP_WIDTH - PACMAN_SPEED;
	}
	else if (CELL_SIZE * MAP_WIDTH <= position.x)
	{
		position.x = PACMAN_SPEED - CELL_SIZE;
	}

	if (1 == map_collision(1, 0, position.x, position.y, i_map)) //When Pacman eats an energizer...
	{
		//He becomes energized!
		energizer_timer = static_cast<unsigned short>(ENERGIZER_DURATION / pow(2, i_level));
	}
	else
	{
		energizer_timer = std::max(0, energizer_timer - 1);
	}
	return allowTurnBack;
}

Position Pacman::get_position()
{
	return position;
}