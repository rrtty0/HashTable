#include "HashTable.h"

//default number of groups in table
const int HashTable::startSize = 16;

//constructor without arguments
HashTable::HashTable(){
	num_of_elements = 0;
	table.resize(startSize);
	for (int i = 0; i < startSize; ++i)
		table[i] = list<int>();
}

//constructor of copy
HashTable::HashTable(const HashTable& hashTable){
	this->num_of_elements = hashTable.num_of_elements;
	this->table.reserve(hashTable.table.size());
	copy(hashTable.table.begin(), hashTable.table.end(), back_inserter(this->table));
}

//constructor with move
HashTable::HashTable(HashTable&& hashTable){
	HashTable empty;
	empty.table.clear();

	this->num_of_elements = hashTable.num_of_elements;
	this->table = hashTable.table;
	
	hashTable.table = empty.table;
}

//constructor with size
HashTable::HashTable(int size){
	int _size;

	if (size > INT_MAX || size <= startSize)
		_size = startSize;
	else
	{
		int degree = 4;
		while (pow(2, degree) < size)
		{
			_size = pow(2, degree + 1);
			++degree;
		}
	}

	num_of_elements = 0;
	table.resize(_size);

	for (int i = 0; i < _size; ++i)
		table[i] = list<int>();
}

//= with copy
HashTable& HashTable::operator=(const HashTable& hashTable){
	if (this != &hashTable)
	{
		this->num_of_elements = hashTable.num_of_elements;
		this->table.reserve(hashTable.table.size());
		copy(hashTable.table.begin(), hashTable.table.end(), back_inserter(this->table));
	}

	return *this;
}

//= with move
HashTable& HashTable::operator=(HashTable&& hashTable){
	if (this != &hashTable)
	{
		HashTable empty;
		empty.table.clear();

		this->num_of_elements = hashTable.num_of_elements;
		this->table = hashTable.table;

		hashTable.table = empty.table;
	}
	
	return *this;
}

//operator &= for hashTable
HashTable& HashTable::operator&=(const HashTable& hashTable){
	HashTable cur(hashTable);
	
	for (int i = 0; i < table.size(); ++i)
		for (auto it = table[i].begin(); it != table[i].end();)
			if (!cur.contains(*it))
			{
				it = table[i].erase(it);
				--num_of_elements;
			}
			else
				++it;

	while (table.size() > startSize && (double)num_of_elements / (double)table.size() < 0.2)
		changeTableSize(0.5);
		
	return *this;
}

//operator & for hashTable
HashTable HashTable::operator&(const HashTable& hashTable) const{
	HashTable cur(*this);
	return (cur &= hashTable);
}

//operator |= for hashTable
HashTable& HashTable::operator|=(const HashTable& hashTable){
	HashTable cur(hashTable);

	for (int i = 0; i < cur.table.size(); ++i)
		for (auto it = cur.table[i].begin(); it != cur.table[i].end(); ++it)
			if (!contains(*it))
			{
				add(*it);
				++num_of_elements;
			}

	return *this;
}

//operator | for hashTable
HashTable HashTable::operator|(const HashTable& hashTable) const{
	HashTable cur(*this);
	return (cur |= hashTable);
}

//operator += for hashTable(additional with mod 2)
HashTable& HashTable::operator+=(const HashTable& hashTable){
	HashTable cur(hashTable);

	for (int i = 0; i < cur.table.size(); ++i)
		for (auto it = cur.table[i].begin(); it != cur.table[i].end(); ++it)
			if (!contains(*it))
			{
				add(*it);
				++num_of_elements;
			}
			else
				remove(*it);

	return *this;
}

//operator + for hashTable(additional with mod 2)
HashTable HashTable::operator+(const HashTable& hashTable) const{
	HashTable cur(*this);
	return (cur += hashTable);
}

//destructor without arguments
HashTable::~HashTable(){
	for (int i = 0; i < table.size(); ++i)
		table[i].clear();
	table.clear();
}

//print hashTable on screen
void HashTable::show(){
	for (int i = 0; i < table.size(); ++i)
	{
		cout << i << ":";
		for (int el : table[i])
			cout << " " << el;
		cout << endl;
	}
}

//get hashcode of value
int HashTable::hashCode(int value){
	return value % table.size();
}

//add element to table
bool HashTable::add(int value){
	int hash = hashCode(value);
	int size_of_group = table[hash].size();
	bool added = false;

	for (auto it = table[hash].begin(); it != table[hash].end() && !added; ++it)
	{
		if (*it == value)
			return false;

		if (*it > value)
		{
			table[hash].emplace(it, value);
			added = true;
			++num_of_elements;
		}
	}

	if (!added)
	{
		table[hash].push_back(value);
		++num_of_elements;
	}

	if ((double)num_of_elements / (double)table.size() > 0.75)
		changeTableSize(2);

	return true;
}

//change number of groups in table
void HashTable::changeTableSize(float coef){
	vector<list<int>> oldTable;

	oldTable.resize(coef * table.size());

	for (int i = 0; i < oldTable.size(); ++i)
		oldTable[i] = list<int>();

	swap(table, oldTable);
	num_of_elements = 0;

	for (int i = 0; i < oldTable.size(); ++i)
		for (auto it = oldTable[i].begin(); it != oldTable[i].end(); ++it)
			add(*it);

	for (int i = 0; i < oldTable.size(); ++i)
		oldTable[i].clear();
	oldTable.clear();
}

//delete element from table
bool HashTable::remove(int value){
	int hash = hashCode(value);
	bool removed = false;
	
	for (auto it = table[hash].begin(); !removed && it != table[hash].end();)
	{
		if (*it > value)
			return false;

		if (*it == value)
		{
			it = table[hash].erase(it);
			removed = true;
			--num_of_elements;
		}
		else
			++it;
	}
	
	if (table.size() > startSize && (double)num_of_elements / (double)table.size() < 0.2)
		changeTableSize(0.5);

	return removed;
}

//check availability of element in hashtable
bool HashTable::contains(int value){
	int hash = hashCode(value);
	int size_of_group = table[hash].size();

	for (auto it = table[hash].begin(); it != table[hash].end(); ++it)
		if (*it == value)
			return true;

	return false;
}

//get number of groups in table
int HashTable::getSize(){
	return table.size();
}

//get number of elements in table
int HashTable::getNumberOfElements(){
	return num_of_elements;
}

//generate hashTable
void HashTable::generate(int supremum){
	srand(time(NULL));
	if (num_of_elements != 0)
	{
		for (int i = 0; i < table.size(); ++i)
			table[i].clear();
		num_of_elements = 0;
	}

	int new_number_of_elements = 0.2 * table.size() + rand() % (table.size() / 2);

	for (int i = 0; i < new_number_of_elements; ++i)
		add(rand() % supremum);
}