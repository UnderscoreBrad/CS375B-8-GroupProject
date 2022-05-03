#ifndef DECLARED
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <random>
#include <pthread.h>
#include <chrono>
#define DECLARED
#endif

#include "tables.cpp"

#define SIZE_DEFAULT 100000
#define THREAD_DEFAULT 1
#define THREAD_MAX 16
#define INPUT_MIN 0
#define INPUT_MAX 10000
#define FLAG_LINEAR 0
#define FLAG_QUADRATIC 1
#define FLAG_DOUBLE_HASH 2
#define FLAG_CHAINING 3

std::vector<int> input;
std::vector<int> delete_data;
std::vector<int> insert_data;
unsigned long input_per_thread;

//create_input: creates a subset of the test array, subset is specified by arg
//parallelized to speed up generating large input sizes.
void *create_input(void* arg){
	unsigned long lower_bound = *(unsigned long*) arg;
	unsigned long upper_bound = lower_bound + input_per_thread;
	std::uniform_int_distribution<int> unif(INPUT_MIN,INPUT_MAX);
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
	std::uniform_int_distribution<int> unif(INPUT_MIN,INPUT_MAX);
	std::default_random_engine random;
	random.seed(std::chrono::system_clock::now().time_since_epoch().count());
	for(unsigned long i = 0; i < size; i++){
		insert_data[i] = unif(random);
	}
}

void* linear_probing_tests(void* arg){
	clear_table(FLAG_LINEAR);
	//test 1: division with m = size, full table
	auto start = std::chrono::system_clock::now();
	unsigned collisions = 0;
	unsigned m = input.size();
	double A = 0.0;
	collisions += linear_probing(true, &input, m, A, input.size());
	collisions += linear_delete(true, &delete_data, m, A);
	collisions += linear_probing(true, &insert_data, m, A, input.size()); 
	collisions += linear_search(true, &insert_data, m, A);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Linear Probing | size:" << input.size() << " | division | m = size | Completion Time: " << duration.count() << "s | Collisions: " << collisions << std::endl;
	//test 2: multiplication with m = size

	return arg;
}

//WARNING: Quadratic probing cannot be used for hash tables with 100% capacity (unless we pick the perfect constants)
void* quadratic_probing_tests(void* arg){
	clear_table(FLAG_QUADRATIC);
	//test 1: division with m = size, full table
	auto start = std::chrono::system_clock::now();
	unsigned collisions = 0;
	unsigned m = input.size();
	double A = 0.0;
	unsigned c1 = 1;
	unsigned c2 = 2;
	collisions += quadratic_probing(true, &input, m, A, input.size(), c1, c2);
	collisions += quadratic_delete(true, &delete_data, m, A, c1, c2);
	collisions += quadratic_probing(true, &insert_data, m, A, input.size(), c1, c2); 
	//collisions += quadratic_search(true, &insert_data, m, A, c1, c2);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Quadratic Probing | size:" << input.size() << " | division | m = size | Completion Time: " << duration.count() << "s | Collisions: " << collisions << std::endl;
	//test 2: multiplication with m = ?, A = ?
	return arg;
}

//WARNING: Double cannot be used for hash tables with 100% capacity (unless we pick the perfect constants)
void* double_hashing_tests(void* arg){
	clear_table(FLAG_DOUBLE_HASH);
	auto start = std::chrono::system_clock::now();
	unsigned collisions = 0;
	unsigned m = input.size()+30000;
	double A = 0.0;
	unsigned m1 = input.size()+30000;//97861;
	unsigned m2 = input.size()+30000;//99409;
	collisions += double_hashing(true, true, &input, m, m1, m2, A, input.size()+30000);
	collisions += double_hashing_delete(true, true, &delete_data, m, m1, m2, A);
	collisions += double_hashing(true, true, &insert_data, m, m1, m2, A, input.size()+30000); 
	collisions += double_hashing_search(true, true, &insert_data, m, m1, m2, A);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Double Hashing | size:" << input.size() << " | division | m = size | Completion Time: " << duration.count() << "s | Collisions: " << collisions << std::endl;
	return arg;
}

void* chaining_tests(void* arg){
	clear_table(FLAG_CHAINING);

	return arg;
}

//argv[1] == input size (unsigned long)
//if not specified, use default
//argv[2] == thread count
//if not specified, use default
int main(int argc, char* argv[]){
	unsigned long input_size = SIZE_DEFAULT;
	unsigned thread_count = THREAD_DEFAULT;
	if(argc > 1){
		input_size = std::stol(argv[1]);
		if(argc > 2) thread_count = std::stoi(argv[2]);
	}
	unsigned long sz = input_size / 100;
	input.resize(input_size);   
	delete_data.resize(sz);
	generate_input_vector(input_size, thread_count);
	generate_helper_arrays(sz);
	//At this point, the vector input holds everything we need to insert into our tables.

	//When testing, due to structure, we cannot test two of the same type of table simultaneously.
	//For instance, we can test linear and quadratic probing simultaneously on separate threads,
	//but not two linear probing tests simultaneously.
	//mostly because i was lazy about how i set up the tests.

	//these tests are just to test functionality, they aren't fully implemented yet.
	//linear_probing_tests(nullptr);
	quadratic_probing_tests(nullptr);
	//double_hashing_tests(nullptr);

	return EXIT_SUCCESS;
}