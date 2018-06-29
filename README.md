# GREMLINS. A project for Estrutura de Dados Básica I (EDB I)

## Introduction

This programming project implements the GREMLINS, GeREnciador de Memória com LIsta eNcadeada Simples, which in english would represent a Memory Management using Nested Lists. Here we implement one (out of several) possible way to work with ***memory pools***, a very useful tool when comes to certain situations where standard allocation functions of C++ won't suffice for your specific problem. This way, making possible adapt dynamic memory allocation depending on the problem. More information over Memory Pool technique can be found [here](https://en.wikipedia.org/wiki/Memory_pool).

## How to compile

First of all, obviously this repository is needed to work as desired, so we must to download this repo. As we know that `git` is a tremendous tool for any programmer, then we must have git installed.

It is also expected for user to have installed [doxygen](https://en.wikipedia.org/wiki/Doxygen) program. This will automatically generate documentation for code. Not a necessary package to compile and run program, but it is a tool to guide user through code functions. To install `doxygen` on UBUNTU, for example:

```bash
$ sudo apt-get install doxygen
```
To compile we will use a makefile, so compilations may be more dynamic and automatic.
```bash
# Using 'git clone' to clone this repo into desired directory:
$ git clone https://github.com/Codigos-de-Guerra/Gremlins.git

# Enter repo:
$ cd Gremlins

# To compile the whole project, insert 'make' inside of path's root:
$ make

# To generate file documentation, insert 'make docs' inside of path's root:
$ make docs

# To clean up all remaining trash data and files, such as the binary ones, insert 'make clean':
$ make clean
```

## How to execute

Now, we show how to run the program. 
```bash
# To execute program:
$ ./gremlins
```

### Example

```bash
$ ./gremlins
```
## GitHub Repository:

*https://github.com/Codigos-de-Guerra/Gremlins*

## Efficiency Comparison Test

Efficiency differences between our memmory pool's allocate and free operations, and standard *new* and *delete* operations, actually is our main focus here.
We decided to test throught allocations and free operations with the same quantities, for both Operational System(SO) and for our Memory Pool. Since they both make allocate the same amount of bytes, we are able to see which course of action is better.

## Recomendations

- Memory Pool's are really efficient. But it's greater efficiency is better achieved when many allocations are sure to be expected.
- Regarding the allocations strategies, the First Fit strategy will mostly like to be more efficient and quicker, when allocating mostly small variables.
- Also regarding allocations strategies, the Best Fit will ensure less fragmenting and consequently bigger free areas within the pool, when client code is expected to allocate bigger memory sizes and often make free operations.
- Considering the different approaches for searching where to store a client's information, we give the client the opportunity to choose which approach to follow. Therefore, within client's code, on the very creation of the memory pool, it should receive which allocation policy to follow. If nothing is provided, then we opted for the First-Fit policy.

#### Example on pool's creation

```bash
# Allocating 2Kb using First-Fit allocation policy for every new allocation on pool.
SLPool pool(2048, StoragePool::FIRST-FIT);

# Allocating 4Kb using First-Fit allocation policy for every new allocation on pool.
SLPool pool(4096);		#DEFAULT POLICY

# Allocating 10Kb using Best-Fit allocation policy for every new allocation on pool.
SLPool pool(10240, StoragePool::BEST_FIT);
```
## Authorship

Program developed by [_Daniel Oliveira Guerra_](https://github.com/Codigos-de-Guerra) (*daniel.guerra13@hotmail.com*) and [_Oziel Alves_](https://github.com/ozielalves) (*ozielalves@ufrn.edu.br*), 2018.1

&copy; IMD/UFRN 2018.
