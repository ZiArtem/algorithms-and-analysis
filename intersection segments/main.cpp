#include <vector>
#include <iostream>
#include "intersect_metod.h"
#include "AVLtree.h"

using namespace std;

int main() {
	std::vector <Segment> vec;
	int size_vec = 4000;
	vec.resize(size_vec);
	get_rand_segm(vec, size_vec, 500);

	for (int i = 0; i < 20; i++) {
		get_rand_segm(vec, size_vec, 0.1);
		print_res(vec, intersection_naive_eff);
		print_res(vec, intersection_naive);
	}
	return 0;
}