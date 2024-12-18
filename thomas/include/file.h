#ifndef __FILE_H
#define __FILE_H

#include <fstream>
#include <vector>
#include <array>
#include <iostream>

enum e_read_format { COLUMNS, ROWS };

std::fstream open_file_read(const char* filename);

int read_line_by_format(std::stringstream& line, e_read_format format, std::vector<std::vector<int>*> pp_vecs);

int read_file_into_vecs(const char* filename, e_read_format format, std::vector<std::vector<int>*> pp_vecs);

std::string read_file_into_string(const char* filepath);

template<std::size_t SIZE>
size_t read_file_into_matrix(const char* filepath, std::array<char, SIZE>* arr)
{
	std::fstream file = open_file_read(filepath);
	char c;
	unsigned n = 0;
	if (file.is_open())
	{
		while((file.good()) && (file.get(c)))
		{
			if ((unsigned)c > 0x40)
			{
				(*arr)[n++] = c;
			}
		}

		file.close();
	}

	return n;
}

unsigned read_delimited_data_into_vector(std::fstream& filestream, std::vector<std::vector<int>>* vec, char delimiter);


#endif /* __FILE_H */
