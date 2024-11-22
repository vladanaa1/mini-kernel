# Mini Kernel on RISC-V Architecture

This project involves the design and implementation of a mini kernel for the RISC-V architecture. It focuses on efficient **memory management**, **system calls**, **thread management**, and **semaphore-based synchronization**. The kernel also provides a user-friendly interface for developers to interact with these features.

For detailed project instructions and specifications, visit the official course page: [Mini Kernel Project - OS1](http://os.etf.bg.ac.rs/OS1/projekat/).

---

## Key Features

### 1. **Memory Management**
- **System Call: `0x01` - Memory Allocation**
  Allocates memory of at least `size` bytes, aligned to `MEM_BLOCK_SIZE` (64 ≤ MEM_BLOCK_SIZE ≤ 1024). Returns a pointer to the allocated memory or `null` if unsuccessful.
  
- **System Call: `0x02` - Memory Deallocation**
  Frees memory allocated with `mem_alloc`. Returns `0` on success or a negative error code on failure.

### 2. **Thread Management**
- **System Call: `0x11` - Thread Creation**
  Creates a thread and starts execution of the provided function. Returns a thread handle and `0` on success or a negative error code on failure.
  
- **System Call: `0x12` - Thread Exit**
  Terminates the calling thread. Returns a negative error code on failure.
  
- **System Call: `0x13` - Thread Dispatch**
  Triggers context switching, allowing other threads to execute.

### 3. **Synchronization Using Semaphores**
- **System Calls:**
  - `0x21` - Semaphore Creation
  - `0x22` - Semaphore Deallocation
  - `0x23` - Semaphore Wait
  - `0x24` - Semaphore Signal
  - `0x25` - Timed Wait
  - `0x26` - Try Wait
  
  These calls enable efficient synchronization and inter-thread communication using semaphores. The kernel handles semaphore creation, destruction, and operations like `wait`, `signal`, and timed waiting.

### 4. **Timing Functions**
- **System Call: `0x31` - Thread Sleep**
  Suspends the calling thread for a specified duration.

### 5. **Console Input/Output**
- **System Calls:**
  - `0x41` - Get Character: Reads a character from the console. Suspends the thread if no character is available.
  - `0x42` - Put Character: Writes a character to the console.

---

## System Call Overview

| Code   | System Call Name       | Description                                                                                   |
|--------|------------------------|-----------------------------------------------------------------------------------------------|
| `0x01` | `mem_alloc`            | Allocates memory aligned to `MEM_BLOCK_SIZE`.                                                 |
| `0x02` | `mem_free`             | Frees previously allocated memory.                                                           |
| `0x11` | `thread_create`        | Creates and starts a new thread.                                                             |
| `0x12` | `thread_exit`          | Terminates the current thread.                                                               |
| `0x13` | `thread_dispatch`      | Switches execution between threads.                                                         |
| `0x21` | `sem_open`             | Creates a semaphore.                                                                         |
| `0x22` | `sem_close`            | Destroys a semaphore and unblocks waiting threads.                                           |
| `0x23` | `sem_wait`             | Wait operation on a semaphore.                                                              |
| `0x24` | `sem_signal`           | Signal operation on a semaphore.                                                            |
| `0x25` | `sem_timedwait`        | Wait with a timeout.                                                                         |
| `0x26` | `sem_trywait`          | Non-blocking wait.                                                                           |
| `0x31` | `time_sleep`           | Suspends thread execution for a specified time.                                              |
| `0x41` | `getc`                 | Reads a character from the console.                                                         |
| `0x42` | `putc`                 | Writes a character to the console.                                                          |

---

## Developer Interface

A simple and intuitive C++ interface is provided for user programs to interact with the kernel.

### Classes and Methods

1. **Thread Management**
   ```cpp
   class Thread {
   public:
       Thread(void (*body)(void*), void* arg);
       virtual ~Thread();
       int start();
       static void dispatch();
       static int sleep(time_t);
   protected:
       Thread();
       virtual void run() {}
   private:
       thread_t myHandle;
       void (*body)(void*); void* arg;
   };
2. **Semaphores**
```c
  class Semaphore {
  public:
       Semaphore(unsigned init = 1);
       virtual ~Semaphore();
       int wait();
       int signal();
       int timedWait(time_t);
       int tryWait();
  private:
      sem_t myHandle;
};
```
3. **Periodic Threads**
```c
  class PeriodicThread : public Thread {
  public:
      void terminate();
  protected:
      PeriodicThread(time_t period);
      virtual void periodicActivation() {}
  private:
      time_t period;
};
```
4. **Console**
```c
  class Console {
  public:
      static char getc();
      static void putc(char);
};
```
