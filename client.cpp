//
//  client.cpp
//  Chat Away
//
//  Created by Maliha on 2/5/25.
//

#include <stdio.h>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8080

void receiveMessages(int sock) {
    char buffer[1024];
    while (true) {
        int valread = recv(sock, buffer, 1024, 0);
        if (valread > 0) {
            buffer[valread] = '\0';
            std::cout << "\n[Chat] " << buffer << "\n> ";
            std::cout.flush();
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string name;

    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    
    send(sock, name.c_str(), name.length(), 0);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr); // or LAN IP

    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    std::thread(receiveMessages, sock).detach();

    std::string message;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, message);
        message = name + ": " + message;
        send(sock, message.c_str(), message.length(), 0);
    }

    close(sock);
    return 0;
}
