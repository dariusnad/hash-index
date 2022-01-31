#include "Bucket.h"
#include <iostream>

using namespace std;

class ExtensibleHashTable {

public:
	ExtensibleHashTable();	//Default constructor
	ExtensibleHashTable(int);	//Constructor with one parameter
	~ExtensibleHashTable();	//Destructor
	void operator= (const ExtensibleHashTable& other);	//Overloading of the assignment operator
	ExtensibleHashTable(const ExtensibleHashTable& copy);	//The Copy Constructor for the hash table
	bool find(int);	
	void insert(int);
	bool remove(int);
	void print();

private:
	Bucket ** directory = NULL;	//Directory which containers pointers to buckets
	int global_depth = 0; 
	int bucket_size = 0;	//Size of each bucket
	int directory_size = 0;	//Size of the directory
	void split_bucket(Bucket& target_bucket, int key);	//Function to split a full bucket
	void double_directory();	//Function to double the size of a directory
	int hash(int key);	//Hash function
};