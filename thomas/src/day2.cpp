#include "file.h"
#include "algos.h"
#include "parts.h"
#include <cmath>
#include <iostream>

static std::vector<std::vector<int>*> g_vectors;

static bool check_sequence_is_safe(std::vector<int>* p_vec, bool allow_one_error);
static int check_step_sequence(std::vector<int> steps);

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
		if(check_sequence_is_safe(v, false)) n_safe++;
	}
	return n_safe;
}

int day2part2(void)
{
	int n_safe = 0;
	for (auto v : g_vectors)
	{
		bool safe = false;
		if (v->size() <= 2) safe = true; /* You can always remove one item to make a list valid */
		if (check_sequence_is_safe(v, true)) safe = true;
		if (safe) n_safe++;
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

static bool check_sequence_is_safe(std::vector<int>* p_vec, bool allow_one_error)
{
	bool safe = false;
	std::vector<int> steps = *p_vec;
	int err_idx = check_step_sequence(steps);
	if (err_idx >= 0)
	{
		if (allow_one_error)
		{	
			for (unsigned n = 0; (n < p_vec->size()); n++)
			{
				std::vector<int> new_vec = *p_vec;
				new_vec.erase(new_vec.begin() + n);
				if (check_sequence_is_safe(&new_vec, false))
				{
					safe = true;
					break;
				}
			}	
		}
	} 
	else
	{
		safe = true;
	}

	return safe;
}

/* Check the step sequence passes the safe criteria, if not return the index in the step sequence that doesn't.
 * Returns -1 if pass */
static int check_step_sequence(std::vector<int> steps)
{
	const int maximum_step = 3;
	int trend = 0;
	int idx = -1;
	bool err = false;
	if (steps.size() == 1) return -1;
	
	for (auto it = std::begin(steps); ((it + 1) < std::end(steps)); it++)
	{
		int c = *it;
		int n = *(it + 1);
		int d = (n - c);
		int loc_trend = trend;
		

		if (abs(d) > maximum_step) err = true;
		
		if (d < 0) loc_trend--;
		else if (d > 0) loc_trend++;
		else if (d == 0) err = true;	

		if (	((d < 0) && (trend > 0) && (loc_trend < trend))
			|| ((d > 0) && (trend < 0) && (loc_trend > trend)))
		{
			err = true;
		}

		trend = loc_trend;
	
		if (err)
		{
			idx = it - std::begin(steps); 
			break;
		}
	}
	
	return idx;
}
