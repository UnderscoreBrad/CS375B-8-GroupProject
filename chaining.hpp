#pragma once

#include <list>
#include <vector>

// A hash table that uses chaining
class ChainingTable {
private:
	int slots;
	int collis = 0;

	// std::list is a built-in linked list implementation
	std::list<int> *table;

	int hash(int key) {
		return key % slots;
	}

public:
	ChainingTable(int slots);

	void insert(std::vector<int> numbers);
	
	void insert(int num);
	
	void remove(int key);

	// Calculates the load factor
	float load_factor();

	int get_collisions();
};
