#include <list>
#include <vector>

using namespace std;

class ChainingTable {
private:
	int slots;
	int collisions = 0;

	// std::list is a built-in linked list implementation
	list<int> *table;

	int hash(int key) {
		return key % slots;
	}

public:
	ChainingTable(int s) {
		slots = s;
		table = new list<int>[s];
	}

	void insert(std::vector<int> numbers) {
		for(int i : numbers)
			insert(i);
	}

	void insert(int num) {
		int i = hash(num);

		// Count a collision if this list has an entry already.
		if (table[i].size() > 0)
			collisions++;

		// Insert at head of list
		table[i].push_front(num);
	}

	void remove(int key) {
		int i = hash(key);
		
		// WARNING: Will remove ALL occurances of key
		// Does not matter if keys are unique
		table[i].remove(key);
	}

	// Calculates the load factor
	float load_factor() {
		int total_keys = 0;

		for (int i = 0; i < slots; i++) {
			total_keys += table[i].size();
		}

		return total_keys / slots;
	}

	int get_collisions() {
		return collisions;
	}
};
