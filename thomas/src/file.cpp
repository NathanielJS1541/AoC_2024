#include "file.h"
#include <string>
#include <sstream>

std::fstream open_file_read(const char* filename)
{
	std::fstream istrm;
	istrm.open(filename, std::fstream::in);
	return istrm;
}

/*	read_file_into_vec
 *
 * INPUTS filename ->	c string filename, points to one of the input files in ./inputs/ usually.
 * 	  vec_ptrs ->	A vector of vector pointers, the number of vector points in the parent 
 * 	  		should be equal to the number of columns that you need to read out of
 * 	  		the input.
 *
 * NOTE: Files need to be altered to remove blocks of whitespace. I know AoC think they're clever by
 * 	 padding things with 4 spaces, but it's not clever, remove this by piping the file into `tr -s ' '`.
 */
int read_file_into_vec(const char* filename, std::vector<std::vector<int>*> vec_ptrs)
{
	int n_items_read = 0;
	std::fstream file = open_file_read(filename);
	unsigned column = 0;
	const unsigned num_cols = vec_ptrs.size();
	if (file.is_open())
	{
		std::string full_line;
		int item;
		while(std::getline(file, full_line, '\n'))
		{
			std::stringstream line(full_line);
			std::string item_str;
			while(std::getline(line, item_str, ' '))
			{
				item = std::stoi(item_str);
				vec_ptrs[column]->push_back(item);
				column = (column + 1) % num_cols;
				n_items_read++;
			}
		}

		file.close();
	}
	return n_items_read;
}
