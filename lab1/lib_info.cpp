//File Name: lib_info.cpp
//Name: Josephine Rich
//Date: 02/06/2022
/*Description: This program opens a file given from the commandline and parses through
 * the information about MP3 files and organizes it with maps. It then prints out the 
 * information in a very specific format. */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdio.h>
using namespace std;

class Song {
	public:
		string title;
		int time;
		int track;
};

class Album {
	public: 
		map<int, Song *> songs;
		string name;
		int time;
};

class Artist {
	public:
		map <string, Album *> albums;
		string name;
		int time;
		int nsongs;
};


int main(int argc,char *argv[]) {
	string line;
	char title[100], artist[100], album[100], genre[100]; /*have to use chars for sscanf*/
	int track, minute, second, time_sec;
	size_t i, j;
	
	Artist *artp;
	Song *sp;
	Album *albp;
	
	vector <string> temp; /*vector to hold all strings from each line to make it easier to change all the '_' to ' '*/
	vector <Song *> all_sp;     /*vectors to store all of the memory addresses of each class type to make*/
	vector <Album *> all_albp;  /*reallocation of memory easier instead of having to traverse complex data structures*/
	vector <Artist *> all_artp;

	map<int, Song*> ::iterator sit;
	map<string, Album *> ::iterator albit;
	map<string, Artist *> ::iterator artit;
	map<string, Artist *> artists;

	(void)argc; /*gets rid of warning when compiling*/

	ifstream musicFile(argv[1]); /*opening file from commandline*/ 
	
	if (musicFile.is_open()){ /*making sure the file successfully opened*/
		while(getline(musicFile, line)) { /*reading in each line*/
			
			temp.clear(); /*clearing the vector for the new line*/

			sscanf(line.c_str(), "%s %d:%d %s %s %s %d", title, &minute, &second, artist,
					album, genre, &track);/*parsing through information*/
			
			time_sec = (minute * 60) + second; /*converting the minutes:seconds to seconds*/

			/*pushing all the strings onto the temp vector*/
			temp.push_back(title); /*temp[0]*/
			temp.push_back(artist); /*temp[1]*/
			temp.push_back(album); /*temp[2]*/
			temp.push_back(genre); /*temp[3]*/


			for(i=0; i < temp.size(); i++) { /*taking all of the '_' out of the strings*/ 
				for(j=0; j < temp[i].size(); j++) {
					if (temp[i][j] == '_') temp[i][j] = (' ');
					else temp[i][j] = temp[i][j];
				}
			}
			
			sp = new Song; /*creating a new instance of the Song class*/
			sp->title = temp[0]; /*setting all of the member variables*/
			sp->time = time_sec;
			sp->track = track;
			all_sp.push_back(sp); /*saving pointer for later in vector*/

			artit = artists.find(temp[1]); /*checking to see if the artist already exists*/
			if(artit == artists.end()) { /*is a new artist and needs to be put in map*/
				artp = new Artist;
				artp->name = temp[1];
				artp->nsongs = 0;
				artp->time = 0;
				artists.insert(make_pair(temp[1], artp));
				all_artp.push_back(artp); /*saving pointer for later in vector*/
			}
			else { /*already in map*/
				artp = artit->second;
			}

			
			albit = artp->albums.find(temp[2]); /*checking for album in map under artist class*/
			if(albit == artp->albums.end()) { /*is a new album and needs to be put in album map*/
				albp = new Album;
				albp->name = temp[2];
				albp->time = 0;
				artp->albums.insert(make_pair(temp[2], albp));
				all_albp.push_back(albp);
			}
			else { /*already in map*/
				albp = albit->second;
			}

			albp->songs.insert(make_pair(track, sp)); /*inserting the songs into the album's map*/
			albp->time += (sp->time); /*updating the album's total time*/
			artp->nsongs += 1; /*updating the artists number of songs*/
			artp->time += (sp->time); /*updating the artists total time*/

			
		}
		musicFile.close();

		/*printing all the information out formatted correctly*/
		for(artit = artists.begin(); artit !=artists.end(); artit++) { /*iterating through the artist map*/
			artp = artit->second;
			minute = (artp->time / 60); /*converting from seconds to minutes:seconds*/
			second = (artp->time) - (minute * 60);
			printf("%s: %d, %d:%02d\n", (artit->first).c_str(), artp->nsongs, minute, second); 
			
			for(albit = artp->albums.begin(); albit != artp->albums.end(); albit++) {/*iterating through the albums map of artist*/
				
				albp = albit->second;
				minute = (albp->time / 60); /*converting from seconds to minutes:seconds*/
				second = (albp->time) - (minute * 60);

				printf("%8s%s: %zu, %d:%02d\n", " ", (albit->first).c_str(), albp->songs.size(), minute, second);
				
				for(sit = albp->songs.begin(); sit != albp->songs.end(); sit++) {/*iterating through the songs of each album*/
					sp =sit->second;
					minute = (sp->time / 60); /*converting from seconds to minutes:seconds*/
					second = (sp->time) - (minute * 60);

					printf("%16s%d. %s: %d:%02d\n", " ", sp->track, sp->title.c_str(), minute, second);
				}
			}
		}

		/*reallocating all the memory from the new classes*/
		for(i=0; i < all_sp.size(); i++) {
			delete all_sp[i];
		}

		for(i=0; i < all_albp.size(); i++) {
			delete all_albp[i];
		}

		for(i=0; i < all_artp.size(); i++) {
			delete all_artp[i];
		}

	}
	else cout << "Can't open file" << endl;
	
	return 0;

}
