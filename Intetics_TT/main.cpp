#include <ctime> //used for show time of work
#include <iostream> //for printTable func, it doesn't influence on algorithm
#include <string>

using std::string;

//struct to save result of task
typedef struct ResultInfo {
	unsigned long long pal; //searched palindrom
	unsigned int mul1; //first multiplier
	unsigned int mul2; //second multiplier
	ResultInfo() = default;
	//prints an Info about result
	void print() { 
		if (pal != 0)
			printf("Result = %d, -> %d * %d\n", pal, mul1, mul2);
		else
			printf("Pal not found\n");
	}
};

//function that checks if arg is palindrom
bool is_pal(string checked) {
	int l = 0, r = checked.size() - 1;
	do {
		if (checked[l] != checked[r])
			return false;
	} while (++l < --r);
	return true;
}

//searches all prime numbers using sieve of Atkin
void AtkinSearch(bool* table, const int& limit) {
	int i, j;
	int x2, y2, n;
	int sqrlimit = (int)sqrt((long double)limit);
	for (int i = 0; i <= limit; i++)
		table[i] = false;
	table[2] = true;
	table[3] = true;
	x2 = 0;
	for (i = 1; i <= sqrlimit; i++) {
		x2 += 2 * i - 1;
		y2 = 0;
		for (j = 1; j <= sqrlimit; j++) {
			y2 += 2 * j - 1;
			//n = 4 * x2 + y2;
			n = 4 * x2 + y2;
			if ((n <= limit) && (n % 12 == 1 || n % 12 == 5))
				table[n] = !table[n];
			//n = 3 * x2 + y2;
			n -= x2;
			if ((n <= limit) && (n % 12 == 7))
				table[n] = !table[n];
			//n = 3 * x2 - y2;
			n -= 2 * y2;
			if ((i > j) && (n <= limit) && (n % 12 == 11))
				table[n] = !table[n];
		}
	}
	for (i = 5; i < sqrlimit; i++) {
		if (table[i]) {
			n = i*i;
			for (j = n; j < limit; j += n)
				table[j] = false;
		}
	}
}

//prints sieve of Atkin
void printTable(bool* table, const int& limit) {
	for (int i = 1; i <= limit; i++) {
		std::cout << i << " " << std::boolalpha << table[i] << std::endl;
	}
}

//searches the biggest palindrom from two prime numbers of 5 digits
ResultInfo* searchPal(bool* table, const int& size) {
	long long i = size;
	long long j;
	unsigned long long poss = 0; //possible palindrom
	ResultInfo* res = (ResultInfo*)malloc(sizeof(ResultInfo));
	res->pal = 0;
	//we are interested only in 5-digit numbers
	while (i > 9999) {
		//to make a palindrom one of prime numbers must end with 11
		//this step allow to reduce the time of search from 270s to 7s
		if (table[i] && (i % 100 == 11)) { 
			j = i;
			//we are interested only in 5-digit numbers
			while (j > 9999) {
				if (table[j]) {
					poss = i*j;
					if (is_pal(std::to_string(poss))) {
						if (poss > res->pal) {
							res->pal = poss;
							res->mul1 = i;
							res->mul2 = j;
						}
					}
				}
				j--;
			}
		}
		i--;
	}; 
	return res;
}

int main() {
	/*test of is_pal()*/
	/*
	int t1 = 12321;
	int t2 = 123321;
	int t3 = 12331;
	bool f1 = is_pal(std::to_string(t1));
	bool f2 = is_pal(std::to_string(t2));
	bool f3 = is_pal(std::to_string(t3));
	if (f1) printf("true f1\n");
	if (f2) printf("true f2\n");
	if (f3) printf("true f3\n");
	*/
	int limit = 99999; //limit of sieve
	bool* table = (bool*)malloc(sizeof(bool)*limit); //table where prime numbers are storaged
	unsigned int start = clock(); //timer starts
	AtkinSearch(table, limit); //building a table of prime numbers
	ResultInfo* final = searchPal(table, limit); //searching for palindrom
	unsigned int end = clock(); //timer ends
	//printTable(table, limit); //prints table of prime numbers up to limit
	final->print(); //printing result
	printf("Time of work = %f\n", (end - start)/1000.0); //printing time of algorithm in sec
	return 0;
}