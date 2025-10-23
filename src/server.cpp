#include <winsock2.h>
#include <iostream>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")

void startServer() {
    WSADATA wsaData;
    SOCKET server, client;
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    bind(server, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(server, 3);

    while (true) {
        client = accept(server, (sockaddr*)&clientAddr, &clientSize);
        if (client == INVALID_SOCKET) continue;

        std::ifstream file("../web/index.html");
        std::string html((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + html;
        send(client, response.c_str(), response.size(), 0);
        closesocket(client);
    }

    closesocket(server);
    WSACleanup();
}
