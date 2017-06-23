/*CSci 4061 S2016 Assignment 3
*name: John Carter, James Anthony Bruno
*/

- How to Compile the Program
  - To compile the program, enter the directory containing all necessary files for the program. Once in the directory, type "make" and the program will compile. To run the program, type in "./application" followed by six arguments. The arguments should be in the following corresponding order: Process Name, Mailbox Key, Window Size, Maximum Delay, Timeout, and Drop Rate (in percent). So for example typing "./application B 1 2 3 4 10" will start the application with the process name "B", key = 1, window size = 2, maximum delay in sending an acknowledgement = 3, a maximum wait time to resend a packet = 4, and the probability the packet wil be lost = 20%.
  
- How the Program Works
  - The main idea behind the program is a message queue that contains all the necessary information including the pid, process name, etc. which will allow communcation with the other processes. The other processses *send_message* and *receive_message* do exactly as the name implies where it will either send or receive the message.

- What is the effect of having a very short TIMEOUT period and a very long TIMEOUT period?
  - Generally, the effect of a short TIMEOUT period is that the program will have to continually send a message until the message becomes acknowledged once it times out or in the case of this assignment, the MAX_TIMEOUT is reached. In the case of a very long TIMEOUT period, the process will take longer since a message will have a long timeout period between message sending attempts and will reach MAX_TIMEOUT after exponentially longer time.

- What is the effect of having a very small window size (e.g., n=1) and a very large window size?
  - One of the obvious problems with a large window size occurs when the packets are received out of the intended order. In addition, having a large window size could mean having to resend more data when the packets fail to be acknowledged. On the flip side, a small window size would require more overall time if there needs to be multiple windows being sent since an entire window would need to finish before the next window could start. 
