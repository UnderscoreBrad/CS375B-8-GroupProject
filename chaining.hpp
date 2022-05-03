#pragma once

#include <stdint.h>

#include <list>
#include <vector>
#include <cmath>
#include <iostream>

// A hash table that uses chaining
class ChainingTable {
private:
	int slots;
	int collis = 0;
	float A;
	bool division;

	// std::list is a built-in linked list implementation
	std::list<int> *table;

public:
	ChainingTable(int slots, bool division, float a);

	void insert(std::vector<int> numbers);
	
	void insert(int num);
	
	void remove(int key);

	// Calculates the load factor
	float load_factor();

	int get_collisions();
};
