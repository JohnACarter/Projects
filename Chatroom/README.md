/* CSci4061 S2016 Assignment 2
* date: 03/11/2016
* name: John Carter, James Anthony Bruno
*/

1. Purpose of the Program
- The purpose of this program is to allow the user to run a "local" chat application which allows several different parties/users to communicate among one another. The program has a central server administrator to manage the chat and enables users to "log on."

2. How to Compile the Program
- To compile to program, enter the directory containing all necessary files for the program. Once in the directory, type "make" and the program will compile. To start the program, type in "./server" 

3. How to Use the Program from the Shell (Syntax)
- After compiling the program, type in "./server" to start the program. 
  - Server Commands
    - "\add <username>" will add a user called _username_ to the chat and generate an x-term window for the user to broadcast and send personal messages.
    - "\list will list all the users currently in the chat
    - "\kick <username>" will remove _username_ from the chat
    - "\exit" will terminate all user sessions and close the chat server
    - Any other text will broadcast _any-other-text_ to all user windows. If there are no users, then it will do nothing.
    
  - User Commands (only in xterm windows)
    - "\list" will list all users currently in the chat
    - "\exit" will log of the user closing the xterm chat window
    - "\p2p <recipient><msg> will send the intended _recipient_ the _msg_. 
    - "\seg" will similarly cause a user to exit out yet maintain the entire chat.
    - Any other text will broadcast _any-other-text_ to all other chat users

4. What Exactly the Program Does

  -This program essentially creates the main parent process (Server process) that has a child process called the Shell process which serves as the interface for the server administrator fo the chat. The Shell process in turn will have zero or more child processes that serve as the users of the chat server. When adding users, the Shell process will open a separate xterm window and run an instance of the Shell process as the chat user interface.

5. Any Explicit Assumptions Made

  - One explicit assumption made was that this program allows "communication" on the same operating system; in other words, the program does not allow communication across multiple different operating systems.

6. Strategies for Error Handling
  - The general strategy for error handling was to check that any system calls (such as close, read, write, pipe, fnctl, fork, or wait) don't return -1. If -1 returned from any of these calls, an error message would be printed out to stdout depending on what went wrong.

