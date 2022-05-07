#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <random>
#include <pthread.h>
#include <chrono>
#include <iomanip>

#include "tables.hpp"
#include "chaining.hpp"

#define SIZE_DEFAULT 100000
#define THREAD_DEFAULT 1
#define THREAD_MAX 16
#define FLAG_LINEAR 0
#define FLAG_QUADRATIC 1
#define FLAG_DOUBLE_HASH 2
#define FLAG_CHAINING 3

std::vector<int> input;
std::vector<int> delete_data;
std::vector<int> insert_data;
unsigned long input_per_thread;

unsigned input_size;
unsigned table_size;
unsigned m;
unsigned m1;
unsigned m2;
unsigned c1;
unsigned c2;
double A;
bool division;
int input_max;
unsigned chain_m;

// struct results {
// 	string
// };

void runtime(void *(func)(void *)) {
	auto start = std::chrono::system_clock::now();

	func(NULL);

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> duration = end - start;

	std::cout << " [" << duration.count() << "s]" << std::endl;
}

//create_input: creates a subset of the test array, subset is specified by arg
//parallelized to speed up generating large input sizes.
void *create_input(void* arg){
	unsigned long lower_bound = *(unsigned long*) arg;
	unsigned long upper_bound = lower_bound + input_per_thread;
	std::uniform_int_distribution<int> unif(0,input_max);
	std::default_random_engine random;
	random.seed(pthread_self());
	for(unsigned long i = lower_bound; i < upper_bound; i++){
		input[i] = unif(random);
		if(i%100 == 0){
			delete_data[i/100] = input[i];
		}
	}
	pthread_exit(EXIT_SUCCESS);
	return nullptr;
}

void generate_input_vector(unsigned long input_size, unsigned thread_count){
	pthread_t tids[16];
	pthread_attr_t attr;
	pthread_attr_init(&attr); 
	if(thread_count > THREAD_MAX) thread_count = THREAD_MAX;
		input_per_thread = input_size / thread_count;   //preset size of input vector
	unsigned long lower_bounds[16];
	for(unsigned i = 0; i < thread_count; i++){
		lower_bounds[i] = input_per_thread * i;
		pthread_create(&tids[i], &attr, create_input, (void*)(&lower_bounds[i]));
	}
	for(unsigned i = 0; i < thread_count; i++){
		pthread_join(tids[i], NULL);
	}
}

void generate_helper_arrays(unsigned long size){
	insert_data.resize(size);
	std::uniform_int_distribution<int> unif(0,input_max);
	std::default_random_engine random;
	random.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for(unsigned long i = 0; i < size; i++){
		insert_data[i] = unif(random);
	}
}

void* linear_probing_tests(void* arg){
	clear_table(FLAG_LINEAR);
	unsigned collisions = 0;
	collisions += linear_probing(division, &input, m, A, table_size);
	collisions += linear_delete(division, &delete_data, m, A);
	collisions += linear_probing(division, &insert_data, m, A, table_size); 
	collisions += linear_search(division, &insert_data, m, A);
    std::cout << "Linear Probing | size:" << input.size() << " | Collisions: " << collisions;
	//test 2: multiplication with m = size

	return arg;
}

//WARNING: Quadratic probing cannot be used for hash tables with 100% capacity (unless we pick the perfect constants)
void* quadratic_probing_tests(void* arg){
	clear_table(FLAG_QUADRATIC);
	unsigned collisions = 0;
	collisions += quadratic_probing(division, &input, m, A, table_size, c1, c2);
	collisions += quadratic_delete(division, &delete_data, m, A, c1, c2);
	collisions += quadratic_probing(division, &insert_data, m, A, table_size, c1, c2); 
	collisions += quadratic_search(division, &insert_data, m, A, c1, c2);
    std::cout << "Quadratic Probing | input size:" << input.size() << "  | Collisions: " << collisions;
	return arg;
}

//WARNING: Double cannot be used for hash tables with 100% capacity (unless we pick the perfect constants)
void* double_hashing_tests(void* arg){
	clear_table(FLAG_DOUBLE_HASH);
	unsigned collisions = 0;
	collisions += double_hashing(division, division, &input, m, m1, m2, A, table_size);
	collisions += double_hashing_delete(division, division, &delete_data, m, m1, m2, A);
	collisions += double_hashing(division, division, &insert_data, m, m1, m2, A, table_size); 
	collisions += double_hashing_search(division, division, &insert_data, m, m1, m2, A);
    std::cout << "Double Hashing | input size:" << input.size() << " | Collisions: " << collisions;
	return arg;
}

// Runs test on chained hash table and prints out results
void *chaining_test(void *arg){

	int table_size = input.size();

	// Division method test
	ChainingTable ct(chain_m, division, A); 
	ct.insert(input);
	for(int i = 0; i < delete_data.size(); i++){
		ct.remove(delete_data[i]);
	}
	ct.insert(insert_data);
	for(int i = 0; i < insert_data.size(); i++){
		ct.search(insert_data[i]);
	}

	std::cout << "Chained Hash Table | "
	          << "input size: " << table_size << " | "
	          << "Collisions: " << ct.get_collisions() << " | "
	          << "Load factor: " << ct.load_factor();
	return 0;
}

/*void *chaining_test_multiplication(void *arg){
	int table_size = input.size();
	ChainingTable ct(table_size, false, .618); 
	ct.insert(input);

	std::cout << "Chained Hash Table | "
	          << "Size: " << table_size << " | "
	          << "Multiplication hashing | "
	          << "m = " << input.size() << " | A = .618 | "
	          << "Load factor: " << ct.load_factor();
	return 0;
}*/

void getConstraints(int constraint_line){
	if(constraint_line <= 0){
		std::cout << "invalid constraint argument" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::fstream file;
	file.open("constraints.txt",std::ios::in);
	int i = 1; // Need to count from 1 to line up with constraints.txt
	std::string constraints;
	if(file.is_open()){
		std::string temp;
		while(i <= constraint_line){
			getline(file,temp);
			
			//std::cout << "temp: " << temp << std::endl;

			// This line is a comment, do not run any tests.
			if (i == constraint_line) {
				if (temp[0] == '#' || temp.empty()) {
					exit(0);
				}

				constraints = temp;
				file.close();
				break;
			}
			i++;
		}
		file.close();
	}
	std::vector<std::string> cons;
	std::stringstream sstream(constraints);
    std::string word;
    while (getline(sstream, word, ' ')){
        word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
        cons.push_back(word);
    }
	input_size = stol(cons[0]);
	table_size = stol(cons[1]);
	m = stol(cons[2]);
	m1 = stol(cons[3]);
	m2 = stol(cons[4]);
	A = stod(cons[5])/100000000000.0;
	c1 = stol(cons[6]);
	c2 = stol(cons[7]);
	chain_m = stol(cons[8]);
	division = cons[9] == "div";
	input_max = stoi(cons[10]);
	std::cout 	<< std::endl;
	std::cout 	<< "Constraints: | input size:" << input_size << " | input range: 0-" << input_max << std::endl;
	std::cout 	<< "m=" << m << " | m1=" << m1 << " | m2=" << m2 << " | A=" << A << " | c1=" << c1 
				<< " | c2=" << c2 << " | chain_m=" << chain_m << " | division=" << division << std::endl;
}

void setup(int argc, char *argv[]) {
	unsigned constraints_line = 1;
	input_size = SIZE_DEFAULT;
	unsigned thread_count = THREAD_DEFAULT;
	bool unique = false;
	if (argc > 1) constraints_line = std::stol(argv[1]);
	if (argc > 2) thread_count = std::stoi(argv[2]);
	if (argc > 3) unique = (strcmp(argv[3], "unique") == 0);
	getConstraints(constraints_line);
	unsigned long sz = input_size / 100;
	input.resize(input_size);   
	delete_data.resize(sz);
	generate_input_vector(input_size, thread_count);
	generate_helper_arrays(sz);

	// Make sure input numbers is unique
	if (unique)
		remove_duplicates(input);
	
}


// Usage: ./hashing_test <input size> <thread count> "unique"
//argv[1] == input size (unsigned long)
//if not specified, use default
//argv[2] == thread count
//if not specified, use default
int main(int argc, char* argv[]){
	setup(argc, argv);
	//At this point, the vector input holds everything we need to insert into our tables.

	//When testing, due to structure, we cannot test two of the same type of table simultaneously.
	//For instance, we can test linear and quadratic probing simultaneously on separate threads,
	//but not two linear probing tests simultaneously.
	//mostly because i was lazy about how i set up the tests.

	//these tests are just to test functionality, they aren't fully implemented yet.
	runtime(linear_probing_tests);
	runtime(quadratic_probing_tests);
	runtime(double_hashing_tests);
	runtime(chaining_test);
	//runtime(chaining_test_multiplication);

	return EXIT_SUCCESS;
}
