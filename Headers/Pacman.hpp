#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
class Pacman
{
	//This is used for the death animation.
	bool animation_over;
	//Am I dead?
	bool dead;

	unsigned char direction;

	//More timers!
	unsigned short animation_timer;
	unsigned short energizer_timer;

	//Current location of this creature, commonly known as Pacman.
	Position position;

private:
	std::vector<std::vector<int>> grid;
	bool gridInitialized = false;
	struct Point {
		int x, y;
		bool operator==(const Point& other) const {
			return x == other.x && y == other.y;
		}
		bool operator!=(const Point& other) const {
			return !(*this == other);
		}
	};

	struct PointHash {
		size_t operator()(const Point& p) const {
			return std::hash<int>()(p.x) ^ std::hash<int>()(p.y);
		}
	};

	struct Node {
		Point position;
		int cost;
		int heuristic;
		Point parent;
	};

	struct CompareNode {
		bool operator()(const Node& a, const Node& b) const {
			return a.cost + a.heuristic > b.cost + b.heuristic;
		}
	};

public:
	Pacman();
	
	bool get_animation_over();
	bool get_dead();

	unsigned char get_direction();

	unsigned short get_energizer_timer();
	//std::vector<Point> findPath(const Point& start, const Point& goal, const std::vector<std::vector<int>>& grid);
	//Point findSafestPoint(const Pacman::Point& pacmanPosition, const std::vector<Position>& ghostPositions, const std::vector<std::vector<int>>& grid);
	//unsigned char calculateNextMove(const Position& startPosition, const std::vector<Position>& ghostPositions, const std::vector<std::vector<int>>& grid);
	void draw(bool i_victory, sf::RenderWindow& i_window);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool i_dead);
	void set_position(short i_x, short i_y);
	//void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, GhostManager& ghostManager);
	void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, std::vector<Position> ghostPositions);
	Position get_position();


};