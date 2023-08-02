# Simple HTTP Server

This is a simple HTTP server written in C that serves static files from the current directory. It handles GET requests and responds with the appropriate HTTP status code along with the requested file's content if found. The server listens on port 8080 by default.

## Getting Started

To run the server, simply compile the `http_server.c` file and execute the resulting binary. The server will start and display the message:

```
-------------------------------------------------------------

              SIMPLE HTTP SERVER Prototype 0.0.1             
                                                            
-------------------------------------------------------------
 * Server initiated at {current_time}
 * Listening on port 8080 ...
 * Browse to http://127.0.0.1:8080 to access the server
 * POWERED BY: Kamau Ngengi Enterprises
```

You can access the server by opening a web browser and navigating to `http://127.0.0.1:8080`.

## Prototypes

### Prototype 1: Basic Functionality

The first prototype of the HTTP server provides basic functionality. It includes the following features:

- **Date and Time**: The server displays the current date and time in the console when it starts.
- **Static File Serving**: It serves static files (HTML, text, images, etc.) from the current directory based on the requested URL.
- **HTTP Response**: The server responds to valid GET requests with the appropriate HTTP status code (200 OK) along with the requested file's content if found. If the requested file does not exist, it returns a 404 Not Found response.

### Usage

1. Compile the `http_server.c` file: `gcc http_server.c -o http_server -pthread`
2. Run the server: `./http_server`

### Next Steps

For future prototypes, additional features can be added, such as:

- **Dynamic Content Generation**: Introduce server-side scripting to generate dynamic content based on user requests.
- **Security Enhancements**: Implement security measures to prevent common web vulnerabilities like cross-site scripting (XSS) and SQL injection.
- **Logging**: Add logging functionality to keep track of server activities and user requests.
- **Configuration File**: Allow server configuration through a separate configuration file.
- **HTTPS Support**: Enable HTTPS support to secure data transmission between the server and clients.
- **Concurrency**: Improve the server's performance by supporting concurrent connections.

## Prototype 1: File Description

### `http_server.c`

This is the main file for the HTTP server prototype. It includes the necessary headers, such as `arpa/inet.h`, `dirent.h`, `fcntl.h`, `netinet/in.h`, `pthread.h`, `regex.h`, `stdbool.h`, `stdio.h`, `stdlib.h`, `string.h`, `sys/socket.h`, `sys/stat.h`, `sys/types.h`, `unistd.h`, and `time.h`.

The file defines various utility functions to handle time, file extensions, MIME types, case-insensitive string comparison, URL decoding, and building HTTP responses. The server listens on port 8080 by default and handles client connections in separate threads.

The `main` function initializes the server socket, binds it to the specified port, and starts listening for incoming connections. Once a client connects, a new thread is created to handle the client's request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
