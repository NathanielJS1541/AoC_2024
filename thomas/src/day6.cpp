#include "parts.h"
#include "file.h"
#include <unordered_map>
#include <algorithm>

#define GRID_SIZE 16900
#define GRID_ROWS 130

enum e_direction
{
	LEFT = 0x3C, 	// <
	RIGHT = 0x3E,	// >
	UP = 0x5E,	// ^
	DOWN = 0x76,	// v
};

static std::array<char, GRID_SIZE> grid;
static std::array<bool, GRID_SIZE> visited;
static std::unordered_map<int, std::vector<int>*> vertical;
static std::unordered_map<int, std::vector<int>*> horizontal;

static size_t posn = 0;
static e_direction direction;

static bool search_for_objects(size_t posn, size_t new_posn);
static void day6prep(void);
static int day6part1(void);
static int day6part2(void);
static void day6cleanup(void);

static inline void print_tables(void)
{
	for (size_t n = 0; (n < GRID_ROWS); n++)
	{
		auto v = vertical.find(n);
		auto h = horizontal.find(n);
		if (v != vertical.end())
		{
			std::cout << n << " -> ";
			for (auto y : (*(v->second))) { std::cout << y << " "; }
			std::cout << std::endl;
		}

		if (h != horizontal.end())
		{
			std::cout << n << " -> ";
			for (auto x : (*(h->second))) { std::cout << x << " "; }
			std::cout << std::endl;
		}
	}
}

static inline void print_grid(void)
{
	for (size_t i = 0; (i < GRID_SIZE); i++)
	{
		if ((i > 0) && ((i % GRID_ROWS) == 0)) 
		{
			std::cout << std::endl;
		}
		
		if (visited[i])
		{
			std::cout << "O";
		}
		else if (i == posn) 
		{
			std::cout << (char)direction;
		}
		else
		{
			std::cout << grid[i];
		}
	}
	std::cout << std::endl;
}

static inline bool out_of_bounds(size_t posn, size_t new_posn, e_direction dir)
{
	switch (dir)
	{
		case (LEFT):
		case (RIGHT):
		{
			// Moved off the board laterally.
			return (((posn % GRID_ROWS) == 0) && ((new_posn % GRID_ROWS) != 0));
		}

		case (UP):
		case (DOWN):
		{
			// Moved off the board vertically.
			return ((new_posn < 0) || (new_posn > GRID_SIZE));
		}
	}
	
	return false;
}

static inline size_t move_guard(void)
{
	switch (direction)
	{
		case (LEFT):
		{
			return posn - 1;
		}

		case (RIGHT):
		{
			return posn + 1;
		}

		case (UP):
		{
			return posn - GRID_ROWS;
		}

		case (DOWN):
		{
			return posn + GRID_ROWS;
		}
	}

	return posn;
}

static inline e_direction turn_guard(void)
{
	switch (direction)
	{
		case (LEFT):
		{
			return UP;
		}
		
		case (RIGHT):
		{
			return DOWN;
		}

		case (UP):
		{
			return RIGHT;
		}

		case (DOWN):
		{
			return LEFT;
		}
	}
	
	return LEFT;
}

static inline bool path_guard(size_t* spaces_moved)
{	
	// If posn modulus the GRID_ROWS value is zero, we may have exceeded the size of the grid.
	// This only happens if moving horizontally. If moving vertically we need to check the posn
	// is > 0 and < GRID_SIZE. 	
	size_t new_posn = 0;

	new_posn = move_guard();
	// Moving off the board?
	if (out_of_bounds(posn, new_posn, direction)) return false;
	// Object?
	if (grid[new_posn] != '.')
	{
		// Rotate the guard for the next move.
		direction = turn_guard();
	}
	else
	{
		posn = new_posn;
		if (!visited[posn])
		{
			visited[posn] = true;
			(*spaces_moved)++;
		}
	}

	return true;
}

static inline void store_hit_object(size_t posn)
{
	int y = (posn % GRID_ROWS);
	int x = (posn / GRID_ROWS);

	auto v = vertical.find(y);

	if (v == vertical.end())
	{
		vertical[y] = new std::vector<int>();
		vertical[y]->push_back(x);
	}
	else
	{
		auto vec = v->second;
		if (std::find(vec->begin(), vec->end(), x) == vec->end())
		{
			vec->push_back(x);
		}
	}

	auto h = horizontal.find(x);

	if (h == horizontal.end())
	{
		horizontal[x] = new std::vector<int>();
		horizontal[x]->push_back(y);
	}
	else 
	{
		auto vec = h->second;
		if (std::find(vec->begin(), vec->end(), y) == vec->end())
		{
			vec->push_back(y);
		}
	}
}

static bool inline find_objects_in_path(std::vector<int>* path, int limit, bool greater_than_limit, size_t next_posn)
{
	if ((grid[next_posn] == '#') || (visited[next_posn] == true))
	{
		return false; // It's not possible to form a loop by adding an object if it's already there.
	}

	for (auto it = path->begin(); (it < path->end()); it++)
	{
		int n = *it;

		if (	((greater_than_limit) && (n > limit))
			|| ((!greater_than_limit) && (n < limit))	)
		{
			return true;
		}
	}

	return false;
}

static bool search_for_objects(size_t posn, size_t new_posn)
{
	bool object_found = false;
	std::vector<int>* objects = nullptr;
	std::unordered_map<int, std::vector<int>*>::iterator table_item;
	int y = (posn % GRID_ROWS);
	int x = (posn / GRID_ROWS);

	switch (direction)
	{
		case (LEFT):
		case (RIGHT):
		{	// Search from the current row up to the top row (LEFT) or from the 
			// current row down to the bottom (RIGHT) for any objects we've already hit.
			table_item = vertical.find(y);
			if (table_item != vertical.end())
			{
				objects = vertical[y];

				// Find all objects above or below the current one (column is the same but row is less than the current row).
				bool search_below = (direction == RIGHT);
				object_found = find_objects_in_path(objects, x, search_below, new_posn);
			}
		} break;

		case (UP):
		case (DOWN):
		{
			table_item = horizontal.find(x);
			if (table_item != horizontal.end())
			{
				objects = horizontal[x];

				bool search_right = (direction == UP);
				object_found = find_objects_in_path(objects, y, search_right, new_posn);
			}
		} break;
	}

	if (object_found)
	{
		switch (direction)
		{
			case (LEFT):
			{
				visited[posn - 1] = true;
			} break;
			case (RIGHT):
			{
				visited[posn + 1] = true;
			} break;
			case (DOWN):
			{
				visited[posn + GRID_ROWS] = true;
			} break;
			case (UP):
			{
				visited[posn - GRID_ROWS] = true;
			} break;
		}
	}

	return object_found;
}

static void day6prep(void)
{
	size_t n = read_file_into_matrix("./inputs/d6.txt", &grid);
	if (n != GRID_SIZE)
	{
		std::cerr << "Wrong size read into matrix (" << n << ") vs. (" << GRID_SIZE << ")\n";
		exit(0);
	}

	// Find starting position (of guard, signified with ^ character).
	char c;
	for (size_t i = 0; (i < GRID_SIZE); i++) 
	{
		c = grid[i];
	
		// < or > or ^ or v
		if ((c == (char)LEFT) || (c == (char)RIGHT) || (c == (char)UP) || (c == (char)DOWN))
		{
			posn = i;
			grid[i] = '.';
			////visited[i] = true;
			direction = (e_direction)c;
			break;
		}
	}
}

static int day6part1(void)
{
	//print_grid();
	size_t spaces_moved = 1;
	while (path_guard(&spaces_moved));
	return (int)spaces_moved;
}

static int day6part2(void)
{
	size_t n_loops = 0;
	size_t new_posn = 0;
	bool moving = true;

	while(moving)
	{
		new_posn = move_guard();
		// Moving off the board?
		if (out_of_bounds(posn, new_posn, direction))
		{
			moving = false;
			break;
		}

		// Check if there is an object you have already hit perpendicular to your path.
		if (search_for_objects(posn, new_posn)) n_loops++;

		// Object?
		if (grid[new_posn] != '.')
		{
			// Store our object position in both object tables.
			store_hit_object(new_posn);
			// Rotate the guard for the next move.
			direction = turn_guard();
		}
		else
		{
			posn = new_posn;
		}
	}

	return (int)n_loops;
}

static void day6cleanup(void)
{
	for (size_t n = 0; (n < GRID_ROWS); n++)
	{
		auto v = vertical.find(n);
		auto h = horizontal.find(n);

		if (v != vertical.end()) delete vertical[n];
		if (h != horizontal.end()) delete horizontal[n];
	}
}

int day6(void)
{
	day6prep();
	// It is not possible to run both with the current implementation
	int p1 = 0; // day6part1();
	int p2 = day6part2();
	day6cleanup();
	std::cout << p1 << " " << p2 << std::endl;
	return 0;
}
