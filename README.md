# Compiler Construction Assignments

## Assignments

### Assignment 1

1. Write a LEX specification file:

   - To count the number of words in a file and their total size
   - That accepts the English language words (without bothering for the meaning) and replaces each occurence of the string "abc" in it to "ABC".

2. The following is a listing of a set of verbs

_is am are were_

_was be being been_

_do does did will_

_would should can could_

_has have had go_

Write a simple LEX specification to recognize these verbs

### Assignment 2

1. Using the Recursive Descent strategy, write a C program for a simple calculator that can be used to perform integer arithmetic involving "+" and "\*". Let your program consist of a set of mutually recursive routines.
2. Using the following grammar,
   $$ S \longrightarrow aS | b $$
   create an interpreter using LEX and YACC which will count the number of a's in the input string.
3. Write LEX and YACC specification files for a small calculator that can add and subtract numbers.

## Getting started

Currently (for the first assignment), the build process is managed using a Makefile, and that means that
the instructions below will only work on Unix-like systems (i.e. not Windows, except through [WSL](https://learn.microsoft.com/en-us/windows/wsl/about)). We can move to [CMake](https://cmake.org/) later to allow for platform-agnostic build management.

This assumes that your machine is set up for C/C++ development.

If not get the packages using the command

```bash
sudo apt install build-essentials
```

You should have [Make](https://www.gnu.org/software/make/) installed in your system. You can check this by using the command:

```bash
make --version
```

If you do not have it, you can get it using `apt` with the commands:

```bash
sudo apt update
sudo apt install make
```

or you can get it from the [Make website](https://www.gnu.org/software/make/#download).

You should also install `flex`, the package for the lexical analyzer:

```bash
sudo apt install flex
```

With `make` installed, to build the binaries from the LEX files corresponding to all the questions above:

```bash
cd assignment-1
make
```

The binaries are in the `build/` folder and each program takes in a file as one of the command-line arguments e.g:

```bash
./build/word_count text-samples/word-count.txt
```

or you can use the make targets with the default files. There is currently only one:

```bash
make run_word_count  # For the first part of question 1
```

### Useful Resources

- [Makefile tutorial](https://makefiletutorial.com)
- [Introduction to Compilers and Language Design](https://www3.nd.edu/~dthain/compilerbook/) book
