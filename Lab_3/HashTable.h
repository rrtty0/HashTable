#pragma once

class HashTable
{
private:
	int num_of_elements;           //number of elements in table
	vector<list<int>> table;       //table with data
	static const int startSize;    //defaulr number of groups in table

	void changeTableSize(float);   //change number of groups in table

public:
	HashTable();                                  //constructor without arguments
	HashTable(const HashTable&);                  //constructor of copy
	HashTable(HashTable&&);                       //constructor with move
	explicit HashTable(int);                      //constructor with size
	HashTable& operator=(const HashTable&);       //= with copy
	HashTable& operator=(HashTable&&);            //= with move
	HashTable& operator&=(const HashTable&);      //operator &= for hashTable
	HashTable operator&(const HashTable&) const;  //operator & for hashTable
	HashTable& operator|=(const HashTable&);      //operator |= for hashTable
	HashTable operator|(const HashTable&) const;  //operator | for hashTable
	HashTable& operator+=(const HashTable&);      //operator += for hashTable(additional with mod 2)
	HashTable operator+(const HashTable&) const;  //operator + for hashTable(additional with mod 2)
	~HashTable();                                 //destructor without arguments

	bool add(int);                                //add element to table
	bool remove(int);                             //delete element from table
	bool contains(int);                           //check availability of element in hashtable
	int hashCode(int);                            //get hashcode of value
	int getSize();                                //get number of groups in table
	int getNumberOfElements();                    //get number of elements in table
	void show();                                  //print hashTable on screen
	void generate(int = 100);                     //generate hashTable
	
};

