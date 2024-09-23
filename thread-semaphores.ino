#include <mbed.h>
#include <rtos.h>
#include <mbed_wait_api.h>
#include <platform/CircularBuffer.h>
#include <platform/Callback.h>

#define BUFF_SIZE 8

using namespace rtos;

// Semaphores manage access to a shared resource
// This semaphore allows one thread to access the shared buffer
Semaphore sem(1);

// These threads will write to the buffer, the first thread
// will read it, if there's data in it
Thread t2;
Thread t3;

// The shared buffer
mbed::CircularBuffer<char, BUFF_SIZE> buff;

// The names of the threads
char sandy[6] = {'S', 'a', 'n', 'd', 'y', '\0'};
char bill[5] = {'B', 'i', 'l', 'l', '\0'};

// The two writing threads will execute
// this method over and over again
void writeBuffer(char* val)
{  
  while(true)
  {
    // Wait for the shared resource to be free
    sem.try_acquire_for(1000);

    // Check, whether the buffer is empty
    // No bounds check is necessary in this case
    // overflowing data will simply overwrite the
    // same buffer
    if(!buff.empty())
    {
      // Don't forget to release the semaphore
      // Otherwise the other threads will wait forever
      sem.release();
      continue;
    }

    // Reset the buffer
    buff.reset();

    // Store the new message
    int index = 0;
    
    while(val[index] != '\0')
    {
      buff.push(val[index++]);
    }

    // Tell the semaphore that this thread is done writing
    sem.release();
  }
}

void setup()
{
  Serial.begin(9600);
  
  t2.start(mbed::callback(writeBuffer, sandy));
  t3.start(mbed::callback(writeBuffer, bill));
}

void loop()
{ 
  char data = 0;
  
  if(!buff.empty())
  {
    sem.try_acquire_for(1000);
    
    Serial.print("The buffer contained: ");

    while(buff.pop(data))
    {
      Serial.print(data);
    }

    Serial.println("");

    sem.release();
  }
  else
    wait_ns(150000);
}
