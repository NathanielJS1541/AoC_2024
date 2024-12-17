#include "parts.h"
#include "algos.h"
#include "file.h"
#include <array>
#include <iostream>

#define MATRIX_SIZE 19600
static const size_t n_in_row = 140;
static std::array<char, MATRIX_SIZE> matrix{};
static std::array<char, MATRIX_SIZE> solve;

static void day4prep(void);
static int day4part1(void);
static int day4part2(void);
static void day4cleanup(void);

static const std::string to_find{"XMAS"};
static const std::string to_find_rev{"SAMX"};

static inline bool check_for_message(std::array<char, 4> subarr)
{
	std::string comp_string(subarr.data(), 4);
	return ((comp_string == to_find) || (comp_string == to_find_rev));
}

static void day4prep()
{
	size_t n = read_file_into_matrix("./inputs/d4.txt", &matrix);
	if (n != MATRIX_SIZE) 
	{
		std::cout << "Size of matrix read-in is wrong! " << n << " vs. " << MATRIX_SIZE << std::endl;
		exit(0);
	}

	solve.fill('.');
}

static int day4part1()
{
	unsigned long count = 0;
	std::array<char, 4> subarr{0, 0, 0, 0};
	for (size_t n = 0; (n < MATRIX_SIZE); n++)
	{
		bool horiz_space = false;
		bool vertic_space = false;
		
		// Space to check horizontal?
		if ((n_in_row - (n % n_in_row)) >= 4)
		{
			horiz_space = true;
			subarr = {matrix[n], matrix[n+1], matrix[n+2], matrix[n+3]};
			if (check_for_message(subarr))
			{
				count++;
			}
		}

		// Space to check vertical?
		if (n < (MATRIX_SIZE - (n_in_row * 3))) 
		{
			vertic_space = true;
			subarr = {matrix[n], matrix[n + n_in_row], matrix[n + (n_in_row * 2)], matrix[n + (n_in_row * 3)]};
			if (check_for_message(subarr))
			{
				count++;
			}
		}

		// Space to check forward horizontal?
		if (horiz_space && vertic_space)
		{
			subarr = {	matrix[n], matrix[n + n_in_row + 1],
					matrix[n + (n_in_row * 2) + 2], matrix[n + (n_in_row * 3) + 3]};
			if (check_for_message(subarr))
			{
				count++;
			}
		}

		// Space to check backward horizontal?
		if (((n % n_in_row) >= 3) && vertic_space)
		{
			subarr = {	matrix[n], matrix[n + n_in_row - 1],
					matrix[n + (n_in_row * 2) - 2], 
					matrix[n + (n_in_row * 3) - 3]};
			if (check_for_message(subarr))
			{
				count++;
			}
		}
	}

	return count;
}

static int day4part2()
{
	// Search for A surrounded by S S and M M on either diagonal.
	// e.g. 
	// 	S		M
	// 		A
	// 	S		M
	// or
	// 	S		S
	// 		A
	// 	M		M

	int count = 0;
	const char A = 65;
	const char M = 77;
	const char S = 83;

	for (unsigned n = n_in_row; (n < (MATRIX_SIZE - n_in_row)); n++)
	{
		char c = matrix[n];
		// At least one character away from either edge.
		if (((n % n_in_row) >= 1) && ((n % n_in_row) < (n_in_row - 1)))
		{
			// Check diagonal.
			char forward_diag = matrix[n - (n_in_row) - 1] | matrix[n + (n_in_row) + 1];
			char backward_diag = matrix[n - (n_in_row) + 1] | matrix[n + (n_in_row) - 1];
			if ((c == A) && (forward_diag == (M | S)) && (backward_diag == (M | S)))
			{
					count++;
			}
		}
	}

	return count;
}

static void day4cleanup()
{
}

int day4()
{
	day4prep();
	int p1 = day4part1();
	int p2 = day4part2();
	day4cleanup();
	std::cout << p1 << " " << p2 << std::endl;
	return 0;
}
