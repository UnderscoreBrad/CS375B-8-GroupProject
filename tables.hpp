#pragma once

#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <random>
#include <pthread.h>
#include <chrono>
#include <algorithm>

#define NIL -1
#define DELETED -2

std::vector<int> linear_probing_table;
std::vector<int> quadratic_probing_table;
std::vector<int> double_hashing_table;

void remove_duplicates(std::vector<int> & vec) {
	sort( vec.begin(), vec.end() );
	vec.erase( unique( vec.begin(), vec.end() ), vec.end() );
}

void clear_table(int arg){
	if(arg == 0){
		linear_probing_table.clear();
	}else if(arg == 1){
		quadratic_probing_table.clear();
	}else if(arg == 2){
		double_hashing_table.clear();
	}
}

//bool division: if true - use division algorithm, if false, multiplication
unsigned long hash_index(bool division, unsigned m, double A, int key, int i){
	if(division){
		return (key+i) % m;
	}else{
		return floor((float)m * ((float) (key+i)*A - floor((float) (key+i)*A)));
		//return floor((double)m * ((double)(key+i) * A) - floor((double)(key+i) * A));
	}
}

unsigned linear_search(bool division, const std::vector<int>* elements, unsigned m, double A){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		unsigned i = 0;
		int key = (*elements)[x];
		do{
			if(i == m){
				break;
			}
			placement = hash_index(division, m, A, key, i);
			i++;
		}while(linear_probing_table[placement] != key && linear_probing_table[placement] != NIL);
		collisions += i-1;
	}
	return collisions;
}

unsigned linear_delete(bool division, const std::vector<int>* elements, unsigned m, double A){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		unsigned i = 0;
		int key = (*elements)[x];
		//std::cout << x << " " << key << std::endl;
		do{
			if(i == m){
				break;
			}
			placement = hash_index(division, m, A, key, i);
			i++;
		}while(linear_probing_table[placement] != key && linear_probing_table[placement] != NIL);
		collisions += i-1;
		if(linear_probing_table[placement] == key) {
			linear_probing_table[placement] = DELETED;
			//std::cout << "found key " << key << std::endl;
		}
	}
	return collisions;
}

unsigned linear_probing(bool division, const std::vector<int>* input, unsigned m, double A, unsigned long size){
	unsigned collisions = 0;
	linear_probing_table.resize(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		unsigned i = 0;
		int key = (*input)[x];
		do{
			if(i == m){
				std::cout << "ERROR: Hash Table Overflow: " << m << std::endl;
				return collisions;
			}
			placement = hash_index(division, m, A, key, i);
			i++;
		}while(linear_probing_table[placement] != NIL && linear_probing_table[placement] != DELETED);
		collisions += i-1;
		linear_probing_table[placement] = key;
	}
	return collisions;
}

unsigned quadratic_search(bool division, const std::vector<int>* elements, unsigned m, double A, unsigned c1, unsigned c2){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		unsigned i = 0;
		int key = (*elements)[x];
		do{
			if(i == m){
				break;//value not found
			}
			int temp = c1 * i + c2 * (i * i);
			placement = hash_index(division, m, A, key, temp);
			i++;
		}while(quadratic_probing_table[placement] != key && quadratic_probing_table[placement] != NIL);
		collisions += i-1;
	}
	return collisions;
}

unsigned quadratic_delete(bool division, const std::vector<int>* elements, unsigned m, double A, unsigned c1, unsigned c2){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		unsigned i = 0;
		int key = (*elements)[x];
		do{
			if(i == m){
				break;//value not found
			}
			int temp = c1 * i + c2 * (i * i);
			placement = hash_index(division, m, A, key, temp);
			i++;
		}while(quadratic_probing_table[placement] != key && quadratic_probing_table[placement] != NIL);
		collisions += i-1;
		if(quadratic_probing_table[placement] == key) quadratic_probing_table[placement] = DELETED;
	}
	return collisions;
}

unsigned quadratic_probing(bool division, const std::vector<int>* input, unsigned m, double A, unsigned long size, unsigned c1, unsigned c2){
	unsigned collisions = 0;
	quadratic_probing_table.resize(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		unsigned i = 0;
		int key = (*input)[x];
		do{
			if(i == m){
				std::cout << "ERROR: Hash Table Overflow" << std::endl;
				return collisions;
			}
			int temp = c1 * i + c2 * (i * i);
			placement = hash_index(division, m, A, key, temp);
			i++;
		}while(quadratic_probing_table[placement] != NIL && quadratic_probing_table[placement] != DELETED);
		collisions += i-1;
		quadratic_probing_table[placement] = key;
	}
	return collisions;
}

unsigned double_hashing_search(bool division1, bool division2, const std::vector<int>* elements, unsigned m, unsigned m1, unsigned m2, double A){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		unsigned i = 0;
		int key = (*elements)[x];
		do{
			if(i == m){
				break;
			}
			placement = (hash_index(division1, m1, A, key, 0) + i*hash_index(division2, m2, A, key, 0))%m;
			i++;
		}while(double_hashing_table[placement] != key && double_hashing_table[placement] != NIL);
		collisions += i-1;
	}
	return collisions;
}

unsigned double_hashing_delete(bool division1, bool division2, const std::vector<int>* elements, unsigned m, unsigned m1, unsigned m2, double A){
	unsigned collisions = 0;
	unsigned long placement = 0;
	for(unsigned x = 0; x < elements->size(); x++){
		unsigned i = 0;
		int key = (*elements)[x];
		do{
			if(i == m){
				break;
			}
			placement = (hash_index(division1, m1, A, key, 0) + i*hash_index(division2, m2, A, key, 0))%m;
			i++;
		}while(double_hashing_table[placement] != key && double_hashing_table[placement] != NIL);
		collisions += i-1;
		if(double_hashing_table[placement] == key) double_hashing_table[placement] = DELETED;
	}
	return collisions;
}

unsigned double_hashing(bool division1, bool division2, const std::vector<int>* input, unsigned m, unsigned m1, unsigned m2, double A, unsigned long size){
	unsigned collisions = 0;
	double_hashing_table.resize(size,NIL);
	unsigned long placement = 0;
	for(unsigned x = 0; x < input->size(); x++){
		unsigned i = 0;
		int key = (*input)[x];
		do{
			if(i == m){
				std::cout << "ERROR: Hash Table Overflow" << std::endl;
				return collisions;
			}
			placement = ((hash_index(division1, m1, A, key, 0)) + i*(hash_index(division2, m2, A, key, i)))%m;
			i++;
		}while(double_hashing_table[placement] != NIL && double_hashing_table[placement] != DELETED);
		collisions += i-1;
		double_hashing_table[placement] = key;
	}
	return collisions;
}
