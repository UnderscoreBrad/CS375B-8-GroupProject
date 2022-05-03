#include "chaining.hpp"

using namespace std;

ChainingTable::ChainingTable(int s)
{
	slots = s;
	table = new list<int>[s];
}

void ChainingTable::insert(vector<int> numbers)
{
	for(int i : numbers)
		insert(i);
}

void ChainingTable::insert(int num)
{
	int i = hash(num);

	// Count a collision if this list has an entry already.
	if (table[i].size() > 0)
		collis++;

	// Insert at head of list
	table[i].push_front(num);
}

void ChainingTable::remove(int key)
{
	int i = hash(key);
		
	// WARNING: Will remove ALL occurances of key
	// Does not matter if keys are unique
	table[i].remove(key);
}

float ChainingTable::load_factor()
{
	int total_keys = 0;

	for (int i = 0; i < slots; i++) {
		total_keys += table[i].size();
	}

	return (float) total_keys / slots;
}

int ChainingTable::get_collisions()
{
	return collis;
}
