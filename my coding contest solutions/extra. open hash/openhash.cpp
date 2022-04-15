#include<iostream>
#include<math.h>
using namespace std;

template<class H>
class OpenHashTable {
	private:
		H**	table;
		int tablesize;
		int keycounter;
	
	protected:
		virtual int hash(H key, int i) = 0;

	public:
		OpenHashTable(int tablesize) {
			this->tablesize 	= tablesize;
			this->keycounter 	= 0;
			this->table 		= new H * [this->tablesize]{NULL}; 
		}

		int getTablesize() {
			return tablesize;
		}

		int getKeycounter() {
			return keycounter;
		}

		OpenHashTable<H>* insert(H key) {
			if (keycounter == tablesize)
				return this;
			
			for (int i=0; i < tablesize; i++) {
				int position = hash(key, i);

				if (table[position] == NULL) {
					table[position] = new H(key);
					keycounter++;
					return this;
				}
			}

			return this;
		}

		int search(H key) {
			for (int i=0; i < tablesize; i++) {
				int position = hash(key, i);

				if (table[position] != NULL && *table[position] == key) {
					return position;
				}
			}

			return -1;
		}

		OpenHashTable<H>* remove(H key) {
			int position = search(key);

			if (position == -1)
				return this;

			delete table[position];
			table[position] = NULL;
			keycounter--;
			return this;
		}
};

template<class H>
class OpenHashLinear : public OpenHashTable<H> {
	private:
		int hash(H key, int i) {
			return (((int)key % this->getTablesize()) + i) % this->getTablesize();
		}

	public:
		OpenHashLinear(int tablesize) : OpenHashTable<H>(tablesize) {}	
};

template<class H>
class OpenHashQuadratic : public OpenHashTable<H> {
	private:
		const float C = 0.5;

		int hash(H key, int i) {
			return (int)((((int)key % this->getTablesize())) + C*i + C*pow(i, 2)) % this->getTablesize();
		}

	public:
		OpenHashQuadratic(int tablesize) : OpenHashTable<H>(tablesize) {}	
};

template<class H>
class OpenHashDouble : public OpenHashTable<H> {
	private:
		int hash(H key, int i) {
			return (((int)key % this->getTablesize()) + (1 + ((int)key % (this->getTablesize() - 1)))*i) % this->getTablesize();
		}

	public:
		OpenHashDouble(int tablesize) : OpenHashTable<H>(tablesize) {}
};

int main() {
	
	cout << "TEST: Ispezione lineare" << endl;
	OpenHashLinear<int> p = OpenHashLinear<int>(11);

	p.insert(10)->insert(22)->insert(31)->insert(4)->insert(15)->insert(28)->insert(17)->insert(88)->insert(59);
	cout << p.search(59) << endl;
	p.remove(10)->remove(22)->remove(31)->remove(4)->remove(15)->remove(28)->remove(17)->remove(88)->remove(59);
	cout << p.search(59) << endl;
	p.insert(10)->insert(22)->insert(31)->insert(4)->insert(15)->insert(28)->insert(17)->insert(88)->insert(59);
	cout << p.search(59) << endl;
	

	cout << "TEST: Ispezione quadratica" << endl;
	OpenHashQuadratic<int> q = OpenHashQuadratic<int>(16);

	q.insert(10)->insert(23)->insert(40)->insert(55)->insert(58)->insert(35)->insert(18)->insert(34)->insert(16)->insert(33)->insert(32);
	cout << q.search(32) << endl;
	q.remove(10)->remove(23)->remove(40)->remove(55)->remove(58)->remove(35)->remove(18)->remove(34)->remove(16)->remove(33)->remove(32);
	cout << q.search(32) << endl;
	q.insert(10)->insert(23)->insert(40)->insert(55)->insert(58)->insert(35)->insert(18)->insert(34)->insert(16)->insert(33)->insert(32);
	cout << q.search(32) << endl;

	cout << "TEST: Hashing doppio" << endl;
	OpenHashDouble<int> v = OpenHashDouble<int>(11);

	v.insert(10)->insert(22)->insert(31)->insert(4)->insert(15)->insert(28)->insert(17)->insert(88)->insert(59);
	cout << v.search(59) << endl;
	v.remove(10)->remove(22)->remove(31)->remove(4)->remove(15)->remove(28)->remove(17)->remove(88)->remove(59);
	cout << v.search(59) << endl;
	v.insert(10)->insert(22)->insert(31)->insert(4)->insert(15)->insert(28)->insert(17)->insert(88)->insert(59);
	cout << v.search(59) << endl;

	cout << "TEST: Hashing doppio" << endl;
	OpenHashDouble<double> d = OpenHashDouble<double>(11);
	
	d.insert(10.9)->insert(22.8)->insert(31.6)->insert(4.5)->insert(15.4)->insert(28.1)->insert(17.4)->insert(88.1)->insert(59.5);
	cout << d.search(59.5) << endl;
	d.remove(10.9)->remove(22.8)->remove(31.6)->remove(4.5)->remove(15.4)->remove(28.1)->remove(17.4)->remove(88.1)->remove(59.5);
	cout << d.search(59.5) << endl;
	d.insert(10.9)->insert(22.8)->insert(31.6)->insert(4.5)->insert(15.4)->insert(28.1)->insert(17.4)->insert(88.1)->insert(59.5);
	cout << d.search(59.5) << endl;
}
