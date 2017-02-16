/*
Header file for class SongList.
*/

#ifndef SONGLIST_H //basically says that if this headerfile isn't defined already, than include the following definition. It guards against redundancy (defining the same file multiple times)
#define SONGLIST_H

#include "Song.h" //includes the headerfile for class Song so that we can use it in class Songlist

const int SONGLIST_CAPACITY = 400; //sets a const size for the SongList capacity

class SongList
{
public:
	//constructors
	SongList(); //default
	SongList(int initCapacity, const char fileName[]); //with parameters (initCapacty is the initial size of the array, fileName[] is the path of the file to be opened and read.)

													   //destructor
	~SongList();

	//accessor functions
	bool searchArtist(const char artist[], Song& match) const;
	bool searchAlbum(const char album[], Song& match) const;
	int getSize() const;
	void printAll() const;
	void saveSongs(const char fileName[]) const;
	bool get(int index, Song& aSong) const;

	//mutator functions
	void addSong(const Song& aSong);
	void removeSong(const int delIndex, Song& aSong);
	void loadSongs(const char fileName[]);

private:
	Song * list; //a pointer variable of type Song--so this will point to the memory address of list
	int size;
	int capacity;

	void expandList();
};

#endif