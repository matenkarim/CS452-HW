# CS 452 - 002 | HW1 
# Maten Karim
## Project Specification
This project provides an implementation of a double-ended queue (Deque) in C, along with:
- A shared library (`libdeq.so`) containing core Deque operations (`deq.c` + `deq.h`).
- A main demonstration file (`main.c`) showcasing usage.
- A test suite (`test_deq.c`) that covers functionality, boundary conditions, and stress tests.
## Features
- Create and delete a Deque.
- Insert (put) elements at the head or tail.
- Remove (get) elements from the head or tail.
- Access elements by index from either the head or the tail.
- Remove elements by value, searching from either the head or the tail.
- Map a function over all elements.
- Convert the Deque to a string (with optional custom formatting).
## Prerequisites
- A C compiler (e.g., GCC).
- `make`, if you want to use the provided Makefile.
- (Optional) Tools like `valgrind` to check for memory leaks.
## Running the Program
### Prerequisites
Make sure you are in the correct project directory. 
From onyx, the terminal should look like this:


### 1. Running **main demo** with `deq.c`
To **build and run** the demo program, `main.c`, and the `deq.c` input this into the terminal:
```bash
make
./deq
```
After you are finished, you can run:
```bash
make clean
```
to get rid of all created files.
### 2. Running **main demo** with the shared object file, `libdeq.so`
To **build and run** the demo program, `main.c`, and the shared object file `libdeq.so` input this into the terminal:
```bash
make try
./try
```
After you are finished, you can run:
```bash
make clean
```
to get rid of all created files.
### 3. Running **Test Suite** with `deq.c`
To **build** the test suite, `test.c`, located in the `tests/` folder with the `deq.c` input this into the terminal:
```bash
gcc -o test tests/test.c deq.c
```
I kept getting an error message stating the `asprintf` implementation wasn't being recognized so I used this command to build instead:
```bash
gcc -D_GNU_SOURCE -o test tests/test.c deq.c
```
To **run** the test suite:
```bash
./test
```
After you are finished, you can run:
```bash
make clean
rm -rf test
```
to clean/delete all extra files.
### 4. Running **Test Suite** with the shared object file, `libdeq.so`
To **build** the test suite, `test.c`, located in the `tests/` folder with the shared object file, `libdeq.so`, input this into the terminal:
```bash
gcc -o test tests/test.c -L. -ldeq -Wl,-rpath=.
```
To **run** the test suite:
```bash
./test
```
After you are finished, you can run:
```bash
make clean
rm -rf test
```
to clean/delete all extra files.
### 5. Using Valgrind
To use valgrind with the **main demo** and the `deq.c`, simply enter this into the terminal:
```bash
make valgrind
```
If you would like to run the **test suite** with the `deq.c`, first build the test executable:
```bash
gcc -o test tests/test.c deq.c
```
**Or** if you are getting the warning message (see Step 3. above):
```bash
gcc -D_GNU_SOURCE -o test tests/test.c deq.c
```
Then, you can run this command to run valgrind for it:
```bash
valgrind --leak-check=full --show-leak-kinds=all ./test
```
## Results
Here is a link to a YouTube video, that demonstrates everything I have written about above:



