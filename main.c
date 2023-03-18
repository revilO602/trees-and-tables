//#define _CRT_SECURE_NO_DEPRECATE
//#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <HASHTABLE_vlastna.h>
#include <HASHTABLE_prebrata.h>
#include <RB_BST_vlastny.h>
#include <AVL_BST_prebraty.h>

float InsertTestRB(RBNODE** root,int *array, int length) {
	clock_t start, end;
	int i;

	start = clock();
	for (i = 0; i <length; i++) {
		insertRBTree(root, array[i]);
	}
	end = clock();
	return end - start;
}
float InsertTestAVL(AVLNODE** root, int* array, int length) {
	clock_t start, end;
	int i;

	start = clock();
	for (i = 0; i < length; i++) {
		*root=insertAVL(*root, array[i]);
	}
	end = clock();
	return end - start;
}

float InsertTestHTOwn(TABLE** table, int* array, int length) {
	clock_t start, end;
	int i;

	start = clock();
	*table = createTableOwn(5);
	for (i = 0; i < length; i++) {
		 insertHTableOwn(table, array[i]);
	}
	end = clock();
	return end - start;
}
float InsertTestHTOther(struct int_set** table, int* array, int length) {
	clock_t start, end;
	int i;

	start = clock();
	*table = int_set_create();
	for (i = 0; i < length; i++) {
		int_set_add(*table,array[i]);
	}
	end = clock();
	return end - start;
}
//search_cycles is the number of times the whole tree/hashtable will be searched
float SearchTestAVL(AVLNODE* root, int* array, int length, int search_cycles) {
	clock_t start, end;
	int i, j;

	start = clock();
	for (j = 0; j < search_cycles; j++) {
		for (i = 0; i < length; i++) {
			if(!(searchAVLTree(root, array[i])))
				printf("Error: Didnt find inserted key\n");
		}
	}
	end = clock();
	return end - start;
}
float SearchTestRB(RBNODE* root, int* array, int length, int search_cycles) {
	clock_t start, end;
	int i,j;

	start = clock();
	for (j = 0; j < search_cycles; j++) {
		for (i = 0; i < length; i++) {
			if (!(searchRBTree(root, array[i])))
				printf("Error: Didnt find inserted key\n"); 
		}
	}
	end = clock();
	return end - start;
}
float SearchTestHTOwn(TABLE* table, int* array, int length, int search_cycles) {
	clock_t start, end;
	int i, j;

	start = clock();
	for (j = 0; j < search_cycles; j++) {
		for (i = 0; i < length; i++) {
			if (!(searchHTOwn(table, array[i])))
				printf("Error: Didnt find inserted key\n"); 
		}
	}
	end = clock();
	return end - start;
}
float SearchTestHTOther(struct int_set* table, int* array, int length, int search_cycles) {
	clock_t start, end;
	int i, j;

	start = clock();
	for (j = 0; j < search_cycles; j++) {
		for (i = 0; i < length; i++) {
			if (int_set_search(table, array[i])==NULL)
				printf("Error: Didnt find inserted key\n"); 
		}
	}
	end = clock();
	return end - start;
}
// not an optimal rng, enough for our purposes
int my_rand() {
	long long n = rand();
	n = (n << 15) | rand();
	n = (n << 15) | rand();
	n = (n << 15) | rand();
	return n % INT_MAX;
}

//create numbers array in asceding order
void initNumbers(int** numbers, int interval_start, int interval_end) {
	int i, length = interval_end - interval_start;

	*numbers = (int*)malloc(sizeof(int) * length);
	for (i = 0; i < length; i++)
		(*numbers)[i] = interval_start + i;
}
//creates array of random numbers without repeating any
void initRandNumbers(int** numbers, int length) {
	int rn,i, j, curr_length = 0, new_number;
	rn = my_rand();
	*numbers = (int*)malloc(sizeof(int) * length);
	for (i = 0; i < length; i++) {
		(*numbers)[i] = rn;
		curr_length++;
		new_number = 0;
		while (!new_number) {
			rn = my_rand();
			for (j = 0; j < curr_length; j++) {
				if ((*numbers)[j] == rn) {
					new_number = 0;
					break;
				}
				new_number = 1;
			}
		}
	}
}

//based on the KFY shuffle
void shuffleNumbers(int*numbers, int length) {
	int i,j,tmp;
	for (i = 0; i < length; i++) {
		j = my_rand() % (length - i) + i;
		tmp = numbers[j];
		numbers[j] = numbers[i];
		numbers[i]=tmp;
	}
}
void copyArr(int* a, int* b, int length) {
	int i;
	for (i = 0; i < length; i++) {
		a[i] = b[i];
	}
}
//insert_shuffle=1 -> shuffle the numbers before inserting
//search_shuffle=1 -> shuffle the numbers before searching
void IntervalTest(int interval_start,int interval_end,int insert_shuffle, int search_reshuffle,int search_cycles) {

	RBNODE* rootRB = NULL;
	AVLNODE* rootAVL = NULL;
	TABLE* tableOwn = NULL;
	struct int_set* tableOther = NULL;
	int* numbers,*search_numbers;
	int length = interval_end- interval_start;
	initNumbers(&numbers, interval_start, interval_end);
	if(insert_shuffle)shuffleNumbers(numbers, length);
	if (search_reshuffle) {
		search_numbers = (int*)malloc(sizeof(int) * length);
		copyArr(search_numbers, numbers, length);
		shuffleNumbers(search_numbers, length);
	}
	else {
		search_numbers = numbers;
	}
	if(insert_shuffle)
		printf("Insert numbers from %d to %d in random order and search for them ", interval_start, interval_end);
	else 
		printf("Insert numbers from %d to %d in ascending order and search for them ",interval_start,interval_end);
	printf("%d times\n ", search_cycles);
	if(search_reshuffle)
		printf("Numbers are shuffled before searching\n");
	printf("\n");
	printf("My red-black tree insert time: %f ms\n", InsertTestRB(&rootRB,numbers, length));
	printf("My red-black tree search time: %f ms\n", SearchTestRB(rootRB, numbers, length, search_cycles));
	freeRBTree(rootRB);
	printf("Taken AVL tree insert time: %f ms *huge because of recursive insert\n", InsertTestAVL(&rootAVL, numbers, length));
	printf("Taken AVL tree search time : %f ms\n\n", SearchTestAVL(rootAVL, numbers, length, search_cycles));
	freeAVLTree(rootAVL);
	printf("My hash table with chaining insert time: %f ms\n", InsertTestHTOwn(&tableOwn, numbers, length));
	printf("My hash table with chaining search time: %f ms\n", SearchTestHTOwn(tableOwn, numbers, length, search_cycles));
	freeHTableOwn(tableOwn);
	printf("Taken hash table with open addressing insert time: %f ms\n", InsertTestHTOther(&tableOther, numbers, length));
	printf("Taken hash table with open addressing search time: %f ms\n\n", SearchTestHTOther(tableOther, numbers, length, search_cycles));
	int_set_destroy(tableOther);
	printf("-----------------------------------------------------\n");
}

void randomTest(int length, int search_reshuffle, int search_cycles) {
	RBNODE* rootRB = NULL;
	AVLNODE* rootAVL = NULL;
	TABLE* tableOwn = NULL;
	struct int_set* tableOther = NULL;
	int* numbers, * search_numbers;
	initRandNumbers(&numbers, length);
	if (search_reshuffle) {
		search_numbers = (int*)malloc(sizeof(int) * length);
		copyArr(search_numbers, numbers, length);
		shuffleNumbers(search_numbers, length);
	}
	else {
		search_numbers = numbers;
	}
	printf("Insert %d random numbers and search for them %d times \n", length,search_cycles);
	if (search_reshuffle)
		printf("Numbers are shuffled before searching\n");
	printf("\n");
	printf("My red-black tree insert time: %f ms\n", InsertTestRB(&rootRB, numbers, length));
	printf("My red-black tree search time: %f ms\n", SearchTestRB(rootRB, numbers, length, search_cycles));
	freeRBTree(rootRB);
	printf("Taken AVL tree insert time: %f ms *huge because of recursive insert\n", InsertTestAVL(&rootAVL, numbers, length));
	printf("Taken AVL tree search time : %f ms\n\n", SearchTestAVL(rootAVL, numbers, length, search_cycles));
	freeAVLTree(rootAVL);
	printf("My hash table with chaining insert time: %f ms\n", InsertTestHTOwn(&tableOwn, numbers, length));
	printf("My hash table with chaining search time: %f ms\n", SearchTestHTOwn(tableOwn, numbers, length, search_cycles));
	freeHTableOwn(tableOwn);
	printf("Taken hash table with open addressing insert time: %f ms\n", InsertTestHTOther(&tableOther, numbers, length));
	printf("Taken hash table with open addressing search time: %f ms\n\n", SearchTestHTOther(tableOther, numbers, length, search_cycles));
	int_set_destroy(tableOther);
	printf("-----------------------------------------------------\n");
}

int main() {
	srand((unsigned)time(0));

	IntervalTest(0,1000000,0,0,10);
	IntervalTest(0, 1000000, 0, 1, 10);
	IntervalTest(0, 1000000, 1, 0, 10);
	IntervalTest(0, 1000000, 1, 1, 10);
	IntervalTest(0, 300000,0, 0, 10);
	IntervalTest(0, 300000, 0, 1, 10);
	IntervalTest(0, 300000, 1, 0, 10);
	IntervalTest(0, 300000, 1, 1, 10);
	IntervalTest(1000000000, 1001000000,0, 0, 10);
	IntervalTest(1000000000, 1001000000, 0, 1, 10);
	IntervalTest(1000000000, 1001000000, 1, 0, 10);
	IntervalTest(1000000000, 1001000000, 1, 1, 10);
	randomTest(200000, 0, 10);
	randomTest(200000, 1, 10);
	randomTest(20000, 0, 10);
	randomTest(20000, 1, 10);

	return 0;
	
}


