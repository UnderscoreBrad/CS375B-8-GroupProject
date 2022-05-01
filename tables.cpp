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

#define NIL -1
#define DELETED -2

//ListNode struct for chained hash table
//Unlikely that we'll need prev
struct ListNode{
	int value;
	ListNode* prev;
	ListNode* next;
};

//bool division: if true - use division algorithm, if false, multiplication
unsigned long hash_index(bool division, unsigned m, double A, int key, int i){
	//in testing, key should never be negative. negative values will be reserved for NIL and DELETED
	if(division){
		return (key+i) % m;
	}else{
		return floor((double)m * ((double)(key+i) * A) - floor((double)(key+i) * A));
	}
}

unsigned linear_probing(bool division, std::vector<int>* input, unsigned m, double A, unsigned long size){
	unsigned collisions = 0;
	std::vector<int> hash_table(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		int i = 0;
		int key = (*input)[x];
		//std::cout << key << std::endl;
		do{
			placement = hash_index(division, m, A, key, i);
			i++;
		}while(hash_table[placement] != NIL && hash_table[placement] != DELETED);
		collisions += i;
		hash_table[placement] = key;
	}
	return collisions;
}

unsigned quadratic_probing(bool division, std::vector<int>* input, unsigned m, double A, unsigned long size, unsigned c1, unsigned c2){
	unsigned collisions = 0;
	std::vector<int> hash_table(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		int i = 0;
		int key = (*input)[x];
		//std::cout << key << std::endl;
		do{
			int temp = c1 * i + c2 * (i * i);
			placement = hash_index(division, m, A, key, temp);
			i++;
		}while(hash_table[placement] != NIL && hash_table[placement] != DELETED);
		collisions += i;
		hash_table[placement] = key;
	}
	return collisions;
}

unsigned double_hashing(bool division1, bool division2, std::vector<int>* input, unsigned m, unsigned m1, unsigned m2, double A, unsigned long size){
	unsigned collisions = 0;
	std::vector<int> hash_table(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		int i = 0;
		int key = (*input)[x];
		//std::cout << key << std::endl;
		do{
			placement = (hash_index(division1, m1, A, key, 0) + i*hash_index(division2, m2, A, key, 0))%m;
			i++;
		}while(hash_table[placement] != NIL && hash_table[placement] != DELETED);
		collisions += i;
		hash_table[placement] = key;
	}
	return collisions;
}