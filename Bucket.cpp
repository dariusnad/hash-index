#include <iostream>
#include "ExtensibleHashTable.h"
#include <cstring>

using namespace std;

//Constructor with two parameters for the bucket
Bucket::Bucket(int depth, int bucket_size)
{
	bucket = new int[bucket_size];
	local_depth = depth;
	size = bucket_size;
	numb_keys = 0;
}

//Destructor
Bucket::~Bucket()
{
	delete[] bucket;
}

//Find function to look for a key inside the bucket
bool Bucket::find(int key)
{
	for (int i = 0; i < numb_keys; i++)
	{
		if (bucket[i] == key)
		{
			return true;
		}
	}
	return false;
}

//Insert function to add a key into the bucket 
void Bucket::insert(int key)
{
	if (isFull())	//Checking for a full bucket
	{
		return;
	}
	bucket[numb_keys++] = key;
}

//Retrieves the local depth of the bucket
int Bucket::get_depth()
{
	return local_depth;
}

//Increases the local depth by one
void Bucket::increase_depth()
{
	this->local_depth++;
}

//Checks for an empty bucket
bool Bucket::isEmpty()
{
	return numb_keys == 0;
}

//Determines if the bucket is full
bool Bucket::isFull()
{
	return numb_keys == size;
}

//Copy constructor for the bucket
Bucket::Bucket(const Bucket& copy)
{
	bucket = new int[copy.size];
	size = copy.size;
	local_depth = copy.local_depth;
	numb_keys = copy.numb_keys;
	memcpy(bucket, copy.bucket, sizeof(int) * size);
}

//Overloading of the assignment operator
void Bucket::operator=(const Bucket& other_buck)
{
	if (this == &other_buck)
	{
		return;
	}
	delete[] bucket;
	bucket = new int[other_buck.size];
	local_depth = other_buck.local_depth;
	size = other_buck.size;
	numb_keys = other_buck.numb_keys;
	memcpy(bucket, other_buck.bucket, sizeof(int) * size);
}

//Removes a key from the bucket
bool Bucket::remove(int key)
{
	if (isEmpty())
	{
		return false;
	}

	int* new_bucket = new int[size];
	int appearances = 0;
	bool key_found = false;
	int j = 0;
	for (int i = 0; i < numb_keys; i++)
	{
		//If the key is found in the bucket
		if (bucket[i] == key)
		{
			key_found = true;
			appearances++;
		}
		//Copy over keys we want to keep into a new bucket
		if (bucket[i] != key)
		{
			new_bucket[j] = bucket[i];
			j++;
		}
	}
	delete[] bucket;	//Remove the old bucket
	bucket = new_bucket;
	numb_keys = numb_keys - appearances;
	return key_found;
}

//Retrieve the number of keys
int Bucket::get_numbkeys()
{
	return this->numb_keys;
}

//Prints the contents of the bucket
void Bucket::print()
{
	cout << "[";
	for (int j = 0; j < size; j++)
	{
		if (j >= numb_keys)
		{
			if (j == size - 1)
			{
				cout << "-";
			}
			else
			{
				cout << "-" << ",";
			}
		}
		else
		{
			if (j == size - 1)
			{
				cout << bucket[j];
			}
			else
			{
				cout << bucket[j] << ",";
			}
		}
	}
	cout << "] " << "(" << local_depth << ")" << endl;
}