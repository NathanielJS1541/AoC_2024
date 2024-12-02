#ifndef ALGOS_H
#define ALGOS_H

#include <algorithm>
#include <vector>
#include <unordered_map>

enum e_sort_order {
	ASCENDING,
	DESCENDING
};

bool sort_vector(std::vector<int>* p_vec, e_sort_order order);

std::unordered_map<int, int> create_occurance_table(std::vector<int>* p_vec);

#endif /* ALGOS_H */
