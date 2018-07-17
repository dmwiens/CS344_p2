/******************************************************************************
* Author:           David Wiens
* Date Created:     07/16/2018
*
* Desc: This program will create the rooms files necessary for a text-based
*           adventure game.
*
******************************************************************************/


#include <string.h>
#include <stdio.h>

#define MAX_ROOM_NAME_LENGTH 8
#define NUM_POSSIBLE_ROOM_NAMES 10
#define NUM_ROOMS 7
#define MAX_OUTBOUND_CONNECTIONS 6

// Declare room type enumerator
enum ROOM_TYPE {START_ROOM, MID_ROOM, END_ROOM};

// Declare room information struct
struct Room {
    char* name;
    enum ROOM_TYPE type;
    struct Room* outboundConnections[MAX_OUTBOUND_CONNECTIONS];
    int numConnections;
};


// Function Prototypes
void SelectRoomNames(struct Room*, char**);


int main()
{
    
    // Declare and set up (hardcode) 10 room names
    // Reference 1: https://stackoverflow.com/questions/1088622/how-do-i-create-an-array-of-strings-in-c
    // Reference 2: https://stackoverflow.com/questions/13501579/2d-array-using-strings
    char possibleRoomNames[NUM_POSSIBLE_ROOM_NAMES][MAX_ROOM_NAME_LENGTH];

    // Set values for possible room names
    strcpy(possibleRoomNames[0], "Dungeon");
    strcpy(possibleRoomNames[1], "Kitchen");
    strcpy(possibleRoomNames[2], "Armory");
    strcpy(possibleRoomNames[3], "Pantry");
    strcpy(possibleRoomNames[4], "Loo");
    strcpy(possibleRoomNames[5], "Hallway");
    strcpy(possibleRoomNames[6], "Stairs");
    strcpy(possibleRoomNames[7], "Foyer");
    strcpy(possibleRoomNames[8], "Lobby");
    strcpy(possibleRoomNames[9], "Pit");

    
    // Declare the rooms
    struct Room rooms[NUM_ROOMS]; 

    int i;
    for (i = 0; i < NUM_ROOMS; i++) {
        rooms[i].name = calloc(8, sizeof(char));
        strcpy(rooms[i].name, possibleRoomNames[i]);
    }

    // Select room names from list of 10 randomly to populate the 7 rooms
    //void SelectRoomNames(rooms, possibleRoomNames);
    
    /*
    // Test hard code room names
    strcpy(&rooms[0].name, "Dungeon");
    strcpy(&rooms[1].name, "Kitchen");
    strcpy(&rooms[2].name, "Armory");
    strcpy(&rooms[3].name, "Pantry");
    strcpy(&rooms[4].name, "Loo");
    strcpy(&rooms[5].name, "Hallway");
    strcpy(&rooms[6].name, "Stairs");
    */

    


    // test room names

    for (i = 0; i < NUM_ROOMS; i++) {
        printf("Room%i is named \"%s\"\n", i, rooms[i].name);
    }








    return 0;

}


void SelectRoomNames(struct Room* roomsToAssign, char** possibleNames){

    int nameUsed[NUM_POSSIBLE_ROOM_NAMES];
    int i;

    for (i = 0; i < NUM_ROOMS; i++) {

        strcpy(&roomsToAssign[i].name, possibleNames[i]);

    }


    return;
}
