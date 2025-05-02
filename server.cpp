//
//  server.cpp
//  Chat Away
//
//  Created by Maliha on 2/5/25.
//

#include <stdio.h>

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <mutex>
#include <map>

#define PORT 8080

std::vector<int> clients;
std::mutex client_mutex;
std::map<int, std::string> clientUsernames;


void broadcastMessage(const std::string& msg, int sender) {
    std::lock_guard<std::mutex> lock(client_mutex);
    for (int client : clients) {
        if (client != sender) {
            send(client, msg.c_str(), msg.size(), 0);
        }
    }
}

void handleClient(int clientSocket) {
    char buffer[1024];

    // get username first
    int valread = recv(clientSocket, buffer, 1024, 0);
    if (valread <= 0) return;
    buffer[valread] = '\0';
    
    std::string input(buffer);
    std::string username, message;
    std::stringstream ss(input);
    std::getline(ss, username, ':');   // read up to :
    std::getline(ss, message);         // read the rest

    {
        std::lock_guard<std::mutex> lock(client_mutex);
        clientUsernames[clientSocket] = username;
    }

    std::cout << username << " joined.\n";
    std::cout << input << "\n";
    broadcastMessage("[Server] " + username + " joined the chat\n", clientSocket);

    // handle regular messages
    while (true) {
        valread = recv(clientSocket, buffer, 1024, 0);
        if (valread <= 0) break;

        buffer[valread] = '\0';
        std::string msg(buffer);
        //std::string fullMsg = username + ": " + msg;
        std::cout << msg << "\n";
        broadcastMessage(msg, clientSocket);
    }

    // handle disconnect
    close(clientSocket);
    {
        std::lock_guard<std::mutex> lock(client_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
        broadcastMessage("[Server] " + username + " left the chat\n", clientSocket);
        clientUsernames.erase(clientSocket);
    }
}


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);
    std::cout << "Server started on port " << PORT << "\n";

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            clients.push_back(new_socket);
        }
        std::thread(handleClient, new_socket).detach();
    }

    return 0;
}

