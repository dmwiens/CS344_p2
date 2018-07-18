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
#include <stdlib.h>

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
void SelectRoomNames(struct Room*, const char**);
void PopulateRoomConnections(struct Room*);
void ConnectRooms(struct Room* x, struct Room* y);
//int IsGraphFull(struct Room* rooms);

int main()
{   
    int i, j, k;

    
    // Declare and set up (hardcode) 10 room names
    // Reference 1: https://stackoverflow.com/questions/1088622/how-do-i-create-an-array-of-strings-in-c
    // Reference 2: https://stackoverflow.com/questions/13501579/2d-array-using-strings
    const char* possibleRoomNames[NUM_POSSIBLE_ROOM_NAMES];

    // Set values for possible room names
    possibleRoomNames[0] = "Dungeon";
    possibleRoomNames[1] = "Kitchen";
    possibleRoomNames[2] = "Armory";
    possibleRoomNames[3] = "Pantry";
    possibleRoomNames[4] = "Loo";
    possibleRoomNames[5] = "Hallway";
    possibleRoomNames[6] = "Stairs";
    possibleRoomNames[7] = "Foyer";
    possibleRoomNames[8] = "Lobby";
    possibleRoomNames[9] = "Pit";

    
    // Declare the rooms
    struct Room rooms[NUM_ROOMS]; 

    // Initialize room structs
    for (i = 0; i < NUM_ROOMS; i++) {
        // Allocate memory for room names
        rooms[i].name = calloc(MAX_ROOM_NAME_LENGTH, sizeof(char));

        // Initialize connections
        rooms[i].numConnections = 0;
    }

    // Select room names from list of 10 randomly to populate the 7 rooms
    SelectRoomNames(rooms, possibleRoomNames);
    

    // TESTING: Print out room names
    for (i = 0; i < NUM_ROOMS; i++) {
        printf("Room%i is named \"%s\"\n", i, rooms[i].name);
    }


    // Populate network of rooms' connections
    //PopulateRoomConnections(rooms);


    ConnectRooms(&rooms[1], &rooms[2]);
    ConnectRooms(&rooms[1], &rooms[3]);
    ConnectRooms(&rooms[1], &rooms[6]);


    // TESTING: Print out room and connections
    for (i = 0; i < NUM_ROOMS; i++) {
        printf("Room%i \"%s\" has %i connections.\n", i, rooms[i].name, rooms[i].numConnections);

        for (j = 0; j < rooms[i].numConnections; j++) {
            printf("  %i \"%s\"\n", j, rooms[i].outboundConnections[j]->name);
        }

    }


    return 0;

}


/******************************************************************************
Name: SelectRoomNames
Desc: Given the rooms array and the array of possible names, randomly assign
a unique possible name to each room.
******************************************************************************/
void SelectRoomNames(struct Room* roomsToAssign, const char** possibleNames){

    int nameUsed[NUM_POSSIBLE_ROOM_NAMES] = {0};
    int i, j, valAssigned = 0;
    time_t t;


    // initialize randomizer
    // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
    srand((unsigned) time(&t));

    // For each room, get a random index of a possible room.
    // Only assign it and move on if the possible room name is not yet used.
    for (i = 0; i < NUM_ROOMS; i++) {
        
        //printf("Assigning name to room %i\n", i);

        valAssigned = 0;
        while (valAssigned==0){

            j = rand() % NUM_POSSIBLE_ROOM_NAMES;

            // Only copy if name is not used
            if (nameUsed[j] == 0){
                
                // Copy from possible names array to iterated room
                strcpy(roomsToAssign[i].name, possibleNames[j]);

                // Set flags to continue loops
                nameUsed[j] = 1;
                valAssigned = 1;

                //printf("name assigned. %i %i \n", i, j);
            } else {
                ;
                //printf("j already used. %i %i \n", i, j);
            }

        }
        
    }


    return;
}




/******************************************************************************
Name: PopulateRoomConnections
Desc: Given the rooms array and the array of possible names, randomly assign
a unique possible name to each room.
******************************************************************************/
void PopulateRoomConnections(struct Room* roomsToPopulate){

    ;


}


// Connects Rooms x and y together, does not check if this connection is valid
void ConnectRooms(struct Room* x, struct Room* y) 
{
    int xNextConnID, yNextConnID;

    // Get index of next connection
    xNextConnID = x->numConnections;
    yNextConnID = y->numConnections;

    // Allocate space for new Room pointer
    x->outboundConnections[xNextConnID] = malloc(sizeof(struct Room*));
    y->outboundConnections[yNextConnID] = malloc(sizeof(struct Room*));

    // Set room pointers to
    x->outboundConnections[xNextConnID] = y;
    y->outboundConnections[yNextConnID] = x;
    
    // Increment count on both
    x->numConnections = x->numConnections + 1;
    y->numConnections = y->numConnections + 1;

}

/*
// Returns true if all rooms have 3 to 6 outbound connections, false otherwise
int IsGraphFull(struct Room* rooms)  
{
    int i = 0;

    // If ANY room has less than three connections, return false
    for (i = 0; i < NUM_ROOMS; i++) {
        if (rooms[i].numConnections < 3) {
            return 0;
        }
    }

    return 1;

}*/