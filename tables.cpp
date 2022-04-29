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
//ListNode struct for chained hash table
//Unlikely that we'll need prev
struct ListNode{
	unsigned value;
	ListNode* prev;
	ListNode* next;
};

//bool division: if true - use division algorithm, if false, multiplication
unsigned hash_index(bool division, unsigned m, double A, unsigned key, unsigned i){
	if(division){
		return (key+i) % m;
	}else{
		return floor((double)m * ((double)(key+i) * A) - floor((double)(key+i) * A));
	}
}

