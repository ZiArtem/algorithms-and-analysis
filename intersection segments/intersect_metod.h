#pragma once
#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <numeric>
#include <algorithm>
#include "AVLtree.h"
using namespace std;

template <class T>
void print_res(std::vector <Segment> vec, T f);

void get_rand_segm(std::vector <Segment>& vec, int size_vec, double r);

bool intersection_naive(int& s1, int& s2, std::vector <Segment> vec);
bool intersection_naive_eff(int& s1, int& s2, std::vector <Segment> vec);

bool intersection(Segment a, Segment b);
double direction(Point p1, Point p2, Point p3);
bool on_segment(Point p1, Point p2, Point p3);


void get_rand_segm(std::vector <Segment>& vec, int size_vec, double del) {
	std::mt19937 gen;
	int x, y;
	random_device rd{};
	mt19937 gen1{ rd() };
	normal_distribution<> d{};
	gen.seed(static_cast<unsigned int>(time(0)));
	for (auto& v : vec) {
		x = gen() % 1000;
		y = gen() % 1000;

		v.p1.x = x + d(gen1) * del;
		v.p1.y = y + d(gen1) * del;
		v.p2.x = x + d(gen1) * del;
		v.p2.y = y + d(gen1) * del;
	}
	for (int i = 0; i < vec.size(); i++) {
		vec[i].g = i;
	}
}

bool intersection_naive(int& s1, int& s2, std::vector <Segment> vec) {
	bool b = false;
	for (auto i = 0; i < vec.size(); i++) {
		for (auto j = i + 1; j < vec.size(); j++) {
			if (intersection(vec[i], vec[j])) {
				b = true;
				s1 = i;
				s2 = j;
				return true;
			}
		}
	}
	return b;
}

bool intersection(Segment s1, Segment s2) {
	double d[4];
	d[0] = direction(s2.p1, s2.p2, s1.p1);
	d[1] = direction(s2.p1, s2.p2, s1.p2);
	d[2] = direction(s1.p1, s1.p2, s2.p1);
	d[3] = direction(s1.p1, s1.p2, s2.p2);

	if (((d[0] > 0 && d[1] < 0) || (d[0] < 0 && d[1] > 0)) && ((d[2] > 0 && d[3] < 0) || (d[2] < 0 && d[3] > 0)))
		return true;
	if (d[0] == 0 && on_segment(s2.p1, s2.p2, s1.p1))
		return true;
	if (d[1] == 0 && on_segment(s2.p1, s2.p2, s1.p2))
		return true;
	if (d[2] == 0 && on_segment(s1.p1, s1.p2, s2.p1))
		return true;
	if (d[3] == 0 && on_segment(s1.p1, s1.p2, s2.p2))
		return true;

	return false;
}

double direction(Point p1, Point p2, Point p3) {
	return (p3.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p3.y - p1.y);
}

bool on_segment(Point p1, Point p2, Point p3) {
	bool  res = min(p1.x, p2.x) <= p3.x && p3.x >= max(p1.x, p2.x);
	res = res && min(p1.y, p2.y) <= p3.y && p3.y >= max(p1.y, p2.y);
	return res;
}

bool intersection_naive_eff(int& s1, int& s2, std::vector<Segment> vec) {
	bool b = false;

	std::vector<S> res(vec.size() * 2);
	for (auto i = 0; i < vec.size(); i++) {
		res[2 * i].p = vec[i].p1;
		res[2 * i + 1].p = vec[i].p2;
		res[2 * i].g = i;
		res[2 * i + 1].g = i;
	}
	std::vector<int> idx(res.size());
	std::iota(idx.begin(), idx.end(), 0);
	std::sort(idx.begin(), idx.end(), [&res](const int& a, const int& b) {
		if (res[a].p.x == res[b].p.x) {
			return res[a].p.y < res[b].p.y;
		}
		return res[a].p.x < res[b].p.x;
	});

	nod* n1 = nullptr, * n2 = nullptr;
	Atree  t;
	for (auto i : idx) {
		t.print_tree();
		if (res[i].p.x == min(vec[res[i].g].p1.x, vec[res[i].g].p2.x)) {
			t.insert(vec[res[i].g]);
			n1 = t.above(vec[res[i].g], 1);
			n2 = t.below(vec[res[i].g], 1);
			if (n1 != nullptr)
				if (intersection(n1->key, vec[res[i].g])) {
					s1 = res[i].g;
					s2 = n1->key.g;
					cout << " 1";
					return true;

				}
			if (n2 != nullptr)
				if (intersection(n2->key, vec[res[i].g])) {
					s1 = res[i].g;
					s2 = n2->key.g;
					cout << " 2";
					return true;
				}
		}	else {
			n1 = t.above(vec[res[i].g], 2);
			n2 = t.below(vec[res[i].g], 2);
			if (n1 != nullptr && n2 != nullptr)
				if (intersection(n2->key, n1->key)) {
					s2 = n2->key.g;
					s1 = n1->key.g;
					cout << " 3";
					return true;
				}
			t.del(vec[res[i].g]);
		}
	}
	return b;
}

template<class T>
void print_res(std::vector<Segment> vec, T f) {
	int s1, s2;
	double t1 = clock();
	if (f(s1, s2, vec)) {
		cout << " segment intersection" << endl;
		//cout << " segment 1 = " << s1 << endl;
		//cout << " point 1: x = " << vec[s1].p1.x << " y = " << vec[s1].p1.y << endl;
		//cout << " point 2: x = " << vec[s1].p2.x << " y = " << vec[s1].p2.y << endl;
		//cout << " segment 2 = " << s2 << endl;
		//cout << " point 1: x = " << vec[s2].p1.x << " y = " << vec[s2].p1.y << endl;
		//cout << " point 2: x = " << vec[s2].p2.x << " y = " << vec[s2].p2.y << endl;
	}	else {
		//cout << "segment not intersection" << endl;
	}
	t1 = clock() - t1;
	cout << " time = " << t1 / 1000 << endl;
	cout << endl;
}
