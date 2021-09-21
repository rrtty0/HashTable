#include "stdafx.h"
#include "HashTable.h"


const int HashTable::startSize = 16;


HashTable::HashTable()
{
	num_of_elements = 0;
	table.resize(startSize);
	for (int i = 0; i < startSize; ++i)
		table[i] = list<int>();
}


HashTable::HashTable(const HashTable& hashTable)
{
	this->num_of_elements = hashTable.num_of_elements;
	this->table.reserve(hashTable.table.size());
	copy(hashTable.table.begin(), hashTable.table.end(), back_inserter(this->table));
}


HashTable::HashTable(HashTable&& hashTable)
{
	HashTable empty;
	empty.table.clear();

	this->num_of_elements = hashTable.num_of_elements;
	this->table = hashTable.table;
	
	hashTable.table = empty.table;
}


HashTable::HashTable(int size)
{
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


HashTable& HashTable::operator=(const HashTable& hashTable)
{
	if (this != &hashTable)
	{
		this->num_of_elements = hashTable.num_of_elements;
		this->table.reserve(hashTable.table.size());
		copy(hashTable.table.begin(), hashTable.table.end(), back_inserter(this->table));
	}

	return *this;
}


HashTable& HashTable::operator=(HashTable&& hashTable)
{
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


HashTable& HashTable::operator&=(const HashTable& hashTable)
{
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


HashTable HashTable::operator&(const HashTable& hashTable) const
{
	HashTable cur(*this);
	return (cur &= hashTable);
}


HashTable& HashTable::operator|=(const HashTable& hashTable)
{
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


HashTable HashTable::operator|(const HashTable& hashTable) const
{
	HashTable cur(*this);
	return (cur |= hashTable);
}


HashTable& HashTable::operator+=(const HashTable& hashTable)
{
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


HashTable HashTable::operator+(const HashTable& hashTable) const
{
	HashTable cur(*this);
	return (cur += hashTable);
}


HashTable::~HashTable()
{
	for (int i = 0; i < table.size(); ++i)
		table[i].clear();
	table.clear();
}


void HashTable::show()
{
	for (int i = 0; i < table.size(); ++i)
	{
		cout << i << ":";
		for (int el : table[i])
			cout << " " << el;
		cout << endl;
	}
}


int HashTable::hashCode(int value)
{
	return value % table.size();
}


bool HashTable::add(int value)
{
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


void HashTable::changeTableSize(float coef)
{
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


bool HashTable::remove(int value)
{
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


bool HashTable::contains(int value)
{
	int hash = hashCode(value);
	int size_of_group = table[hash].size();

	for (auto it = table[hash].begin(); it != table[hash].end(); ++it)
		if (*it == value)
			return true;

	return false;
}


int HashTable::getSize()
{
	return table.size();
}


int HashTable::getNumberOfElements()
{
	return num_of_elements;
}


void HashTable::generate(int supremum)
{
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