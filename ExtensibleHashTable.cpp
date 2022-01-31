#include <iostream>
#include "ExtensibleHashTable.h"
#include <iomanip>
#include <cstring>
#include <map>
#include <cmath>

using namespace std;

//Default Constructor
ExtensibleHashTable::ExtensibleHashTable()
{
	bucket_size = 4;
	global_depth = 1;
	directory_size = 2;
	directory = new Bucket * [2];
	directory[0] = new Bucket(global_depth, bucket_size);
	directory[1] = new Bucket(global_depth, bucket_size);
}

//Constructor with one parameter
ExtensibleHashTable::ExtensibleHashTable(int x)
{
	bucket_size = x;
	global_depth = 1;
	directory_size = 2;
	directory = new Bucket * [2];
	directory[0] = new Bucket(global_depth, bucket_size);
	directory[1] = new Bucket(global_depth, bucket_size);
}

//Destructor
ExtensibleHashTable::~ExtensibleHashTable()
{
	delete []directory;
}

//Hash function
int ExtensibleHashTable::hash(int key)
{
	int numb_buckets = pow(2, global_depth);
	return key % numb_buckets;
}

//Find function to look for a key
bool ExtensibleHashTable::find(int key)
{
	int bucket_numb = hash(key);	//Retrieving the directory index
	Bucket& target_bucket = *directory[bucket_numb];
	return target_bucket.find(key);	//Calling find function for the bucket
}

//Insert function to place a key in the hash table
void ExtensibleHashTable::insert(int key)
{
	int bucket_numb = hash(key);	//Retrieving the directory index	
	Bucket& target_bucket = *directory[bucket_numb];
	bool identical_check = true;
	try
	{
		//Checking to see if we are attempting to insert into a bucket with identical values
		for (int j = 0; j < target_bucket.numb_keys; j++)
		{
			//Comparing key value with the bucket keys
			if (key != target_bucket.bucket[j])
			{
				identical_check = false;
			}
		}
		if (identical_check == true && target_bucket.isFull())
		{
			throw runtime_error("Attempting to insert identical value into bucket with the same values");
		}
	}
	catch (runtime_error& ex)
	{
		cout << ex.what() << endl;
		return;
	}

	//Splitting the bucket and inserting
	if (target_bucket.isFull() && global_depth > target_bucket.get_depth())
	{
		split_bucket(target_bucket, bucket_numb);
		insert(key);
	}
	//Doubling directory, splitting the bucket, and attempting to insert again
	else if(target_bucket.isFull() && global_depth == target_bucket.get_depth())
	{
		double_directory();
		split_bucket(target_bucket, bucket_numb);
		insert(key);
	}
	else
	{
		target_bucket.insert(key);	//Inserting when the bucket is not full
	}
}

//Function to the double the size of the directory
void ExtensibleHashTable::double_directory()
{

	int old_directory_size = (1 << global_depth);
	int new_directory_size = (1 << (global_depth + 1));
	Bucket ** new_directory = new Bucket*[new_directory_size];
	int j = directory_size;
	int i = 0;

	//Placing pointers from old directory to the new one
	for (int y = 0; y < directory_size; y++)
	{
		new_directory[y] = directory[y];
	}

	while (j < new_directory_size) //Setting pointers for second half of directory
	{
		new_directory[j] = directory[i];
		i++;
		j++;
	}
	delete[] directory;
	directory = new_directory;	//Assigning the new directory
	directory_size = new_directory_size;
	global_depth++;
}

//Function to split a full bucket
void ExtensibleHashTable::split_bucket(Bucket& target_bucket, int bucket_numb)
{
	int local_depth = target_bucket.get_depth();
	target_bucket.increase_depth();
	int new_bucket_local_depth = local_depth + 1;

	int ptr_index = 0;
	//Loop to find the position in the directory of the second pointer which points to the same bucket
	for (int i = (bucket_numb + 1) % directory_size; i < directory_size; i++)
	{
		if (directory[i] == directory[bucket_numb])
		{
			ptr_index = i;
			break;
		}
	}

	int mask = (1 << (local_depth + 1)) - 1;	//Creating a mask based on the local depth of the bucket

	int* original_bucket = new int[target_bucket.size];
	int* new_bucket_temp = new int[target_bucket.size];
	int i = 0, j = 0, n = 0;
	int original_bucket_keys = 0, new_bucket_keys = 0;
	for (int i = 0; i < target_bucket.size; i++)
	{
		int key = mask & target_bucket.bucket[i];	//Key value of each element using bitwise AND with the mask

		if (key == bucket_numb)	//Keeping elements in the original bucket
		{
			original_bucket[n++] = target_bucket.bucket[i];
			original_bucket_keys++;
		}
		else if (key == ptr_index)	//Adding elements to the new bucket
		{
			new_bucket_temp[j++] = target_bucket.bucket[i];
			new_bucket_keys++;
		}
	}
	delete[] target_bucket.bucket;
	target_bucket.bucket = original_bucket;	//Assigning the original bucket with its new keys
	target_bucket.numb_keys = original_bucket_keys;
	Bucket* new_bucket = new Bucket(new_bucket_local_depth, bucket_size);	//Creating the new bucket
	delete[] new_bucket->bucket;
	new_bucket->bucket = new_bucket_temp;	//Assigning the new bucket
	new_bucket->numb_keys = new_bucket_keys;
	directory[ptr_index] = new_bucket;	//Placing the new bucket in the directory 
}

//Printing the contents of the directory 
void ExtensibleHashTable::print()
{
	map<Bucket*,int> direc_map;	//Using a map to determine if the directory pointer needs to actually print the bucket
	for (int i = 0; i < directory_size; i++)
	{
		//Case for when a bucket has already been printed
		if (direc_map.find(directory[i]) != direc_map.end())
		{
			cout << i << ": " << directory[i] << " --> " << endl;
		}
		else
		{
			//Case for when a bucket has not yet been printed
			cout << i << ": " << directory[i] << " --> ";
			directory[i]->print();	//Calling the print function for this bucket
			direc_map.insert({ directory[i], 1 });
		}
	}
}

//Function to remove a key from the hash table
bool ExtensibleHashTable::remove(int key)
{
	int bucket_numb = hash(key);
	Bucket& target_bucket = *directory[bucket_numb];	//Retrieving the appropriate bucket
	return target_bucket.remove(key);
}

//Overloading of the assignment operator
void ExtensibleHashTable::operator= (const ExtensibleHashTable& other)
{
	if (this == &other)
	{
		return;
	}
	delete[] directory;
	directory = new Bucket * [other.directory_size];
	global_depth = other.global_depth;
	directory_size = other.directory_size;
	bucket_size = other.bucket_size;
	//Assigning buckets to the directory pointers
	for (int i = 0; i < other.directory_size; i++)
	{
		directory[i] = new Bucket(other.global_depth, other.bucket_size);
		*directory[i] = *(other.directory[i]);
	}
}

//The Copy Constructor for the hash table
ExtensibleHashTable::ExtensibleHashTable(const ExtensibleHashTable& copy)
{
	directory = new Bucket * [copy.directory_size];
	directory_size = copy.directory_size;
	global_depth = copy.global_depth;
	bucket_size = copy.bucket_size;
	//Assigning buckets to the directory pointers
	for (int j = 0; j < copy.directory_size; j++)
	{
		directory[j] = new Bucket(copy.global_depth, copy.bucket_size);
		*directory[j] = *(copy.directory[j]);
	}
}


