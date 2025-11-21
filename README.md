# Simple HTTP Server with multithreading  (Not Completed)

This is a simple HTTP/1.1 server written in C. It is not fully completed and is primarily intended as a learning project. The goal is to understand the fundamentals of how HTTP servers work, including:

## Key Goals:
- Learn how to handle HTTP requests and responses in C.
- Understand and work with sockets in C for networking.
- Implement basic error handling for missing or invalid requests.
- Explore directory and file management in a server context.
- Add multithreading to handle multiple clients concurrently.
- Gain practical experience with `POSIX` system calls and C standard library functions.

## Features

- Handles **basic HTTP GET requests**.
- Multithreading server.
- Serves static files.
- Dummy HTTP parser.
- Returns standard HTTP status codes:
  - `200 OK` – file found
  - `404 NOT FOUND` – file not found
  - `501 NOT IMPLEMENTED` – method not supported
- Supports nested directories when searching for files.

## To Do
- [x] Add multithreading using POSIX threads
- [ ] Improve error handling and implement security enhancements
- [ ] Prevent directory traversal attacks

## Requirements

- GCC or any C compiler
- Linux or macOS (POSIX compatible)
- Basic knowledge of sockets and HTTP

---

## Project Structure
- `main.c` - Main server loop
- `socket_utilities.c` - Socket initialization and helpers
- `file_utilities.c` - File searching utilities
- `http_utilities.c` - HTTP parsing and response helpers
- `queue_t.h` - header-only implementation of queue for threadpool
- `threadpool_t.h` - header-only implementation of threadpool.
- `pages/` - Directory for HTML pages (index.html, 404.html, 501.html)

## How to Run
1. Clone the repository:
   ```
   git clone https://github.com/notorious1dev/http-server.git
   cd ./http-server
   ./start.sh
   ```
2. Open your browser and navigate to:
  ```
  http://127.0.0.1:8080/
  ```

  <img width="925" height="944" alt="image" src="https://github.com/user-attachments/assets/be716af8-cbd2-4c1e-be8f-68873a50f19c" />
