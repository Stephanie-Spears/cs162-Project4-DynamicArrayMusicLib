/*
Implementation file for class Songlist
*/

#include "SongList.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

/*
Default constructor. It initializes the list to be empty.
*/
SongList::SongList()
{
	capacity = SONGLIST_CAPACITY; //initializing capacity using a constant variable
	list = new Song[capacity]; //sets the SongList member var pointer "list" equal to the base address of array Song[capacity]
	size = 0; //set size to 0, so we know there are no elements in the array we've just created

			  /*	the operator "new" allocates memory(a variable) of the designated type and returns a pointer to it--
			  meaning it returns the address of this allocated memory.The allocated memory is uninitialized.
			  */
}

/*
Constructor that initializes the list with data from the passed in file.
in: fileName
*/
SongList::SongList(int initCapacity, const char fileName[]) //we pass in initCapacity so we can give the array a capacity size of our choosing (which we define in the parameters of main)
{
	capacity = initCapacity;
	list = new Song[capacity];
	size = 0;
	loadSongs(fileName); //accesses the SongList class member function loadSongs with the fileName parameter
}

/*
Destructor. It releases the memory allocated for the array.
*/
SongList::~SongList() //we set this destructor so that when we finish with our array, it will deallocate the memory the array occupied, so we don't get any memory leaks
{
	if (list != NULL) //if list isn't empty, delete (deallocate) list contents.
		delete[] list;

}

/* Reads the entries from the file and adds them to the list.
in: fileName
*/
void SongList::loadSongs(const char fileName[])
{
	ifstream in; //filename variable
	char title[MAX_CHAR]; //declares local variables for us to store the contents of the file into
	char artist[MAX_CHAR];
	char duration[MAX_CHAR];
	char album[MAX_CHAR];

	Song aSong; //declares a variable of type Song (so it has access to all member objects of class Song)

	in.open(fileName);
	if (!in)
	{
		in.clear();
		cerr << endl << "Failed to open filename '" << fileName << "' for input!" << endl << endl;
		exit(1);
	}

	in.getline(title, MAX_CHAR, ';'); //stores the contents of filename "in" into local variable "title," up until the deliminator ';' which tells the getline to stop reading.
	aSong.setTitle(title); //Takes the line (that we just read from "in" file and stored into our local "title" variable), and passes it as a parameter to the Song member function "setTitle()"
						   /*remember, setTitle checks to see if Song member var is empty, if not, empties it, then sets the member pointer this->title = new char[strlen(title) +1] .. so it allocates a memory space just big enough to hold the passed in "title" length (+1 to be null-terminated)
						   and then it copies the contents of the passed in "title" to the member var "this->title" -- strcpy(this->title, title);
						   */
	while (!in.eof()) //while there's more to be read from the input file, the above process repeats for each encapsulated element of the Song class 
	{

		in.getline(artist, MAX_CHAR, ';');
		aSong.setArtist(artist);
		in.getline(duration, MAX_CHAR, ';');
		aSong.setDuration(duration);
		in.getline(album, MAX_CHAR, '\n'); //album is the final object to be read from the file, so it's not separated by a ';', and is instead separated by a newline character.
		aSong.setAlbum(album);

		addSong(aSong); //once all elements of a single song have been set, this runs the function addSong
						/*SongList::addSong(const Song &aSong) -- it passes our local variable 'aSong' as a reference.
						Normally passing by reference (&) means it's treated as if it were passing the variable itself (whereas passing by value copies the contents and can be time-consuming)
						but by using "const Song &aSong" the function can't change the value.
						*NOTE: you CAN'T pass a const argument to a non-const reference parameter, so using const parameters ensures you can pass both non-const AND const arguments to the function.
						-when passing an argument by reference, always use a const reference unless you need to change the value of the argument.

						SongList::addSong(const Song &aSong) function first sees if the size of our array has hit the capacity limit, and if it has, it runs expandList();
						-Next, it takes the passed in const parameter "aSong" and runs the Song::getTitle(title) function, where "title" is a local variable inside addSong.
						-this sets the local variable "title" to the value stored in the member function "this->title"
						-Next, it takes SongList member pointer (of type Song) "list" and runs: list[size].setTitle(title);
						-this takes list[size] and runs the function setTitle(title).
						*remember that setTitle(title) first checks if the member var this->title is empty. then it allocates a new memory address that's the array size of the local var "title" +1 (null-terminated), and then copies local var "title" into member var "this->title"
						FINALLY addSong(aSong) increments "size". size++.
						*/
		in.getline(title, MAX_CHAR, ';');		//start the next record
		aSong.setTitle(title);
	}
	in.close();
}

/*
This function returns the size of the list.
return: size
*/
int SongList::getSize() const
{
	return size;
}


/* Searches the list for an entry with passed in artist.
in: artist
out: match
return: true if a match is found and match will be returned through parameter.
false if a match can not be found and match contains garbage.
*/
bool SongList::searchArtist(const char artist[], Song &match) const
{
	int index;
	char currentTitle[MAX_CHAR];
	char currentArtist[MAX_CHAR];
	char currentDuration[MAX_CHAR];
	char currentAlbum[MAX_CHAR];

	for (index = 0; index < size; index++) //this runs through our populated list
	{
		list[index].getTitle(currentTitle);//Makes list[index] access getTitle(currentTitle), which copies the member "this->title" into "title" (locally declared as "currentTitle") 
		list[index].getArtist(currentArtist);
		list[index].getDuration(currentDuration);
		list[index].getAlbum(currentAlbum);

		if (strcmp(artist, currentArtist) == 0) //checks to see if the passed in "artist" from the user is equal to our local variable "currentArtist", which is being cycled through list[index]
		{ //if the two c strings are equal, we use our parameter Song &match (reference means it's directly accessible, like passing by value--except the actual values don't have to be copied, thus saving time).
		  //We use our Song object "match" and run the setTitle(currentTitle) function.
		  //sets match's member var "this->title" to the passed in "currentTitle"
			match.setTitle(currentTitle);
			match.setArtist(currentArtist);
			match.setDuration(currentDuration);
			match.setAlbum(currentAlbum);
			cout << endl << currentArtist << endl << currentTitle << endl << currentDuration << endl << currentAlbum << endl << endl;
			//should change this section so that it loops in main, not cout'ing in this function...bad form?
		}
	}
	if (index == size)
	{
		return false;
	}
	else
	{
		return true;
	}
}



/* This function searches the list for an entry with passed in album.
in: album
out: match
return: true if a match is found and match will be returned through parameter.
false if a match can not be found and match contains garbage.
*/
bool SongList::searchAlbum(const char album[], Song &match) const
{
	int index;
	char currentTitle[MAX_CHAR];
	char currentArtist[MAX_CHAR];
	char currentDuration[MAX_CHAR];
	char currentAlbum[MAX_CHAR];

	for (index = 0; index<size; index++)
	{
		list[index].getTitle(currentTitle); //for SongList Song Pointer member [at position "index"] stores value from getTitle ("this->title") into currentTitle
		list[index].getArtist(currentArtist);
		list[index].getDuration(currentDuration);
		list[index].getAlbum(currentAlbum);

		if (strcmp(album, currentAlbum) == 0)
		{
			match.setTitle(currentTitle);
			match.setArtist(currentArtist);
			match.setDuration(currentDuration);
			match.setAlbum(currentAlbum);
			cout << endl << currentTitle << endl << currentArtist << endl << currentDuration << endl << currentAlbum << endl << endl;
			//should change so it loops and cout's outside the function
		}
	}
	if (index == size)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/*
Prints all the entries to the standard output.
*/
void SongList::printAll() const
{
	int index;
	char title[MAX_CHAR];
	char artist[MAX_CHAR];
	char duration[MAX_CHAR];
	char album[MAX_CHAR];

	for (index = 0; index<size; index++)
	{
		list[index].getTitle(title);
		list[index].getArtist(artist);
		list[index].getDuration(duration);
		list[index].getAlbum(album);
		cout << "Title: " << title << endl << "Artist: " << artist << endl << "Duration: " << duration << endl << "Album: " << album << endl << endl;
	}
}

/*
Saves the list to the external file.
in: fileName
*/
void SongList::saveSongs(const char fileName[]) const
{
	ofstream out;
	int index;
	char title[MAX_CHAR];
	char artist[MAX_CHAR];
	char duration[MAX_CHAR];
	char album[MAX_CHAR];

	out.open(fileName);
	if (!out)
	{
		out.clear();
		cerr << endl << "Failed to open filename '" << fileName << "' for output." << endl << endl;
		exit(1);
	}

	for (index = 0; index < size; index++)
	{
		list[index].getTitle(title);
		list[index].getArtist(artist);
		list[index].getDuration(duration);
		list[index].getAlbum(album);

		out << title << ';' << artist << ';' << duration << ';' << album << endl; //Basically what we're pushing to the file "out"
	}

	out.close();
}

/* This function adds a passed in entry to the end of the list.
in: anEntry
*/
void SongList::addSong(const Song &aSong)
{
	char title[MAX_CHAR];
	char artist[MAX_CHAR];
	char duration[MAX_CHAR];
	char album[MAX_CHAR];


	if (size == capacity)
	{//this is where we "fake" the growing array size
	 //by using the expandList() function, we're able to put our current array
	 //into a temporary array, find the array size that we actually need, and copy
	 //our temporary arry back into our actual array.
		expandList();
	}
	aSong.getTitle(title); //taking what's already set in "this->title" and copying it into local variable "title"
	aSong.getArtist(artist);
	aSong.getDuration(duration);
	aSong.getAlbum(album);
	list[size].setTitle(title); //for list[size], setTitle erases the previous "this->title" value, and uses the passed in parameter "title" to copy it into "this->title" ...? is this right? 
	list[size].setArtist(artist);
	list[size].setDuration(duration);
	list[size].setAlbum(album);

	size++; //grows the size because we've added one more song
}


/* This function removes a passed in song from the list.
in: song ... is this part right?
*/
void SongList::removeSong(const int delIndex, Song &aSong) //why do I need &aSong here?
{

	char temp[MAX_CHAR];

	if (delIndex<0 || delIndex >= size)
	{ //this is where I could use the "get" function at the bottom.
		cout << "Invalid index number. " << endl;
		exit(1);
	}
	else
	{
		for (int i = delIndex; i < size - 1; i++)
		{
			list[i + 1].getTitle(temp); //for list at pos+1 run getTitle and store "this->title" into our local var "temp"
			list[i].setTitle(temp); //for list at pos, run setTitle and stores our "temp" value into "this->title"

			list[i + 1].getArtist(temp); //am i envisioning this right?
			list[i].setArtist(temp);

			list[i + 1].getDuration(temp);
			list[i].setDuration(temp);

			list[i + 1].getAlbum(temp);
			list[i].setAlbum(temp);
		}
		size--;
	}
}

/* This function creates a new array that is twice as big as the current one. It copies the Song
objects over to this new array.
making the array twice as big is a middle ground of efficiency. more efficient than
changing the array size every time a new object is added, and doesn't take up a
massive, unnecessary amount of memory (ie making the array size enormous and leaving most of it empty)
*/
void SongList::expandList()
{
	Song * tempList;
	char title[MAX_CHAR];
	char artist[MAX_CHAR];
	char duration[MAX_CHAR];
	char album[MAX_CHAR];
	int index;

	//create new array
	capacity *= 2; //here we double the capacity
	tempList = new Song[capacity]; //here we create a temporary array with the doubled capacity size

								   //copy over the contents
	for (index = 0; index<size; index++) //here we copy over the contents of the original array
	{
		list[index].getTitle(title);
		list[index].getArtist(artist);
		list[index].getDuration(duration);
		list[index].getAlbum(album);
		tempList[index].setTitle(title);
		tempList[index].setArtist(artist);
		tempList[index].setDuration(duration);
		tempList[index].setAlbum(album);
	}

	//release the memory from the old array
	delete[] list; //here we delete the contents of the original array (so we don't get a memory leak)

				   //make list point to the new array
	list = tempList; //now we make the list point to this temp list
	tempList = NULL;
	//with a static array, you can't change the pointer like this, because
	//static array name is a constant pointer, so it can't point to anywhere else
	//but since this is a dynamic array, we can change where the pointer points to.
	//Since list already has the templist address copied, we can get rid of tempList's copy of the address by setting it to null (effectively emptying it).
}


/*
DON'T NEED THIS FUNCTION, UNLESS I WANT TO USE IT FOR MY REMOVE OR INSIDE THE CASE MENU
*/
/* This function returns the Song object at the specified position through parameter
in: index
out: aSong
return: true if the entry exists, false if illegal index is passed in
*/

bool SongList::get(int index, Song &aSong) const
{
char title[MAX_CHAR];
char artist[MAX_CHAR];
char duration[MAX_CHAR];
char album[MAX_CHAR];

if (index < 0 || index >= size)
{
return false;
}

list[index].getArtist(artist);
list[index].getDuration(duration);
list[index].getAlbum(album);
list[index].getTitle(title);

aSong.setArtist(artist);
aSong.setDuration(duration);
aSong.setAlbum(album);
aSong.setTitle(title);
return true;
}
