// Lab_3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	
	HashTable A;
	HashTable B;
	HashTable C;
	HashTable D;
	HashTable E;
	HashTable result;

	//A.add(0);
	//A.add(1);
	A.add(30);
	for (int i = 0; i < 15; ++i)
		A.add(i);

	//result = A & B | C | (D + E);

	A.show();
	

	//HashTable hashTable;
	//hashTable.generate();
	//hashTable.show();

	system("pause");
    return 0;
}

