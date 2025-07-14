# LAN Chat App 💬

A simple multi-user chat application built in C++ using socket programming. Users on the same local network (LAN) can communicate in real-time via terminal or a text-based UI using `ncurses`.

## Features

* 👥 Multi-user chat support over LAN
* 🔁 Real-time message broadcasting
* 🔐 Username-based identification
* 🧵 Threaded client handling on server
* 🎨 UI client using `ncurses` (scrollable chat + input box)
* 🧊 Clean modular code

## Project Structure

```
.
├── server.cpp           # Server to accept and manage clients
├── client.cpp           # Basic terminal-based client
├── client_ui.cpp        # Ncurses-based UI client
├── README.md            # Project overview
```

## Requirements

* C++11 or later
* `ncurses` (for `client_ui.cpp`)
* A POSIX-compliant OS (macOS/Linux)

### 🛠 Install `ncurses` if not available

**macOS:**

```bash
brew install ncurses
```

**Linux (Debian/Ubuntu):**

```bash
sudo apt install libncurses-dev
```

## 🧪 Build & Run

### 🔧 Compile

#### 1. Server

```bash
g++ server.cpp -o server -pthread
```

#### 2. Terminal Client

```bash
g++ client.cpp -o client -pthread
```

#### 3. UI Client (Ncurses)

```bash
g++ client_ui.cpp -o client_ui -lncurses -pthread -std=c++11
```

### 🚀 Run

#### 1. Start the server

```bash
./server
```
#### 2. Run one or more clients (in different terminals or machines)

```bash
./client_ui      # UI version
# or
./client         # Simple terminal version
```

🔹 The server listens on port `8080`. All clients should connect to the server’s IP address.

## LAN Setup (Optional)

To chat across devices:

* Replace `127.0.0.1` in `client.cpp` / `client_ui.cpp` with the **server's LAN IP** (e.g., `192.168.1.10`)
* Make sure firewall allows connections on port `8080`

## 📜 License

MIT License
