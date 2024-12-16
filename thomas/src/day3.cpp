#include "parts.h"
#include "file.h"
#include <iostream>
#include <regex>
#include <stdio.h>

static void day3prep(void);
static int day3part1(void);
static int day3part2(void);
static void day3cleanup(void);

static std::string buffer;

typedef struct s_match
{
	std::ssub_match matched;
	long posn;
	bool discard;
} T_Match;

static inline long get_position_in_buffer(std::ssub_match match)
{
	return (match.first - buffer.begin());
}

static inline long execute_mul_op(std::string mul_string)
{
	int a, b;
	(void)sscanf(mul_string.c_str(), "mul(%d, %d)", &a, &b);
	return a * b;
}

static inline std::vector<T_Match>::iterator discard_muls(std::vector<T_Match>::iterator it, std::vector<T_Match>& matches, long* last_dont, long do_posn)
{
	// Find first mul match after the `don't()`.
	long dont_posn = *last_dont;
	for (; ((it < matches.end()) && (it->posn < dont_posn)); it++);

	// Find each mul match from the `don't()` til the `do()`, set each ones `discard` field to true.
	for(; ((it < matches.end()) && (it->posn < do_posn)); it++)
	{
		it->discard = true;
	}

	if ((it->posn > do_posn) && (it < matches.end())) it--;

	*last_dont = 0;
					
	return it;
}

static void day3prep()
{
	buffer = read_file_into_string("./inputs/d3.txt");
}

static int day3part1()
{
	long p1 = 0;
	std::regex search_pattern{"mul\\([0-9]{1,3}\\,[0-9]{1,3}\\)"};
	std::string local_buff = buffer;
	for (std::smatch match; std::regex_search(local_buff, match, search_pattern); local_buff = match.suffix())
	{
			p1 += execute_mul_op(match[0].str());
	}

	return p1;
}

static int day3part2()
{
	long p2 = 0;
	std::vector<T_Match> mul_matches;
	std::regex mul_pattern{"mul\\([0-9]{1,3}\\,[0-9]{1,3}\\)"};
	std::regex cond_pattern{"(do\\(\\))|(don't\\(\\))"};
	// Perform a double-pass search over the buffer, storing the mul matches, then for each do or don't match (two seperate passes) mark the mul matches to be kept or thrown.
	std::string local_buff = buffer;

	std::string::const_iterator search_buff(buffer.cbegin());

	for (std::smatch matches; std::regex_search(search_buff, buffer.cend(), matches, mul_pattern); search_buff = matches.suffix().first)
	{	// Iterate over the buffer using an iterator (no replacement is important) finding each instance of the pattern.
		auto match = matches[0];
		long start_posn = get_position_in_buffer(match);
		mul_matches.push_back({match, start_posn, false});
	}

	long posn_of_last_dont = 0;
	auto mul_it = mul_matches.begin();
	search_buff = buffer.cbegin();

	for (std::smatch matches; std::regex_search(search_buff, buffer.cend(), matches, cond_pattern); search_buff = matches.suffix().first)
	{
		// When encountering a `don't()` we need to raise a flag such that when we encounter the next `do()`, or the end of the file, we can backtrack from the position of the
		// `do()` or EOF to the `don't()` and discard all the mul_matches in that positional range. 
		// e.g. mul(8, 1)#$try()!get()do()mul(1, 1)don't()!*(+mul(1, 2)mul(5, 3)do()mul(10, 10) 
		// Should resolve to give us mul(8,1) mul(1,1), mul(10,10) by backtracking from the last `do()` to the first `don't()` and setting mul(5,3) and mul(1,2) as invalid.
		auto match = matches[0];
		auto match_str = match.str();

		switch(match_str[2])
		{
			case '(': // do()
			{ // Check if we've got a waiting don't. If we do, retroactively invalidate any mul matches.
				if (posn_of_last_dont > 0)
				{
					mul_it = discard_muls(mul_it, mul_matches, &posn_of_last_dont, get_position_in_buffer(match));
				}
			} break;
			case 'n': // don't()
			{
				posn_of_last_dont = (posn_of_last_dont == 0) ? get_position_in_buffer(match) : posn_of_last_dont; 
			} break;
		}
	}
	

	if (posn_of_last_dont > 0) (void)discard_muls(mul_it, mul_matches, &posn_of_last_dont, (long)(buffer.end() - buffer.begin()));

	for (auto m : mul_matches)
	{
		if (!m.discard)
		{
			p2 += execute_mul_op(m.matched.str());	
		}
	}

	return p2;
}

static void day3cleanup()
{
	buffer.empty();
}

int day3(void)
{
	day3prep();
	int p1 = day3part1();
	int p2 = day3part2();
	std::cout << p1 << " " << p2 << std::endl;
	day3cleanup();
	return 0;
}
