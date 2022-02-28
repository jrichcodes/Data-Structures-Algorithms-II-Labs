//File: cs302-midi.cpp
//Name: Josephine Rich
//Date: 2/20/2022

//Description: In this lab, we were given a class called CS302_Midi that lets you read, manipulate and wrtie both
//types of MIDI files (event files and note files). I wrote the el_to_nd() and nd_to_el() methods that helps
//convert between the two types of files. 

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "cs302-midi.h"
using namespace std;

void CS302_Midi::el_to_nd()
{
	vector <ND *> temp (128, NULL);
	EventList::iterator eit;
	Event *e;
	ND *n, *d;
	int index;
	double current_time = 0;

	nd = new NDMap; /*setting the NDMap pointer to a new NDMap*/

	for(eit = el->begin(); eit != el->end(); eit++){ /*iterating through the EventList*/
		e = *eit; /*setting event pointer to the iterator*/
		
		if(e->key == 'O') { /*if statements to parse through the different types of events*/
			//turning note on 
			n = new ND;
			n->key = 'N';
			n->start = (e->time/480.) + current_time;
			n->volume = e->v2;
			n->pitch = e->v1;
			index = e->v1;
			temp[index] = n; /*inserting into temp vector based on pitch*/
			current_time += (e->time/480.); /*updating the time to keep the relevant time*/
		}
		if(e->key == 'F') { /*turning note off*/
			index = e->v1;
			n = temp[index];
			n->stop = (e->time/480.) + current_time;
			nd->insert(make_pair(n->start, n)); /*inserting into the NDMap*/
			current_time += (e->time/480.);
		}
		if(e->key == 'D') {
			if(e->v1 == 1) { /*damper down*/
				d = new ND;
				d->key = 'D';
				d->start = (e->time/480.) + current_time;
				current_time += (e->time/480.);
			}
			if(e->v1 == 0){ /*damper up*/
				d->stop = (e->time/480.) + current_time;
				current_time += (e->time/480.);
				nd->insert(make_pair(d->start, d));
			}

		}
	}
}

void CS302_Midi::nd_to_el()
{
	NDMap::iterator nit;
	map <int, multimap<int, Event *> > temp;
	map <int, multimap<int, Event *> >::iterator it;
	multimap <int, Event *>::iterator it2;
	ND *n;
	Event *e;
	int starttime, stoptime, prevtime = 0;

	el = new EventList; /*setting the EventList pointer to a new EventList*/

	for(nit = nd->begin(); nit != nd->end(); nit++){ /*iterating through the NDMap*/
		n = nit->second;
		starttime = rint(n->start * 480); /*calculating start and stop time*/
		stoptime = rint(n->stop * 480);
	
		if(rint(n->start * 480) == rint(n->stop * 480)) continue; /*ignoring the event b/c start and stop at the same time*/
		
		//event: 0 = OFF; 1 = DAMPER UP; 2 = DAMPER DOWN; 3 = ON

		temp.insert(make_pair(starttime, multimap<int, Event *>()));
		temp.insert(make_pair(stoptime, multimap<int, Event *>()));
		
		if(n->key == 'N') { /*note on*/
			e = new Event;
			e->key = 'O';
			e->time = rint(n->start * 480);
			e->v1 = n->pitch;
			e->v2 = n->volume;

			temp[starttime].insert(make_pair(3, e)); /*inserting when to turn on note*/
			
			e = new Event; /*note off*/
			e->key = 'F';
			e->time = rint(n->stop * 480);
			e->v1 = n->pitch;

			temp[stoptime].insert(make_pair(0, e)); /*inserting when to turn off note*/
		}
		
		if(n->key == 'D') { /*damper down*/
			e = new Event;
			e->key = 'D';
			e->time = rint(n->start * 480);
			e->v1 = 1;
			
			temp[starttime].insert(make_pair(2, e)); /*inserting when to damper down*/

			e = new Event; /*damper up*/
			e->key = 'D';
			e->time = rint(n->stop * 480);
			e->v1 = 0;

			temp[stoptime].insert(make_pair(1, e)); /*inserting when to release damper*/
		}

	}

	for(it = temp.begin(); it != temp.end(); it++){ /*traversing the temporary map*/
		for(it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			e = it2->second;
			if(it2 == it->second.begin()) e->time = (e->time) - (prevtime); /*adjusting time*/
			else e->time = 0;
			el->push_back(e); /*pushing back onto EventList*/
		}
		prevtime = it->first; /*keeping track of previous time*/
	}

}
