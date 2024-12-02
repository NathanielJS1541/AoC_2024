#include "file.h"
#include "algos.h"
#include "parts.h"
#include <cmath>
#include <iostream>

static std::vector<std::vector<int>*> g_vectors;

void day2prep(void)
{	
	const char* input_filename = "./inputs/d2.txt";
	
	/* Count number of lines and update vectors for each row */
	std::fstream file = open_file_read(input_filename);
	const int n_rows = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
	file.close();
	for (int n = 0; n < n_rows; n++)
	{
		g_vectors.push_back(new std::vector<int>());
	}
	

	/* Read data into the vectors, creating a new row for each line */
	(void)read_file_into_vecs(input_filename, ROWS, g_vectors);
}

int day2part1(void)
{
	int n_safe = 0;
	for (auto v : g_vectors)
	{
		std::tuple<int, int> steps = determine_step_sequence(v);
		int n_steps = std::get<0>(steps);
		int max_step = std::get<1>(steps);
		/* Number of steps taken is equal to the number of elements minus the first, obviously.
		 * AND the maximum step taken is less than 4, (1-3) permitted */
		if ((n_steps == ((int)v->size() - 1)) && (max_step < 4)) n_safe++;
	}
	return n_safe;
}

int day2part2(void)
{
	int n_safe = 0;
	for (auto v : g_vectors)
	{
		std::tuple<int, int> steps = determine_step_sequence(v);
		int n_steps = std::get<0>(steps);
		int max_step = std::get<1>(steps);
		int steps_off = abs(n_steps - (int)v->size());
		if ((steps_off < 2) && (steps_off > 0) && (max_step < 4)) n_safe++;

		std::cout << "(";
		for (auto x : *v) { std::cout << x << " "; }
		std::cout << ") " << steps_off << " " << max_step << " " << n_safe << std::endl;
	}
	return n_safe;
}

void day2cleanup(void)
{
	for (auto v : g_vectors)
	{
		delete v;
	}
}

int day2(void)
{
	day2prep();
	int part1 = day2part1();
	std::cout << part1 << std::endl;
	int part2 = day2part2();
	std::cout << part2 << std::endl;
	day2cleanup();
	return part2;
}
