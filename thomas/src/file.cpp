#include "file.h"
#include <string>
#include <sstream>
#include <iostream>
#include <array>

std::fstream open_file_read(const char* filename)
{
	std::fstream istrm;
	istrm.open(filename, std::fstream::in);
	return istrm;
}

int read_line_by_format(std::stringstream& line, e_read_format format, std::vector<std::vector<int>*> pp_vecs)
{
	std::string line_part;
	int n_read = 0;
	int item; 
	int column = 0;
	int n_vecs = pp_vecs.size();
	static int row = 0;

	while(std::getline(line, line_part, ' '))
	{
		item = std::stoi(line_part);
		switch (format)
		{
			case (COLUMNS):
			{
				pp_vecs[column]->push_back(item);
				column = (column + 1) % n_vecs;
			} break;

			case (ROWS):
			{
				pp_vecs[row]->push_back(item);
			} break;

			default: break;
		}
		n_read++;
	}

	if (format == ROWS) row++;

	return n_read;
}

/*	read_file_columns_into_vecs
 *
 * INPUTS filename ->	c string filename, points to one of the input files in ./inputs/ usually.
 * 	  pp_vecs ->	A vector of vector pointers, the number of vector points in the parent 
 * 	  		should be equal to the number of columns that you need to read out of
 * 	  		the input.
 *
 * NOTE: Files need to be altered to remove blocks of whitespace. I know AoC think they're clever by
 * 	 padding things with 4 spaces, but it's not clever, remove this by piping the file into `tr -s ' '`.
 */
int read_file_into_vecs(const char* filename, e_read_format format, std::vector<std::vector<int>*> pp_vecs)
{
	int n_items_read = 0;
	std::fstream file = open_file_read(filename);

	if (file.is_open())
	{
		std::string line; 
		while(std::getline(file, line, '\n'))
		{
			std::stringstream line_stream(line);
			n_items_read += read_line_by_format(line_stream, format, pp_vecs);
		}
		file.close();
	}
	return n_items_read;
}

std::string read_file_into_string(const char* filepath)
{
	std::fstream file = open_file_read(filepath);
	std::string file_as_string;
	if (file.is_open())
	{
		std::ostringstream stream; 
		stream << file.rdbuf();
		file_as_string =  stream.str();
	}

	{
		file_as_string.empty();
	}

	return file_as_string;
}
