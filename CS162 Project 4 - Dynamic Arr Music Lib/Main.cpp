
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <iostream>
#include <cstring>
#include <cctype>

#include "UserInterface.h"
#include "InputTools.h"
#include "Song.h"
#include "SongList.h"



using namespace std;

int main()
{
	//enable memory leak detection tools
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	char command;
	char fileName[] = "songs.txt";
	SongList list(1, fileName); //setting the "initCapacity" parameter to 1 allows us to check to see
								//if the expandList() member function works, because the capacity of our
								//list will definitely exceed 1.

	displayMenu();
	command = readInCommand();
	while (command != 6)
	{
		processCommand(command, list);
		displayMenu();
		command = readInCommand();
	}

	list.saveSongs(fileName);

	return 0;
}