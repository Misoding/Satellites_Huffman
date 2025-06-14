# Sattelites System & Huffman Tree Builder

This project implements a program in C that constructs a binary tree (similar to a Huffman tree) from a set of elements, using a min-heap as an intermediate structure. The program supports several tasks, including building the tree, traversing it, encoding/decoding paths, and finding the lowest common ancestor of nodes.

---

## Table of Contents

- [Introduction](#introduction)
- [Data Structures](#data-structures)
- [Main Functionalities](#main-functionalities)
- [Program Flow](#program-flow)
- [Building and Running](#building-and-running)
- [Memory Management](#memory-management)
- [File Structure](#file-structure)

---

## Introduction

This program reads a list of items (each with a name and frequency), builds a min-heap, and then constructs a binary tree by repeatedly combining the two nodes with the smallest frequencies. The resulting tree can be used for encoding/decoding (like Huffman coding), traversing by levels, and other tree-based queries.

---

## Data Structures

```c
typedef struct Item {
    int frequency;
    char* name;
} Item;

typedef struct node {
    Item* data;
    struct node* left;
    struct node* right;
} node;

typedef struct Tree {
    int n_nodes;
    struct node* root;
} Tree;

typedef struct Heap {
    node** data_arr;
    int n_nodes;
    int max_capacity;
} Heap;
```

- **Item**: Holds the frequency and name of an element.
- **node**: Represents a tree node, with pointers to left/right children and its data.
- **Tree**: Holds the root of the tree and the number of nodes.
- **Heap**: Implements a min-heap of tree nodes for efficient tree construction.

---

## Main Functionalities

### 1. Heap Operations

- **INIT_HEAP**: Initializes a new heap.
- **INSERT_MIN_HEAP**: Inserts a new node into the min-heap, maintaining heap order.
- **REMOVE_ELEMENT**: Removes a node with a given frequency from the heap.
- **GET_MIN**: Returns the node with the minimum frequency.

### 2. Tree Construction

- **CONSTRUCT_HEAP**: Builds the initial heap from input data.
- **CONSTRUCT_TREE**: Builds the binary tree by combining nodes from the heap.

### 3. Tree Traversal and Queries

- **PRINT_TREE_LEVELS**: Prints the tree level by level.
- **PROCEED_TASK_2**: Decodes binary paths to names by traversing the tree.
- **PROCEED_TASK_3**: Finds and prints the binary path to a given node.
- **PROCEED_TASK_4**: Finds the lowest common ancestor of a set of nodes.

### 4. Utility Functions

- **GET_TREE_HEIGHT**: Computes the height of the tree.
- **GET_NODE**: Finds a node by name.
- **LOWEST_COMMON_NODE**: Finds the lowest common ancestor of two nodes.

---

## Program Flow

1. **Input**: The program takes three command-line arguments:
   - Task type (`-c1`, `-c2`, `-c3`, `-c4`)
   - Input file path
   - Output file path

2. **Initialization**: Initializes the heap and tree.

3. **Task Execution**: Depending on the task type, the program:
   - Builds the heap and tree from input.
   - Performs the requested operation (tree printing, path finding, decoding, etc.).
   - Writes the result to the output file.

4. **Cleanup**: Frees all allocated memory and closes files.

---

## Building and Running

To compile and run the program:

```sh
gcc tema2.c -o tema2 -Wall -Wextra -g
./tema2 -c1 input.txt output.txt
```

Replace `-c1` with the desired task (`-c1`, `-c2`, `-c3`, `-c4`), and provide your input/output files.

---

## Memory Management

- All dynamic allocations (nodes, items, arrays) are properly freed at the end of execution.
- Helper functions `FREE_TREE` and `FREE_HEAP` ensure no memory leaks.

---

## File Structure

- **tema2.c**: Main source file containing all logic and function implementations.
- **input.txt**: Input data file (format depends on the task).
- **output.txt**: Output file for results.

---

## Example Tasks

- **-c1**: Build the tree and print it level by level.
- **-c2**: Decode binary paths to names.
- **-c3**: Find the binary path to a given node.
- **-c4**: Find the lowest common ancestor of a set of nodes.

---

## Notes

- The program is modular and can be extended with new tasks by adding functions and updating the main switch-case.
- Error handling is included for all memory allocations and file operations.

---

## Author

*Your Name Here*
