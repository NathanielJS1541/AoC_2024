#include "parts.h"
#include "file.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#include <algorithm>

static std::unordered_map<int, std::vector<int>*> preceeding;
static std::vector<std::vector<int>> orders;
static std::vector<std::vector<int>> pages;

static void day5prep(void);
static int day5part1(void);
static int day5part2(void);

static void day5prep(void)
{
	std::fstream file = open_file_read("./inputs/d5.txt");
	read_delimited_data_into_vector(file, &orders, '|');
	read_delimited_data_into_vector(file, &pages, ',');
	file.close();

	// Take the orders and store them in the preceeding map, wherein the item n
	// contains a list of items that MUST come before it.
	for (auto order : orders) 
	{
		int n = order[1];
		
		if (preceeding.find(n) == preceeding.end())
		{
			preceeding[n] = new std::vector<int>();
		}
		
		preceeding[n]->push_back(order[0]);
	}

	for (auto it = preceeding.begin(); (it != preceeding.end()); it++)
	{	// Sort the vector keys.
		auto vec = it->second;
		std::sort(vec->begin(), vec->end());
	}
}

static int day5part1(void)
{
	bool in_order = true;
	size_t middle_page_n = 0;
	
	for (auto p : pages)
	{
		in_order = true;
		for (auto it = p.begin(); (it < p.end()); it++)
		{
			int n = *it;
			auto item = preceeding.find(n);
			if (item != preceeding.end())
			{
				std::vector<int>* prec = item->second;
				for (auto next = it + 1; (next < p.end()); next++)
				{	// If the associated vector of items that *should* preceed the target value contains any of the next values.
					// then the numbers are out of order.
					auto prec_item = std::find(prec->begin(), prec->end(), *next);
					if (prec_item != prec->end())
					{
						break;
					}
				}
			}
			if (!in_order) break;
		}

		std::cout << std::endl;

		if (in_order)
		{
			int n_items = p.size();
			int middle_item = n_items / 2;
			middle_page_n += p.at(middle_item);
		}
	}

	return (int)middle_page_n;
}

static int day5part2(void)
{
	int middle_page_n = 0;

	for (auto p : pages)
	{
		bool reordered = false;

		for (auto it = p.begin(); (it < p.end()); it++)
		{
			unsigned to_swap = 0;
			int n = *it;			

			auto item = preceeding.find(n);
			if (item != preceeding.end())
			{
				std::vector<int>* prec = item->second;
				for (auto next = (it + 1); (next < p.end()); next++)
				{
					auto comp_item = std::find(prec->begin(), prec->end(), *next);
					if (comp_item != prec->end())
					{	// Store index to be swapped.
						to_swap = (next - p.begin());
						reordered = true;
					}
				}

				if (to_swap > 0) 
				{
					std::swap(*it, p[to_swap]);
					to_swap = 0;
					it--;
				}
			}
		}

		if (reordered)
		{
			int n_items = p.size();
			int middle_item = n_items / 2;
			middle_page_n += p.at(middle_item);
		}
	}

	return middle_page_n;
}

static void day5cleanup(void)
{
	for (auto it = preceeding.begin(); (it != preceeding.end()); it++)
	{
		delete it->second;
	}
}

int day5(void)
{
	day5prep();
	int p1 = day5part1();
	int p2 = day5part2();
	day5cleanup();
	std::cout << p1 << " " << p2 << std::endl;
	return 0;
}
