
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

// Node constructor, given.
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

// twoDtree destructor, given.
twoDtree::~twoDtree(){
	clear();
}

// twoDtree copy constructor, given.
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

// twoDtree assignment operator, given.
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){
	stats* s = new stats(imIn);
	width = imIn.width();
	height = imIn.height();
	root = buildTree(*s, pair<int, int>(0, 0), pair<int, int>(width-1, height-1), true);
	delete s;
}

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
	Node* node = new Node(ul, lr, s.getAvg(ul, lr));
	if (ul.first != lr.first || ul.second != lr.second) {
		if (vert) {
			if (ul.first != lr.first) {
				int sp = findVSplitPoint(s, ul, lr);
				node->left = buildTree(s, ul, pair<int, int>(sp, lr.second), false);
				node->right = buildTree(s, pair<int, int>(sp+1, ul.second), lr, false);
			} else {
				int sp = findHSplitPoint(s, ul, lr);
				node->left = buildTree(s, ul, pair<int, int>(lr.first, sp), false);
				node->right = buildTree(s, pair<int, int>(ul.first, sp+1), lr, false);
			}
		} else {
			if (ul.second != lr.second) {
				int sp = findHSplitPoint(s, ul, lr);
				node->left = buildTree(s, ul, pair<int, int>(lr.first, sp), true);
				node->right = buildTree(s, pair<int, int>(ul.first, sp+1), lr, true);
			} else {
				int sp = findVSplitPoint(s, ul, lr);
				node->left = buildTree(s, ul, pair<int, int>(sp, lr.second), true);
				node->right = buildTree(s, pair<int, int>(sp+1, ul.second), lr, true);
			}
		}
	}
	return node;
}

int twoDtree::findVSplitPoint(stats & s, pair<int, int> & ul, pair<int, int> & lr) {
	int sp = ul.first;
	long minVar = s.getScore(ul, pair<int, int>(sp, lr.second)) + s.getScore(pair<int, int>(sp+1, ul.second), lr);
	for (int i = sp+1; i < lr.first; i++) {
		long newVar = s.getScore(ul, pair<int, int>(i, lr.second)) + s.getScore(pair<int, int>(i+1, ul.second), lr);
		if (newVar <= minVar) {
			sp = i;
			minVar = newVar;
		}
	}
	return sp;
}

int twoDtree::findHSplitPoint(stats & s, pair<int, int> & ul, pair<int, int> & lr) {
	int sp = ul.second;
	int minVar = s.getScore(ul, pair<int, int>(lr.first, sp)) + s.getScore(pair<int, int>(ul.first, sp+1), lr);
	for (int i = sp+1; i < lr.second; i++) {
		int newVar = s.getScore(ul, pair<int, int>(lr.first, i)) + s.getScore(pair<int, int>(ul.first, i+1), lr);
		if (newVar <= minVar) {
			sp = i;
			minVar = newVar;
		}
	}
	return sp;
}

PNG twoDtree::render(){
	PNG im(width, height);
	render(root, im);
	return im;
}

void twoDtree::render(Node* subRoot, PNG &im) {
	if (subRoot) {
		if (!subRoot->left && !subRoot->right) {
			for (int y = subRoot->upLeft.second; y <= subRoot->lowRight.second; y++) {
				for (int x = subRoot->upLeft.first; x <= subRoot->lowRight.first; x++) {
					*im.getPixel(x, y) = subRoot->avg;
				}
			}
		}
		if (subRoot->left) {
			render(subRoot->left, im);
		}
		if (subRoot->right) {
			render(subRoot->right, im);
		}
	}
}

int twoDtree::idealPrune(int leaves) {
	int tolLower = 0;
	int tolUpper = 1;
	int size = pruneSize(tolUpper);
	while (size > leaves) {
		tolLower = tolUpper;
		tolUpper *= 2;
		size = pruneSize(tolUpper);
	}

	size = pruneSize(tolLower);
	int tol = 0;
	while (tolLower < tolUpper) {
		std::cout << tol << '\n';
		tol = (tolLower + tolUpper) / 2;
		size = pruneSize(tol);
		if (size <= leaves) {
			tolUpper = tol;
		} else {
			tol++;
			tolLower = tol;
		}
	}

	size = pruneSize(tol);
	while (size == pruneSize(tol-1)) {
		tol--;
		size = pruneSize(tol-1);
	}

 	return tol;
}

int twoDtree::pruneSize(int tol) {
	return pruneSize(root, tol);
}

int twoDtree::pruneSize(Node* & subRoot, int tol) {
	if (subRoot == NULL) {
		return 0;
	} else {
		if (canPrune(subRoot, tol, subRoot->avg)) {
			return 1;
		} else {
			return pruneSize(subRoot->left, tol) + pruneSize(subRoot->right, tol);
		}
	}
}

void twoDtree::prune(int tol) {
	prune(root, tol);
}

void twoDtree::prune(Node* & subRoot, int tol) {
	if (subRoot) {
		if (canPrune(subRoot, tol, subRoot->avg)) {
			clear(subRoot->left);
			clear(subRoot->right);
		} else {
			if (subRoot->left) {
				prune(subRoot->left, tol);
			}
			if (subRoot->right) {
				prune(subRoot->right, tol);
			}
		}
	}
}

bool twoDtree::canPrune(Node* & subRoot, int tol, RGBAPixel avg) {
	bool ret = true;
	if (subRoot) {
		if (!subRoot->left && !subRoot->right) {
			ret = ret && (subRoot->avg.r - avg.r) * (subRoot->avg.r - avg.r) + (subRoot->avg.g - avg.g) * (subRoot->avg.g - avg.g) + (subRoot->avg.b - avg.b) * (subRoot->avg.b - avg.b) <= tol;
		}
		if (subRoot->left) {
			ret = ret && canPrune(subRoot->left, tol, avg);
		}
		if (subRoot->right) {
			ret = ret && canPrune(subRoot->right, tol, avg);
		}
	}
	return ret;
}


void twoDtree::clear() {
	width = 0;
	height = 0;
	clear(root);
}

void twoDtree::clear(Node*& subRoot) {
	if (subRoot) {
		clear(subRoot->left);
		clear(subRoot->right);
		delete subRoot;
		subRoot = NULL;
	}
}

void twoDtree::copy(const twoDtree & other) {
	width = other.width;
	height = other.height;
	root = copy(other.root);
}

twoDtree::Node* twoDtree::copy(const Node* subRoot) const {
	Node* node = NULL;
	if (subRoot) {
		node = new Node(subRoot->upLeft, subRoot->lowRight, subRoot->avg);
		node->left = copy(subRoot->left);
		node->right = copy(subRoot->right);
	}
	return node;
}
