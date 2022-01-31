#include <iostream>

using namespace std;

class Bucket {
	friend class ExtensibleHashTable;
public:
	Bucket(int depth, int size);	//Constructor with two parameters
	bool find(int);
	void insert(int);
	bool remove(int);
	Bucket(const Bucket& copy);	//Copy constructor
	void operator= (const Bucket& other);	//Overloading of assignment operator
	void print();
	~Bucket();

private:
	int get_depth();	//Retrieves the local depth of a bucket
	bool isEmpty();	//Checks if a bucket is empty
	bool isFull();	//Determines if a bucket is full
	void increase_depth();	//Increases local depth by one
	int get_numbkeys();	//Retrieve the number of keys stored in the bucket
	int* bucket = NULL;	//Pointer to an int array for the bucket
	int local_depth = 0;
	int size = 0;	//Size of the bucket
	int numb_keys = 0;	//Number of keys in the bucket
};