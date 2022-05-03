#include "chaining.hpp"

using namespace std;

//bool division: if true - use division algorithm, if false, multiplication
static uint64_t hash_index(bool division, unsigned m, double A, int key){
	if(division){
		return (key) % m;
	}else{
		//uint64_t hash = floor((double) m * ((double) key * A - floor((double)key * A));
		uint64_t hash = floor(  m * (  (float) key*A - floor((float) key*A)  )  );
		//cout << "multiplication hash = " << hash << endl;
		return hash;
	}
}

ChainingTable::ChainingTable(int _slots, bool _division, float _a)
{
	slots = _slots;
	table = new list<int>[_slots];
	A = _a;
	division = _division;
}

void ChainingTable::insert(vector<int> numbers)
{
	for(int i : numbers)
		insert(i);
}

void ChainingTable::insert(int key)
{
	int i = hash_index(division, slots, A, key);

	// Count a collision if this list has an entry already.
	if (table[i].size() > 0)
		collis++;

	// Insert at head of list
	table[i].push_front(key);
}

void ChainingTable::remove(int key)
{
	int i = hash_index(division, slots, A, key);
		
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
