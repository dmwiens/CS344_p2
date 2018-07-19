/******************************************************************************
* Author:           David Wiens
* Date Created:     07/16/2018
*
* Desc: This program will create the rooms files necessary for a text-based
*           adventure game.
*
******************************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
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
void AssignRoomTypes(struct Room* roomsToType);
void PopulateRoomConnections(struct Room*);
void AddRandomConnection(struct Room* rooms);
void ConnectRooms(struct Room* x, struct Room* y);
int ConnectionExists(struct Room* roomA, struct Room* roomB);
int IsGraphFull(struct Room* rooms);
int CanConnect(struct Room* room);
void MakeRoomFile(struct Room* room, char* dir);
void FreeRoomMem(struct Room* rooms);

int main()
{   
    int i, j, k;
    char dirName[32];
    
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
    

    // Assigne Room Types
    AssignRoomTypes(rooms);

    /*
    // TESTING: Print out room names
    for (i = 0; i < NUM_ROOMS; i++) {
        printf("Room%i is named \"%s\" of type %i\n", i, rooms[i].name, rooms[i].type);
    }
    */

    // Populate network of rooms' connections
    PopulateRoomConnections(rooms);


    /*
    // Testing: create full graph
    ConnectRooms(&rooms[1], &rooms[2]);
    ConnectRooms(&rooms[2], &rooms[3]);
    ConnectRooms(&rooms[3], &rooms[4]);
    ConnectRooms(&rooms[4], &rooms[5]);
    ConnectRooms(&rooms[5], &rooms[6]);
    ConnectRooms(&rooms[6], &rooms[1]);
    ConnectRooms(&rooms[1], &rooms[4]);    
    ConnectRooms(&rooms[2], &rooms[5]);
    ConnectRooms(&rooms[3], &rooms[6]);
    ConnectRooms(&rooms[0], &rooms[1]);    
    ConnectRooms(&rooms[0], &rooms[2]);
    ConnectRooms(&rooms[0], &rooms[3]);
    */


    /*
    // TESTING: Print out room and connections
    for (i = 0; i < NUM_ROOMS; i++) {
        printf("Room%i \"%s\" has %i connections.\n", i, rooms[i].name, rooms[i].numConnections);

        for (j = 0; j < rooms[i].numConnections; j++) {
            printf("  %i \"%s\"\n", j, rooms[i].outboundConnections[j]->name);
        }

    }
    */


    // Testing: Print is graph full
    //printf("Graph Fullness: %i\n", IsGraphFull(rooms));


    // Get current process id
    pid_t pid = getpid();

    // Testing : print process id
    //printf("The process id is: %i\n", pid);


    // Create directory
    // Reference: https://oregonstate.instructure.com/courses/1683586/pages/2-dot-4-manipulating-directories
    sprintf(dirName, "wiensda.rooms.%d", pid);
    int mkdirResult = mkdir(dirName, 0755);


    //Make Room File for each room
    for (i = 0; i < NUM_ROOMS; i++) {
        MakeRoomFile(&rooms[i], dirName);
    }


    // Clean up rooms
    FreeRoomMem(rooms);


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
Name: AssignRoomTypes
Desc: This assigns type. No need to assign randomly since the names are random.
******************************************************************************/
void AssignRoomTypes(struct Room* roomsToType){

    int i;

    // Assign middle types
    for (i = 1; i < NUM_ROOMS - 1; i++) {
        roomsToType[i].type = MID_ROOM;
    }

    // Assign first and last rooms Start and End, respectively
    roomsToType[0].type = START_ROOM;
    roomsToType[NUM_ROOMS - 1].type = END_ROOM;

    return;
}


/******************************************************************************
Name: PopulateRoomConnections
Desc: Given the rooms array and the array of possible names, randomly assign
a unique possible name to each room.
******************************************************************************/
void PopulateRoomConnections(struct Room* roomsToPopulate){

    while (IsGraphFull(roomsToPopulate) == 0) {

        AddRandomConnection(roomsToPopulate);

    }

}


/******************************************************************************
Name: AddRandomConnection
Desc: This program adds a random connection between two rooms.
******************************************************************************/
void AddRandomConnection(struct Room* rooms){

    int aID, bID;
    int success = 0;
    time_t t;


    // initialize randomizer
    // Reference: https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
    srand((unsigned) time(&t));


    while (success == 0) {

        // Generate two random integers (room ids)
        aID = rand() % NUM_ROOMS;
        bID = rand() % NUM_ROOMS;

        

        if ((aID != bID) && CanConnect(&rooms[aID]) && CanConnect(&rooms[bID]) && !ConnectionExists(&rooms[aID], &rooms[bID])) {
            ConnectRooms(&rooms[aID], &rooms[bID]);
            success = 1;
        }

    }

}


/******************************************************************************
Name: CanConnect
Desc: returns true if room can connect to any more rooms
******************************************************************************/
int CanConnect(struct Room* room) {

    return (room->numConnections < 6);

}


/******************************************************************************
Name: ConnectionExists
Desc: returns true if connection exists between the two rooms
******************************************************************************/
int ConnectionExists(struct Room* roomA, struct Room* roomB) {

    int i = 0;

    // If ANY room has less than three connections, return false
    for (i = 0; i < roomA->numConnections; i++) {
        // if any outbound pointers from roomA match roomB, return true
        if (roomA->outboundConnections[i] == roomB) {
            return 1;
        }
    }

    return 0;

}




/******************************************************************************
Name: ConnectRooms
Desc: connects the two rooms together
******************************************************************************/
void ConnectRooms(struct Room* x, struct Room* y) 
{
    int xNextConnID, yNextConnID;

    // Get index of next connection
    xNextConnID = x->numConnections;
    yNextConnID = y->numConnections;

    // Set room pointers to
    x->outboundConnections[xNextConnID] = y;
    y->outboundConnections[yNextConnID] = x;
    
    // Increment count on both
    x->numConnections = x->numConnections + 1;
    y->numConnections = y->numConnections + 1;

}




/******************************************************************************
Name: IsGraphFull
Desc: Returns true if all rooms have 3 to 6 outbound connections, false otherwise
******************************************************************************/
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

}



/******************************************************************************
Name: MakeRoomFile
Desc: Makes a file for each room struct
******************************************************************************/ 
void MakeRoomFile(struct Room* room, char* dir)  
{


    char fileDirAndName[40];
    char contents[200];
    int i, fd;
    

    // Set file name
    sprintf(fileDirAndName, "%s/%s%s", dir, room->name, "_room");


    // Create file
    fd = open(fileDirAndName, O_WRONLY | O_CREAT, 0600);
    if (fd < 0) {
        fprintf(stderr, "Could not open %s\n", fileDirAndName);
        perror("Error in main()");
        exit(1);
    }



    //char contents[] = "Hello and this is my file contents\n and more\n";

    // Generate contents: Room name
    sprintf(contents, "ROOM NAME: %s\n", room->name);


    // Generate contents: Connections
    for (i = 0; i < room->numConnections; i++) {
        sprintf(contents, "%sCONNECTION %d: %s\n", contents, i+1, room->outboundConnections[i]->name);
    }

    // Generate contents: Room Type
    switch (room->type) {
        case START_ROOM:
            sprintf(contents, "%sROOM TYPE: %s\n", contents, "START_ROOM");
            break;
        case MID_ROOM:
            sprintf(contents, "%sROOM TYPE: %s\n", contents, "MID_ROOM");
            break;
        case END_ROOM:
            sprintf(contents, "%sROOM TYPE: %s\n", contents, "END_ROOM");
            break;
        default:
            fprintf(stderr, "Invalid Room Type");
    }


    // Finally, write the contents to the file and close
    write(fd, contents, strlen(contents) * sizeof(char));
    close(fd);


    return;

}





/******************************************************************************
Name: FreeRoomMem
Desc: Makes 
******************************************************************************/ 
void FreeRoomMem(struct Room* rooms)
{
    int i;

    for (i = 0; i < NUM_ROOMS; i++) {
        // Free room name
        free(rooms[i].name);
    }

}