/* CSci 4061 S2016 Assignment 4
 * name: John Carter, James Anthony Bruno
 */

## Compilation

To compile the program run `make` from the command line from within the "web-server" directory.

## Running

To run the program, run `./web_server PORT PATH NUM_DISPATCHERS NUM_WORKERS QUEUE_LENGTH CACHE_SIZE`. All arguments are required and are explained below.

- **PORT** is the port number to run your server on. It should be within the range 1025 through 65535.
- **PATH** is the root directory of your server. All requests will be relative to this path. The path must be a valid directory.
- **NUM_DISPATCHERS** is the amount of threads that receive requests. This needs to be at least 1 and at most 100.
- **NUM_WORKERS** is the amount of threads that will process and return requests. This needs to be at least 1 and at most 100.
- **QUEUE_LENGTH** is how many requests can be held in a queue at a time. If the queue is full, the dispatcher thread will block until there is an empty slot. This needs to be at least 1 and at most 100.
- **CACHE_SIZE** is how many slots of cache to use. This needs to be at least 0 (in which case the cache is not used) and at most 100.

## Program workflow

### Main thread

- Receives and checks arguments to make sure they are valid.
  - If they aren't, returns an error message and exits.
- Creates a logfile called "web_server_log" within the same directory as the compiled web_server executable.
- Changes directory to the provided root path.
- Creates the queue and cache.
- Creates the dispatcher and worker threads.
- Joins the dispatcher and worker threads.

### Dispatcher thread

- Retrieves thread ID.
- Infinite loop:
  - Waits until a connection is received by the `accept_connection()` method.
    - If there was an error with the connection, closes the dispatcher.
      - If there are no more dispatchers left, notifies the workers.
  - Receives request path from the socket.
  - Places request in request queue.
    - If the queue is full, waits until a slot is open.

### Worker thread

- Retrieves thread ID.
- Infinite loop:
  - Retrieves request from request queue.
    - If the queue is empty, waits until there's an available request.
    - If all dispatchers have been removed and there's no more requests to work on, exits thread.
  - If the request has been cached already:
    - If the file is now inaccessible, marks the cache slot as empty, and returns and logs an error. Continues to next loop iteration.
    - If the file has been modified since the last check (this is considered a MISS):
      - Retrieves new file contents and size.
      - Marks the old cache slot as empty.
      - Places the new cache slot in cache.
      - Returns the file to the requester.
    - Otherwise (this is considered a HIT):
      - Retrieves all the file information from the cache.
      - Returns the file to the requester.
  - Otherwise (this is considered a MISS):
    - Retrieve the file contents, size, and mime type.
    - Places the file in cache.
    - Returns the file to the requester.

### Cache logic

Slots are replaced using the following logic:

- In an empty slot.
- Otherwise in a deleted slot (a file existed at one point but then became inaccessible).
- Otherwise in the slot that was requested the longest time ago.
