//
//  client_ui.cpp
//  Chat Away
//
//  Created by Maliha on 2/5/25.
//

#include <stdio.h>
#include <iostream>
#include <ncurses.h>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <mutex>

WINDOW *outputWin, *inputWin;
std::mutex print_mutex;

void initUI() {
    initscr();
    noecho();
    cbreak();

    int height = LINES - 3;
    outputWin = newwin(height, COLS, 0, 0);
    inputWin = newwin(3, COLS, height, 0);

    scrollok(outputWin, TRUE);
    box(inputWin, 0, 0);
    wrefresh(outputWin);
    wrefresh(inputWin);
}

void printMessage(const std::string &msg) {
    std::lock_guard<std::mutex> lock(print_mutex);
    wprintw(outputWin, "%s\n", msg.c_str());
    wrefresh(outputWin);
}

void receiveMessages(int sock) {
    char buffer[1024];
    while (true) {
        int valread = recv(sock, buffer, 1024, 0);
        if (valread > 0) {
            buffer[valread] = '\0';
            printMessage(buffer);
        }
    }
}

int main() {
    std::string username;
    std::cout << "Enter your username: ";
    std::getline(std::cin, username);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    send(sock, username.c_str(), username.length(), 0);

    initUI();
    printMessage("Welcome to Chat Away!");

    //std::thread receiver(receiveMessages, sock);
    //receiver.detach();
    
    std::thread receiver([sock]() {
        receiveMessages(sock);
    });
    receiver.detach();


    char input[1024];
    while (true) {
        werase(inputWin);
        box(inputWin, 0, 0);
        mvwgetnstr(inputWin, 1, 1, input, 1024);
        send(sock, input, strlen(input), 0);
    }

    endwin();
    close(sock);
    return 0;
}

