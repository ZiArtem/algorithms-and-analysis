#pragma once
#include <algorithm>
#include <iostream>

using namespace std;

struct Point {
	double x, y;
	bool operator ==(Point& p) {
		return this->x == p.x && this->y == p.y;
	}
};

double dir(Point p1, Point p2, Point p3) {
	return (p3.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p3.y - p1.y);
}

struct Segment {
	Point p1, p2;
	int g;
	bool less(Segment& s, int k) {
		Point a;
		double res = 0;
		if (k == 1) {
			if (s.p1.x == min(s.p1.x, s.p2.x))
				a = s.p1;
			else
				a = s.p2;
		}	else {
			if (s.p1.x == min(s.p1.x, s.p2.x))
				a = s.p2;
			else
				a = s.p1;
		}
		if (this->p1.x < this->p2.x)
			res = dir(a, this->p1, this->p2);
		else
			res = dir(a, this->p2, this->p1);
		return res < 0;
	}

	bool more(Segment& s, int k) {
		Point a;
		double res = 0;
		if (k == 1) {
			if (s.p1.x == min(s.p1.x, s.p2.x))
				a = s.p1;
			else
				a = s.p2;
		}	else {
			if (s.p1.x == min(s.p1.x, s.p2.x))
				a = s.p2;
			else
				a = s.p1;
		}
		if (this->p1.x < this->p2.x)
			res = dir(a, this->p1, this->p2);
		else
			res = dir(a, this->p2, this->p1);
		return res > 0;
	}

	bool operator == (Segment& s) {
		return this->p1 == s.p1 && this->p2 == s.p2;
	}

	bool operator != (Segment& s) {
		return !(this->p1 == s.p1 && this->p2 == s.p2);
	}
};

struct S {
	Point p;
	int g;
};

class nod {
public:
	Segment key;
	int balance;
	nod* left, * right, * parent;

	nod(Segment k, nod* p) : key(k), balance(0), parent(p),
		left(nullptr), right(nullptr) {}

	~nod() {
		if (left != nullptr)
			delete left;
		if (right != nullptr)
			delete right;
	}
};

class Atree {
public:
	nod* root;
	Atree();
	~Atree();
	bool insert(Segment& key);
	void print_val(nod* n);
	void print_tree();
	nod* above(Segment key, int f);
	nod* below(Segment key, int f);
	void del(Segment key);
	nod* rotateLeft(nod* a);
	nod* rotateRight(nod* a);
	nod* rotateLeftThenRight(nod* n);
	nod* rotateRightThenLeft(nod* n);
	void rebalance(nod* n);
	int height(nod* n);
	void setBalance(nod* n);
};

Atree::Atree() : root(nullptr) {}

Atree::~Atree() {
	delete root;
}

bool Atree::insert(Segment& key) {
	//cout << "start insert val = " << min(key.p1.y, key.p2.y) << endl;
	if (root == nullptr) {
		root = new nod(key, nullptr);
	}	else {
		nod* n, * par;
		par = n = root;
		while (n != nullptr) {
			if (n->key == key)
				return false;
			par = n;
			if (n->key.less(key, 1)) {
				n = n->right;
			}	else {
				n = n->left;
			}
		}
		if (par->key.less(key, 1)) {
			par->right = new nod(key, par);
		}	else {
			par->left = new nod(key, par);
		}
		rebalance(par);
	}
	return true;
}

void Atree::print_val(nod* n) {
	if (n != nullptr) {
		print_val(n->left);
		//cout << min(n->key.p1.y, n->key.p2.y) << ' ';
		print_val(n->right);
	}
}

void Atree::print_tree() {
	if (root != nullptr)
		//cout << "root = " << min(root->key.p1.y, root->key.p2.y) << " ";
	//cout << "all tree = ";
		print_val(root);
	//cout << endl;
}

nod* Atree::above(Segment key, int f) {
	//cout << "start above, key = " << min(key.p1.y, key.p2.y);
	nod* n = root;
	if (n == nullptr)
		return nullptr;
	while (!(n->key == key)) {
		if (n->key.less(key, f)) {
			n = n->right;
		}	else {
			n = n->left;
		}
		if (n == nullptr)
			return nullptr;
	}
	if (n->right != nullptr) {
		n = n->right;
		while (n->left != nullptr)
			n = n->left;
	}	else {
		if (n->parent == nullptr)
			return nullptr;
		if (n == n->parent->left) {
			n = n->parent;
		}	else {
			while (n != n->parent->left) {
				n = n->parent;
				if (n->parent == nullptr)
					return nullptr;
			}
			n = n->parent;
		}
	}
	//cout << "return key = " << min(n->key.p1.y, n->key.p2.y) << endl;
	return n;
}

nod* Atree::below(Segment key, int f) {
	//cout << "start below, key = " << min(key.p1.y, key.p2.y);
	nod* n = root;
	if (n == nullptr)
		return nullptr;
	while (!(n->key == key)) {
		if (n->key.less(key, f)) {
			n = n->right;
		}	else {
			n = n->left;
		}
		if (n == nullptr)
			return nullptr;
	}

	if (n->left != nullptr) {
		n = n->left;
		while (n->right != nullptr)
			n = n->right;
	}	else {
		if (n->parent == nullptr)
			return nullptr;
		if (n == n->parent->right) {
			n = n->parent;
		}	else {
			while (n != n->parent->right) {
				n = n->parent;
				if (n->parent == nullptr)
					return nullptr;
			}
			n = n->parent;
		}
	}
	//cout << "return key = " << min(n->key.p1.y, n->key.p2.y)<<endl;
	return n;
}

void Atree::del(Segment key) {
	//cout << "start del, key = " << min(key.p1.y, key.p2.y) << endl;
	if (root == nullptr)
		return;
	nod* n = root;
	while (n->key != key) {
		if (n->key.less(key, 1)) {
			n = n->right;
		}	else {
			n = n->left;
		}
		if (n == nullptr) {
			return;
		}
	}

	nod* child = n;
	nod* parent;
	//cout << "key naiden" << endl;
	if (n->left != nullptr && n->right != nullptr) {
		//cout << "1" << endl;
		child = child->right;
		while (child->left != nullptr) {
			child = child->left;
		}
		if (child != n->right) {
			child->parent->left = child->right;
			if (child->right != nullptr) {
				child->right->parent = child->parent;
			}
			child->right = n->right;
			child->right->parent = child;
		}
		child->left = n->left;
		child->left->parent = child;
		child->parent = n->parent;
		if (n->parent != nullptr) {
			if (n->parent->left == n) {
				n->parent->left = child;
			}	else {
				n->parent->right = child;
			}
		}
		if (root == n) {
			root = child;
		}
		if (child->parent != nullptr)
			rebalance(child->parent);
		else {
			if (child != nullptr)
				rebalance(child);
		}
	}	else {
		if (n->left != nullptr || n->right != nullptr) {
			//cout << "2" << endl;
			if (n->left != nullptr) {
				child = child->left;
			}	else {
				child = child->right;
			}
			child->parent = n->parent;

			if (n->parent != nullptr) {
				if (n->parent->left == n) {
					n->parent->left = child;
				}	else {
					n->parent->right = child;
				}
			}
		}
		if (root == n) {
			root = child;
		}
		if (child->parent != nullptr)
			rebalance(child->parent);
		else {
			if (child != nullptr)
				rebalance(child);
		}
	}

	if (n->left == nullptr && n->right == nullptr) {
		//cout << "3" << endl;
		if (n->parent != nullptr) {
			if (n->parent->left == n) {
				n->parent->left = nullptr;
			}	else {
				n->parent->right = nullptr;
			}
		}
		if (root == n) {
			root = nullptr;
		}	else {
			rebalance(n->parent);
		}
	}
	n->left = n->right = n->parent = nullptr;
	//delete n;
}

nod* Atree::rotateLeft(nod* a) {
	nod* b = a->right;
	b->parent = a->parent;
	a->right = b->left;

	if (a->right != NULL)
		a->right->parent = a;

	b->left = a;
	a->parent = b;

	if (b->parent != NULL) {
		if (b->parent->right == a) {
			b->parent->right = b;
		}	else {
			b->parent->left = b;
		}
	}
	setBalance(a);
	setBalance(b);
	return b;
}

nod* Atree::rotateRight(nod* a) {
	nod* b = a->left;
	b->parent = a->parent;
	a->left = b->right;

	if (a->left != NULL)
		a->left->parent = a;

	b->right = a;
	a->parent = b;

	if (b->parent != NULL) {
		if (b->parent->right == a) {
			b->parent->right = b;
		}	else {
			b->parent->left = b;
		}
	}
	setBalance(a);
	setBalance(b);
	return b;
}

nod* Atree::rotateLeftThenRight(nod* n) {
	n->left = rotateLeft(n->left);
	return rotateRight(n);
}

nod* Atree::rotateRightThenLeft(nod* n) {
	n->right = rotateRight(n->right);
	return rotateLeft(n);
}

void Atree::rebalance(nod* n) {
	setBalance(n);
	//cout << "in rebalance" << endl;

	if (n->balance == -2) {
		if (height(n->left->left) >= height(n->left->right)) {
			//cout << "left_right rotate" << endl;
			n = rotateRight(n);
		}	else {
			//cout << "left rotate" << endl;
			n = rotateLeftThenRight(n);
		}
	}	else if (n->balance == 2) {
		if (height(n->right->right) >= height(n->right->left)) {
			n = rotateLeft(n);
			//cout << "right_left rotate" << endl;
		}	else {
			//cout << "right rotate " << endl;
			n = rotateRightThenLeft(n);
		}
	}

	if (n->parent != NULL) {
		rebalance(n->parent);
	}	else {
		root = n;
	}
}

int Atree::height(nod* n) {
	if (n == nullptr)
		return -1;
	return 1 + max(height(n->left), height(n->right));
}

void Atree::setBalance(nod* n) {
	n->balance = height(n->right) - height(n->left);
}

