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

std::vector<int> input;
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
	input.resize(input_size);   
	generate_input_vector(input_size, thread_count);
	//At this point, the vector input holds everything we need to insert into our tables.

	//These lines are designed to be WORST CASE to make sure our functions work.
	std::cout << linear_probing(true, &input, input.size(), 0, input.size()) << std::endl;
	std::cout << quadratic_probing(true, &input, input.size(), 0, input.size(), 1, 1) << std::endl;
	std::cout << double_hashing(true, true, &input, input.size(), 14178, 12413, 0, input.size()) << std::endl;
	return EXIT_SUCCESS;
}