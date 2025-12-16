# Secure-Messaging-System-with-AES-Encryption-and-Database-Integration

## Requirements:

1. C++ Compiler: Ensure you have a compiler supporting C++17 (e.g., g++).
2. Libraries: Install the required libraries:
   sudo apt update
   sudo apt install libssl-dev libsqlite3-dev
3. Make Utility: Ensure make is installed:
   sudo apt install make

## Build Instructions:

1. Navigate to the project directory:
   cd /path/to/secure_messaging
2. Build the project:
   make
3. Clean build files (optional):
   make clean

## Running the Program:

### Start the Server:
1. Run the server:
   ./server

### Send a Message from the Client:
1. Create a file with the message (e.g., message.txt).
2. Run the client to send the message:
   ./client message.txt
