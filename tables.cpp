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
	int key = -1;
	ListNode* prev;
	ListNode* next;
};

std::vector<int> linear_probing_table;
std::vector<int> quadratic_probing_table;
std::vector<int> double_hashing_table;
std::vector<ListNode> chaining_table;

//bool division: if true - use division algorithm, if false, multiplication
unsigned long hash_index(bool division, unsigned m, double A, int key, int i){
	if(division){
		return (key+i) % m;
	}else{
		return floor((double)m * ((double)(key+i) * A) - floor((double)(key+i) * A));
	}
}

void clear_tables(){
	linear_probing_table.clear();
	quadratic_probing_table.clear();
	double_hashing_table.clear();
	chaining_table.clear();
}

unsigned linear_delete(bool division, std::vector<int>* elements, unsigned m, double A){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		int i = 0;
		int key = (*elements)[x];
		do{
			placement = hash_index(division, m, A, key, i);
			i++;
		}while(linear_probing_table[placement] != key);
		collisions += i-1;
		linear_probing_table[placement] = DELETED;
	}
	return collisions;
}

unsigned linear_probing(bool division, std::vector<int>* input, unsigned m, double A, unsigned long size){
	unsigned collisions = 0;
	linear_probing_table.resize(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		int i = 0;
		int key = (*input)[x];
		do{
			placement = hash_index(division, m, A, key, i);
			i++;
		}while(linear_probing_table[placement] != NIL && linear_probing_table[placement] != DELETED);
		collisions += i-1;
		linear_probing_table[placement] = key;
	}
	return collisions;
}

unsigned quadratic_delete(bool division, std::vector<int>* elements, unsigned m, double A, unsigned c1, unsigned c2){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		int i = 0;
		int key = (*elements)[x];
		do{
			int temp = c1 * i + c2 * (i * i);
			placement = hash_index(division, m, A, key, temp);
			i++;
		}while(quadratic_probing_table[placement] != key);
		collisions += i-1;
		quadratic_probing_table[placement] = DELETED;
	}
	return collisions;
}

unsigned quadratic_probing(bool division, std::vector<int>* input, unsigned m, double A, unsigned long size, unsigned c1, unsigned c2){
	unsigned collisions = 0;
	quadratic_probing_table.resize(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		int i = 0;
		int key = (*input)[x];
		do{
			int temp = c1 * i + c2 * (i * i);
			placement = hash_index(division, m, A, key, temp);
			i++;
		}while(quadratic_probing_table[placement] != NIL && quadratic_probing_table[placement] != DELETED);
		collisions += i-1;
		quadratic_probing_table[placement] = key;
	}
	return collisions;
}

unsigned double_hashing_delete(bool division1, bool division2, std::vector<int>* elements, unsigned m, unsigned m1, unsigned m2, double A){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		int i = 0;
		int key = (*elements)[x];
		do{
			placement = (hash_index(division1, m1, A, key, 0) + i*hash_index(division2, m2, A, key, 0))%m;
			i++;
		}while(double_hashing_table[placement] != key);
		collisions += i-1;
		double_hashing_table[placement] = DELETED;
	}
	return collisions;
}

unsigned double_hashing(bool division1, bool division2, std::vector<int>* input, unsigned m, unsigned m1, unsigned m2, double A, unsigned long size){
	unsigned collisions = 0;
	double_hashing_table.resize(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		int i = 0;
		int key = (*input)[x];
		do{
			placement = (hash_index(division1, m1, A, key, 0) + i*hash_index(division2, m2, A, key, 0))%m;
			i++;
		}while(double_hashing_table[placement] != NIL && double_hashing_table[placement] != DELETED);
		collisions += i-1;
		double_hashing_table[placement] = key;
	}
	return collisions;
}