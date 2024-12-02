#include "file.h"
#include "algos.h"
#include "parts.h"
#include <cmath>
#include <iostream>

static std::vector<std::vector<int>*> g_vectors;

void day1prep(void)
{
	g_vectors = { new std::vector<int>(), new std::vector<int>() };

	const char* input_filename = "./inputs/d1p1.txt";

	(void)read_file_into_vecs(input_filename, COLUMNS, g_vectors);

	for (auto v : g_vectors)
	{
		(void)sort_vector(v, ASCENDING);
	}
}

int day1part1(void)
{	
	unsigned sum = 0U;
	std::vector<int> primary_column = *(g_vectors[0]);
	std::vector<int> secondary_column = *(g_vectors[1]);

	for (unsigned n = 0U; n < primary_column.size(); n++)
	{
		unsigned distance = std::abs(primary_column.at(n) - secondary_column.at(n));
		sum += distance;
	}

	return (int)sum;
}

int day1part2(void)
{
	std::unordered_map<int, int> occurance_table = create_occurance_table(g_vectors[1]);
	std::vector<int> primary_column = *(g_vectors[0]);

	unsigned similarity = 0;
	for (int x : primary_column)
	{
		similarity += (x * occurance_table[x]);
	}

	return (int)similarity;
}

void day1cleanup(void)
{
	for (auto v : g_vectors)
	{
		delete v;
	}
}

int day1(void)
{
	day1prep();
	int part1 = day1part1();
	std::cout << part1 << std::endl;
	int part2 = day1part2();
	std::cout << part2 << std::endl;
	day1cleanup();
	return part2;
}
