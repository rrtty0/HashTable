#include "HashTable.h""

int main(){
	HashTable A;
	HashTable B;
	HashTable C;
	HashTable D;
	HashTable E;
	HashTable result;

	
	A.generate();
	A.add(1);                       //only for demonstration method add()
	A.remove(0);                    //only for demonstration method remove()
	cout << "HashTable A:\n";
	A.show();
	cout << "\n\n";
	
	B.generate();
	cout << "HashTable B:\n";
	B.show();
	cout << "\n\n";

	C.generate();
	cout << "HashTable C:\n";
	C.show();
	cout << "\n\n";

	D.generate();
	cout << "HashTable D:\n";
	D.show();
	cout << "\n\n";

	E.generate();
	cout << "HashTable E:\n";
	E.show();
	cout << "\n\n";

	cout << "HashTable result:\n";
	result = A & B | C | (D + E);
	result.show();
	cout << "\n\n";

	system("pause");
    return 0;
}