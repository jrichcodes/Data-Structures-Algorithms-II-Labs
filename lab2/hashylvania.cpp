//File: hashylvania.cpp
//Name: Josephine Rich
//Date: 02/14/2022

/*Description: This program takes in input from standard input. It hashes people and inserts them into a map 
 * of a map. Then it orders the people based on a string and modulus.*/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <string>
#include <sstream>
using namespace std;

unsigned long long jsp_hash(const string &s) /*offical hash function for Hashylvania given*/
{
	size_t i;
	unsigned long long h, t;

	h = 0x6bd300f29f1505ULL;

	for(i=0; i < s.size(); i++) {
		t = h & 0x3ffffffffffffffULL;
		h = (t << 5) ^ h ^ s[i];
	}

	return h;
}

class Person {
	public:
		string name;
		string id;
};

string dtoh (unsigned long long hash) { /*fuction used to convert from decimal to hex*/
	stringstream ss;

	ss << hex << hash;
	string hexhash(ss.str());
	return hexhash;
}

int main() {
	vector <Person *> people;
	Person *pp, *fp, *lp;
	string command, name, id, word, hashme, hashstr;
	unsigned long long modulus, hash, key, lk, hk;
	map <unsigned long long, map <unsigned long long, Person *> > m;
	map <unsigned long long, map <unsigned long long, Person *> >:: iterator it;
	map <unsigned long long, map < unsigned long long, Person*> >:: iterator prev;
	map <unsigned long long, Person*> :: iterator it2;
	size_t i;

	while(cin >> command) {
		if(command == "PERSON") {
			cin >> name >> id; /*reading in person's name and id*/
			
			pp = new Person; /*creating a new person*/
			pp->name = name;
			pp->id = id;

			people.push_back(pp); /*pushing person onto vector*/
		}
		else if(command == "ORDER") {
			if(people.size() == 0) continue; /*there are no people yet to order*/
			cin >> word >> modulus;
			m.clear();

			for(i=0; i < people.size(); i++) { /*going through the vector of people and hashing them*/
				pp = people[i];
				hashme = (pp->name) + (pp->id) + word;
				hash = jsp_hash(hashme);
				key = hash % modulus;

				m.insert(make_pair(key, map<unsigned long long, Person*>())); /*inserting into a map of a map*/
				m[key].insert(make_pair(hash, pp));
			}

			for(i=0; i < people.size(); i++) { 
				hash = jsp_hash(word); /*hashing the random string or last hash and the person's id*/
				key = hash % modulus;

				
				it = m.lower_bound(key); /*finding the map of people in the first outter map that are closest to the key*/
				
				if(it == m.end()) --it;/*lower bound returns last element*/
				else if(it == m.begin()) it = m.begin(); /*lower bound returns first element*/
				else if(it->first == key) it = it;
				else { 
					/*checking the previous and next values to find the closest to the key*/
					prev = it;
					lk = (--prev)->first;
					hk = it->first;
					if((key - lk) < (hk - key)) it = --it;  /*the lower number is closest to the key*/
				}

				it2 = it->second.lower_bound(hash); /*finding the person in the inner map 
													whos hash >= to hash*/

				if(it2 == it->second.end()) { /*largest hash*/
					--it2;
				}
			
				/*storing first and last person*/
				if(i == 0) fp = it2->second;
				if(i == people.size()-1) lp = it2->second;

				id = it2->second->id;

				hashstr = dtoh(hash); /*change decimal to hex with function*/
				word = id + hashstr; /*hash + person's id (all lowercase)*/

				m[it->first].erase(it2); /*deleting person from map after put in order*/
				if(m[it->first].size() == 0) {
					m.erase(it); /*deleting outermap key once it's empty*/
				}
			}

			/*printing last and first person in the order after each order command*/
			cout << "Last: " << (lp->name) << " " << (lp->id) << endl;
			cout << "First: " << (fp->name) << " " << (fp->id) << endl;

		}
		else {
			cout << "File doesn't have PERSON or ORDER in it" << endl;
		}
	}

	for(i=0; i < people.size(); i++) { /*freeing memory used from the people class*/
		delete people[i];
	}

	return 0;
}
