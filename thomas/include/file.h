#ifndef __FILE_H
#define __FILE_H

#include <fstream>
#include <vector>

enum e_read_format { COLUMNS, ROWS };

std::fstream open_file_read(const char* filename);

int read_line_by_format(std::stringstream& line, e_read_format format, std::vector<std::vector<int>*> pp_vecs);

int read_file_into_vecs(const char* filename, e_read_format format, std::vector<std::vector<int>*> pp_vecs);

std::string read_file_into_string(const char* filepath);

#endif /* __FILE_H */
