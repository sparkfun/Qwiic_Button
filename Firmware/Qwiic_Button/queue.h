/******************************************************************************
  registers.h
  Fischer Moseley @ SparkFun Electronics
  Original Creation Date: July 31, 2019

  This file defines the Queue struct, which serves as an easy way to implement a
  FIFO-style circular ring buffer. It contains a few functions for determining
  the state of the queue, and will allow the user to push/pop items from the
  queue, and also get the items at the front and back of the queue.

  It's worth noting that the fundamental datatype here is a 32-bit unsigned long,
  so if you're looking to store signed numbers you may want to adjust that.
  Other than swapping "unsigned long" for "signed int" the code would require
  no further modification.

  This code is beerware; if you see me (or any other SparkFun employee) at the
  local, and you've found our code helpful, please buy us a round!

  Distributed as-is; no warranty is given.
******************************************************************************/

#define BUTTON_QUEUE_SIZE 15 //How many button events to store in memory.

struct Queue {

  //constructor
  unsigned long buffer[BUTTON_QUEUE_SIZE]; //create array to store timestamps in
  byte head = 0; //create pointers to the first and last elements
  byte tail = 0;
  bool full = false; //state variable, true if queue is full

  //Returns whether or not the Queue is full
  bool isFull() {
    return full;
  }

  //Returns whether or not the Queue is empty
  bool isEmpty() {
    if (!full && (head == tail) )
      return true;

    return false;
  }

  //Increments the head pointer with wrap around
  void incrementHead() {
    head = (head + 1) % BUTTON_QUEUE_SIZE;
  }

  //Incrments the tail pointer with wrap around
  void incrementTail() {
    tail = (tail + 1) % BUTTON_QUEUE_SIZE;
  }

  //Pushes a value to the top of the buffer, but removes the oldest value if the buffer is full
  void push(unsigned long timestamp) {
    if (isFull()) {
      incrementTail();
    }
    buffer[head] = timestamp;
    incrementHead();

    if (head == tail) {
      full = true;
    }
  }

  //Returns the oldest value in the buffer, or garbage if the buffer is empty
  unsigned long back() {
    return buffer[tail];
  }

  //Returns the youngest value in the buffer
  unsigned long front() {
    if (!isEmpty()) {
      return buffer[(head + BUTTON_QUEUE_SIZE - 1) % BUTTON_QUEUE_SIZE];
    }
    return 0;
  }

  //Removes a value from the back of the buffer, but also returns the value it removed
  unsigned long pop() {

    if (!isEmpty()) {
      unsigned long return_val = buffer[tail];
      incrementTail();
      full = false;

      return return_val;
    }
  }

  //Prints a bunch of debug data on the queue
  void displayBuffer() {
#if defined(__AVR_ATmega328P__)
    Serial.println("Wake!");
    Serial.print("Queue: Head:");
    Serial.print(head);
    Serial.print("/ Tail: ");
    Serial.print(tail);
    Serial.println();

    for (int x = 0 ; x < BUTTON_QUEUE_SIZE ; x++) {
      Serial.print(x);
      if (x < 10) Serial.print(" ");
      Serial.print(":");
      Serial.print(buffer[x]);

      if (x == head) {
        Serial.print(" (HEAD)");
      }

      if (x == tail) {
        Serial.print(" (TAIL)");
      }
      Serial.println();
    }
#endif    
  }
};
