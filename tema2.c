#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Item
{
    // Struct to store data, used in the Tree structure and Heap
    int frequency;
    char* name;
} Item;
typedef struct node
{
    // Node structure, used for building trees
    Item* data; // Holds the data for the node
    struct node* left;
    struct node* right;
} node;

typedef struct Tree
{
    int n_nodes;
    struct node* root;
} Tree;

typedef struct Heap
{
    node** data_arr; // Array to store pointers to nodes (heap elements)
    int n_nodes;
    int max_capacity;
} Heap;


void INIT_TREE(Tree** src_tree)
{
    // Helper function to initialize a tree
    (*src_tree) = (Tree*)malloc(sizeof(Tree));
    if ((*src_tree) == NULL)
    {
        perror("Error on malloc src_tree");
        return;
    }
    (*src_tree)->n_nodes = 0;
    (*src_tree)->root = NULL;
}

void INIT_HEAP(Heap** src_heap)
{
    // Helper function to initialize a heap
    (*src_heap) = (Heap*)malloc(sizeof(Heap));
    if((*src_heap) == NULL)
    {
        perror("Error on malloc src_heap");
        return;
    }
    (*src_heap)->n_nodes = 0;
    (*src_heap)->max_capacity = 10;
    (*src_heap)->data_arr = (node**)malloc(sizeof(node*) * 10);
}

void HEAP_SWAP(node** el_a, node** el_b)
{
    // Function to swap two nodes in the heap array, used in insert and remove operations
    node* temp = *el_a;
    *el_a = *el_b;
    *el_b = temp;
}

int NODE_CMP(node* el_a, node* el_b)
{
    // Function to handle cases when nodes have the same frequency
    // In such cases, they are ordered by their name (lexicographically)
    if (el_a->data->frequency < el_b->data->frequency)
    {
        return -1;
    } else if (el_a->data->frequency > el_b->data->frequency)
    {
        return 1;
    } else{
        return strcmp(el_a->data->name, el_b->data->name);
    }
}

void INSERT_MIN_HEAP(Heap* min_heap, char* name, int freq, node* inserted_node)
{
    // Function to insert a new item into the min-heap
    node* new_node;
    if (inserted_node == NULL)
    {
        Item* new_item = (Item*)malloc(sizeof(Item)); // Initialize new data container
        if (new_item == NULL)
        {
            perror("Error on malloc new item");
            return;
        }
        new_node = (node*)malloc(sizeof(node));
        if (new_node == NULL)
        {
            free(new_item);
            perror("Error on malloc new node");
            return;
        }
        new_item->frequency = freq;
        new_item->name = strdup(name);
        new_node->data = new_item;
        new_node->left = NULL;
        new_node->right = NULL;
    } else {
        new_node = inserted_node;
    }
    // Perform insertion
    int n = min_heap->n_nodes;
    int max_cap = min_heap->max_capacity;
    // If the data array is full, double its capacity using realloc
    if (n == max_cap)
    {
        max_cap *= 2; // Increase max capacity
        node** new_data_arr = (node**)realloc(min_heap->data_arr,
                                              sizeof(node*) * max_cap);
        if (new_data_arr == NULL)
        {
            perror("Error on realloc new data_arr");
            return;
        }
        min_heap->data_arr = new_data_arr;
        min_heap->max_capacity = max_cap;
    }
    min_heap->data_arr[n] = new_node;
    min_heap->n_nodes++;

    // Perform heapify according to min-heap rules
    while (n > 0 && NODE_CMP(min_heap->data_arr[n], 
           min_heap->data_arr[(n-1) / 2]) < 0)
    {
        HEAP_SWAP(&min_heap->data_arr[n], &min_heap->data_arr[(n-1) / 2]);
        n = (n-1) / 2;
    }
}

node* GET_MIN(Heap* min_heap)
{
    // Function to get the minimum element in the heap
    if (min_heap->n_nodes == 0) return NULL;
    return min_heap->data_arr[0]; // In a min-heap, the minimum is the first element
}

void REMOVE_ELEMENT(Heap* min_heap, int val)
{
    int idx = -1;
    int n = min_heap->n_nodes;
    // Try to find a specific element by its value (frequency)
    for (int i = 0; i < n; i++)
    {
        if (min_heap->data_arr[i]->data->frequency == val)
        {
            idx = i;
            break;
        }
    }

    if (idx == -1)
    {
        printf("Val not found in min heap");
        return;
    }
    // Replace current element with the last one
    min_heap->data_arr[idx] = min_heap->data_arr[n-1];
    // Remove that element from the min-heap
    min_heap->data_arr[n-1] = NULL;
    min_heap->n_nodes--;
    n--;
    // Perform heapify according to min-heap rules
    while (1)
    {
        int child_l = 2 * idx + 1;
        int child_r = 2 * idx + 2;
        int lowest = idx;
        if (child_l < n && NODE_CMP(min_heap->data_arr[child_l], 
                min_heap->data_arr[lowest]) < 0)
        {
            lowest = child_l;
        }
        if(child_r < n && NODE_CMP(min_heap->data_arr[child_r], 
                min_heap->data_arr[lowest]) < 0)
        {
            lowest = child_r;
        }
        if(lowest != idx)
        {
            HEAP_SWAP(&min_heap->data_arr[idx], &min_heap->data_arr[lowest]);
            idx = lowest;
        } else {
            break;
        }
    }
}

node* HEAP_BUILD_MIN_NODE(Heap* min_heap)
{
    // Function to get the minimum element from the heap and remove it
    // (In the first version, the heap was built using an array of Item*,
    // so this function created a new node from an Item element)
    node* min_element = GET_MIN(min_heap);
    if (min_element == NULL) return NULL;
    REMOVE_ELEMENT(min_heap, min_element->data->frequency);
    return min_element;
}

void CONSTRUCT_HEAP(Heap* min_heap, int* satellites_freq,
                    char** satellites_name, int n)
{
    // Function to build the initial heap from input data
    for (int i=0; i < n; i++)
    {
        INSERT_MIN_HEAP(min_heap, satellites_name[i], satellites_freq[i], NULL);
    }
}

void CONSTRUCT_TREE(Heap* min_heap, Tree* final_tree)
{
    // Function to build the final tree from the heap
    node* parent = NULL;
    while (min_heap->n_nodes > 1)
    {
        // Get two nodes with the lowest frequency
        node* min_left = HEAP_BUILD_MIN_NODE(min_heap);
        node* min_right = HEAP_BUILD_MIN_NODE(min_heap);
        Item* new_data_container = (Item*)malloc(sizeof(Item));
        if (new_data_container == NULL)
        {
            perror("Error on malloc new data container for parent node");
        }
        int new_name_len = (int)strlen(min_left->data->name) +
                           (int)strlen(min_right->data->name) + 1;
        // Initialize a new parent node to store concatenated data
        parent = (node*)malloc(sizeof(node));
        if (parent == NULL)
        {
            perror("Error on malloc new parent node");
            free(new_data_container);
            return;
        }
        parent->left = min_left; // Store addresses for those two nodes
        parent->right = min_right;
        new_data_container->frequency = min_left->data->frequency +
                                        min_right->data->frequency;
        new_data_container->name = (char*)malloc(sizeof(char) * new_name_len);
        if (new_data_container->name == NULL)
        {
            perror("Error on malloc name field for parent node");
            free(new_data_container);
            free(parent);
        }
        // Again, handle the case when nodes have the same frequency
        if (NODE_CMP(min_left, min_right) < 0)
        {
            strcpy(new_data_container->name, min_left->data->name);
            strcat(new_data_container->name, min_right->data->name);
        } else {
            strcpy(new_data_container->name, min_right->data->name);
            strcat(new_data_container->name, min_left->data->name);
        }
        parent->data = new_data_container;
        final_tree->n_nodes++;
        INSERT_MIN_HEAP(min_heap, NULL, 0, parent);
    }
    final_tree->root = parent;
}

int GET_MAX(int a, int b)
{
    // Function equivalent to max()
    return a > b ? a : b;
}

int GET_TREE_HEIGHT(node* root)
{
    // Function that recursively finds the deepest leaf in the tree
    if (root == NULL)
    {
        return 0;
    }
    int left_height = GET_TREE_HEIGHT(root->left);
    int right_height = GET_TREE_HEIGHT(root->right);
    return 1 + GET_MAX(left_height, right_height);
}

void PRINT_SPECIFIC_TREE_LEVEL(node* root, int level, FILE* out_file)
{
    // Recursively, starting from the root node, go down "level" times
    // then print the node at that level
    if (root == NULL) {return;}
    if (level == 0)
    {
        fprintf(out_file, "%d-%s ", root->data->frequency, root->data->name);
    } else {
        PRINT_SPECIFIC_TREE_LEVEL(root->left, level-1, out_file);
        PRINT_SPECIFIC_TREE_LEVEL(root->right, level-1, out_file);
    }
}
void PRINT_TREE_LEVELS(Tree* final_tree, FILE* out_file)
{
    // Helper function to print all levels, starting from the root
    if (final_tree->root == NULL)
    {
        printf("Tree is empty\n");
        return;
    }
    int levels = GET_TREE_HEIGHT(final_tree->root);
    for (int i = 0; i < levels; i++)
    {
        PRINT_SPECIFIC_TREE_LEVEL(final_tree->root, i, out_file);
        fprintf(out_file, "\n");
    }
}

void PROCEED_TASK_1(Tree* final_tree, Heap* min_heap, FILE* in_file)
{
    // Main function to perform task 1 and build the final tree for all other tasks
    int satellit_count, temp;
    char buff[256];
    fscanf(in_file, "%d", &satellit_count); // Get number of satellites
    while ((temp = fgetc(in_file)) != '\n' && temp != EOF){}
    // Array to store all satellites' frequencies
    int* satellites_freq = (int*)malloc(sizeof(int) * satellit_count);
    if (satellites_freq == NULL)
    {
        perror("Error on malloc satellites_freq");
        return;
    }
    // Array to store all satellites' names
    char** satellites_name = (char**)malloc(sizeof(char*) * satellit_count);
    if (satellites_name == NULL)
    {
        perror("Error on malloc satellites_name vector");
        free(satellites_freq);
        return;
    }

    // Read n satellites from input
    for (int i = 0; i < satellit_count; i++){
        fscanf(in_file, "%d %s", &satellites_freq[i], buff);
        buff[strcspn (buff, "\n")] = '\0';
        satellites_name[i] = strdup(buff);
        if (satellites_name[i] == NULL)
        {
            perror("Error on malloc satellites_name_i");
            for (int j = 0; j < i; j++){
                if (satellites_name[i] != NULL)
                {
                    free(satellites_name[j]);
                }
            }
            free(satellites_freq);
            free(satellites_name);
            return;
        }		
    }
    // After obtaining all input data, build the heap and construct the tree
    CONSTRUCT_HEAP(min_heap, satellites_freq, satellites_name, satellit_count);
    for (int i = 0; i < satellit_count; i++)
    {
        free(satellites_name[i]);
    }
    free(satellites_freq);
    free(satellites_name);
    CONSTRUCT_TREE(min_heap, final_tree);
}
void PROCEED_TASK_2(Tree* final_tree, FILE* in_file, FILE* out_file)
{
    // Main function to perform task 2
    // (Traverse the tree by a given binary path)
    int n_codif, temp;
    char buff[256];
    fscanf(in_file, "%d", &n_codif);
    while ((temp = fgetc(in_file)) != '\n' && temp != EOF){}
    for (int i = 0; i < n_codif; i++){
        fscanf(in_file, "%s", buff);
        node* root = final_tree->root;
        for (int j = 0; j < (int)strlen(buff); j++)
        // If 0, go left; if 1, go right in the tree
        {
            if (buff[j] == '0')
            {
                root = root->left;
            } else if (buff[j] == '1')
            {
                root = root->right;
            }
            if (root->left == NULL && root->right == NULL)
            {
                fprintf(out_file, "%s ", root->data->name);
                root = final_tree->root;
            }
        }
        fprintf(out_file, "\n");
    }
}

int BUILD_PATH_NODE(node* root, char** path, int level, char* name, int* max_len)
{
    // Helper function used for task 3.
    // Recursively traverses left and right until it finds the specific node
    // If found, builds the binary path
    if (root == NULL)
    {
        return 0;
    }
    if (strcmp(root->data->name, name) == 0) 
    {
        // If we are at the searched node, we know the depth in the tree
        // so we can build the path
        *max_len = level;
        if ((int)strlen(*path) <= level)
        {
            char* temp_buff = (char*)realloc(*path, sizeof(char) * (level * 2));
            if (temp_buff == NULL)
            {
                perror("Error on malloc new temp buffer");
                return 0;
            }
            *path = temp_buff;
        }
        return 1;
    }
    if (BUILD_PATH_NODE(root->right, path, level + 1, name, max_len))
    {
        (*path)[level] = '1'; 
        return 1;
    } else if (BUILD_PATH_NODE(root->left, path, level + 1, name, max_len))
    {
        (*path)[level] = '0';
        return 1;
    }
    return 0;
}
void PROCEED_TASK_3(Tree* final_tree, FILE* in_file, FILE* out_file)
{
    // Main function to perform task 3
    // Reads n satellites and tries to find the binary path to each of them
    int n_satellites, temp;
    char buff[512] = {0};
    char* path = (char*)calloc(sizeof(char), 256); // buffer for build_path_node
    int concat_capacity = 1024; // initial max capacity of final path
    char* concat_path = (char*)calloc(sizeof(char), concat_capacity);
    if (path == NULL || concat_path == NULL)
    {
        perror("Error on calloc path | concat_path");
    }
    fscanf(in_file, "%d", &n_satellites);
    while ((temp = fgetc(in_file)) != '\n' && temp != EOF){}
    int n;
    // Read n satellites and call build_path_node for each
    // Then concatenate each path to the final path
    for (int i = 0; i < n_satellites; i++)
    {
        fscanf(in_file, "%s", buff);
        buff[strcspn (buff, "\n")] = '\0';
        BUILD_PATH_NODE(final_tree->root, &path, 0, buff, &n);
        path[n] = '\0';
        if ((int)strlen(concat_path) + n >= concat_capacity)
        {
            char* temp_buff = (char*)realloc(
                              concat_path, sizeof(char) * strlen(concat_path));
            if (temp_buff == NULL)
            {
                perror("Error on realloc new tmp buffer");
                free(path);
                free(concat_path);
                return;
            }
            concat_path = temp_buff;
        }
        strncat(concat_path, path, n);
    }
    fprintf(out_file, "%s", concat_path);
    free(path);
    free(concat_path);
}
node* GET_NODE(node* root, char* name)
{
    // Helper function to find a specific node in the tree
    if (root == NULL)
    {
        return NULL;
    }
    if (strcmp(root->data->name,name) == 0)
    {
        return root;
    }
    return GET_NODE(root->left, name) != NULL ? GET_NODE(root->left, name) : 
                                                GET_NODE(root->right, name);
}

node* LOWEST_COMMON_NODE(node* root, node* left, node* right)
{
    // Helper function for Lowest Common Ancestor
    // If NULL, nothing found
    // If root == left or root == right, found the searched node
    if (root == NULL || root == left || root == right)
    {
        return root;
    }
    // Recursively call LOWEST_COMMON_NODE for left and right subtrees
    // to find the two specific nodes
    node* left_side = LOWEST_COMMON_NODE(root->left, left, right);
    node* right_side = LOWEST_COMMON_NODE(root->right, left, right);
    if (left_side != NULL && right_side != NULL)
    {
        // In this case, root is the lowest common ancestor.
        return root;
    }
    if (left_side != NULL)
    {
        return left_side;
    }
    return right_side;
}

void PROCEED_TASK_4(Tree* final_tree, FILE* in_file, FILE* out_file)
{
    // Main function for solving task 4
    // (Finding the lowest common ancestor of all given nodes)
    int n_satellites, temp;
    char buff[256];
    fscanf(in_file, "%d", &n_satellites); // Read number of given nodes
    while ((temp = fgetc(in_file)) != '\n' && temp != EOF){}
    // Array of node pointers (addresses of the given nodes in the tree)
    node** node_arr = malloc(sizeof(node*) * n_satellites); 
    if (node_arr == NULL)
    {
        perror("Error on malloc node_arr");
        return;
    }
    for (int i = 0; i < n_satellites; i++)
    {
        fscanf(in_file, "%s", buff); // Read node name
        node_arr[i] = GET_NODE(final_tree->root, buff); // Search for it in the tree and store the pointer
    }
    // Lowest Common Ancestor algorithm
    node* top_node = node_arr[0]; // Set the first node as the initial ancestor
    for (int i = 1; i < n_satellites; i++)
    {
        // For n nodes, call the helper function LOWEST_COMMON_NODE
        // It takes the current ancestor and the i-th node from the array
        // and tries to find their common ancestor
        top_node = LOWEST_COMMON_NODE(final_tree->root, top_node, node_arr[i]);
    }
    fprintf(out_file, "%s", top_node->data->name); // Print the result
    free(node_arr);
}
void FREE_NODES(node* root)
{
    // Recursive function to free all nodes in the tree
    if (root == NULL) return;
    FREE_NODES(root->left);
    FREE_NODES(root->right);
    if (root->data != NULL)
    {
        if (root->data->name != NULL)
        {
            free(root->data->name);
        }
        free(root->data);
    }
    free(root);
}
void FREE_TREE(Tree* final_tree)
{
    // Helper function to free the tree memory
    if (final_tree == NULL) return;
    FREE_NODES(final_tree->root);
    free(final_tree);
}
void FREE_HEAP(Heap* min_heap)
{
    // Helper function to free the heap memory
    if (min_heap == NULL) return;
    if (min_heap->data_arr != NULL)
    {
        free(min_heap->data_arr);
    }
    free(min_heap);
}

int main(int argc, char** argv)
{
    // Check if the number of command line arguments is correct
    if (argc != 4)
    {
        printf("[ERROR] You should have 3 args | TASK | IN_FILE | OUT_FILE |");
        return 1;
    }
    // Initialize the tree of satellites and the min-heap to access the lowest nodes
    Tree* final_tree;
    Heap* min_heap;
    INIT_TREE(&final_tree);
    INIT_HEAP(&min_heap);
    FILE* in_file = fopen(argv[2], "r");
    FILE* out_file = fopen(argv[3], "w");
    // Initialize a vector of strings that can be extended with more subtasks
    // A simple mapping variant
    const char* task_type[] = {
        "-c1", "-c2", "-c3", "-c4", "-c5"
    };
    enum task_enum {
        task_c1, task_c2, task_c3, task_c4, task_c5
    };
    int task_len = sizeof(task_type) / sizeof(task_type[0]);
    int type = -1;
    for (int i = 0; i < task_len; i++)
    {
        // Try to map the task argument
        if (strstr(argv[1], task_type[i]) != NULL) 
        {
            type = i;
        }
    }
    if (type == -1)
    {
        printf("Task %s is invalid argument", argv[2]);
        return 1;
    }
    // Switch that can be adapted and extended for more tasks
    // by adding a new case and its name to the enum
    switch (type)
    {
        case task_c1: {
            PROCEED_TASK_1(final_tree, min_heap, in_file); 
            PRINT_TREE_LEVELS(final_tree, out_file);
            break;
        }
        case task_c2: {
            PROCEED_TASK_1(final_tree, min_heap, in_file); 
            PROCEED_TASK_2(final_tree, in_file, out_file);
            break;
        }
        case task_c3: {
            PROCEED_TASK_1(final_tree, min_heap, in_file); 
            PROCEED_TASK_3(final_tree, in_file, out_file);
            break;
        }
        case task_c4: {
            PROCEED_TASK_1(final_tree, min_heap, in_file);
            PROCEED_TASK_4(final_tree, in_file, out_file);
            break;
        }
    }
    FREE_TREE(final_tree);
    FREE_HEAP(min_heap);
    fclose(in_file);
    fclose(out_file);
    return 0;
}


