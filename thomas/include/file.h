#ifndef __FILE_H
#define __FILE_H

#include <fstream>
#include <vector>

std::fstream open_file_read(const char* filename);

int read_file_into_vec(const char* filename, std::vector<std::vector<int>*> vec_ptrs);

#endif /* __FILE_H */
