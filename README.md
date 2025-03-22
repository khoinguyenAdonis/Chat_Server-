## Chat Server - Multi-Client Messaging System
### Introduce 

The Chat Server is a multi-client messaging system built in C, utilizing sockets, multithreading (pthread), and synchronization (mutex). It allows multiple clients to connect to a server and send private messages using the following format:

###  Features

Supports multiple clients simultaneously.(10 clients)
Private messaging between clients.
The server manages the client list using mutex to avoid data conflicts.

### How to run the application 
    
**Use makefile to build both client and server**
    
    make : Builds both the client and server. Generates executable files in the build/ directory.

    make SERVER : Builds the server executable file.

    make CLIENT : Builds the client executable file.

    make clean : Removes the build/ folder.
    
**Start the server**
    
    ./build/server <port>

**Start the client**

    ./build/client <server_ip> <port>

### Instruction for use 

Firstly, running the server before running the client.

When running the client, the user must enter a username before starting chat.
    
    Enter user name: username

**Message format**

Send a private message

    @username message?

    @username → Specifies the recipient’s username.
    message → The actual message content you want to send.

If the format is incorrect, the output will be:

     Warrning Invalid format! Use: @username message

If the recipient client does not exist on the server, the output will be:

    The client recipient’s does not exist on the server

### Technologies used
🛠 Programming Language: C

🛠 Socket API: BSD Sockets

🛠 Multithreading: pthread

🛠 Synchronization: Mutex

### 📌 Notes
🔹 The server only stores client data temporarily in memory, without persistent storage.

🔹 Always shut down the server first to prevent socket binding issues.