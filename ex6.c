/******************
Name: Yotam Yakov
ID: 207918822
Assignment: ex6
*******************/

#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# define INT_BUFFER 128


// PRE MADE FUNCTION
void trimWhitespace(char* str) {
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0) {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r')) {
        str[--len] = '\0';
    }
}

char* myStrdup(const char* src) {
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char* dest = (char* )malloc(len + 1);
    if (!dest) {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char* prompt) {
    char buffer[INT_BUFFER];
    int value;
    int success = 0;

    while (!success) {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }

        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';

        // 2) Check if empty after stripping
        if (len == 0) {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char* endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0') {
            printf("Invalid input.\n");
        }
        else {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

char* getDynamicInput() {
    char* input = NULL;
    size_t size = 0, capacity = 1;
    input = (char* )malloc(capacity);
    if (!input) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char* temp = (char* )realloc(input, capacity);
            if (!temp) {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}

const char* getTypeName(PokemonType type) {
    switch (type) {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

// BST ITERATING FUNCTIONS
// Apply funtion on BFS ordering
void BFSGeneric(PokemonNode* root, VisitNodeFunc visit) {
    // Create queue
    Queue* queue = (Queue*) malloc(sizeof(Queue));

    enQueue(queue, root);

    // Iterate queue
    while(queue->front) {
        PokemonNode* current = deQueue(queue);
        // Use visit function
        visit(current);
        if(current->left) {
            enQueue(queue, current->left);
        }
        if(current->right) {
            enQueue(queue, current->right);
        }
    }

    // Free queue node
    free(queue);
}

// Apply funtion on pre order
void preOrderGeneric(PokemonNode* root, VisitNodeFunc visit) {
    if(!root) {
        return;
    }
    visit(root);
    preOrderGeneric(root->left, visit);
    preOrderGeneric(root->right, visit);
}

// Apply funtion on in order
void inOrderGeneric(PokemonNode* root, VisitNodeFunc visit) {
    if(!root) {
        return;
    }
    inOrderGeneric(root->left, visit);
    visit(root);
    inOrderGeneric(root->right, visit);
}

// Apply funtion on post order
void postOrderGeneric(PokemonNode* root, VisitNodeFunc visit) {
    if(!root) {
        return;
    }
    if(root->left) {
        postOrderGeneric(root->left, visit);
    }
    if(root->right) {
        postOrderGeneric(root->right, visit);
    }
    visit(root);
}


// SEARCHING FUNCTIONS
// Search for pokemon by ID
PokemonNode* searchPokemonBFS(PokemonNode* root, int id) {
    if(root->data->id == id) {
        return root;
    }

    PokemonNode* left = root->left ? searchPokemonBFS(root->left, id) : NULL;
    PokemonNode* right = root->right ? searchPokemonBFS(root->right, id) : NULL;

    return left ? left : (right ? right : NULL);
}

// Search fir owner by ID
OwnerNode* findOwnerByIndex(int index) {
    OwnerNode* owner = ownerHead;

    for(int i = 1; i < index; i++) {
        owner = owner->next;
    }

    return owner;
}

// Search fir owner by name
OwnerNode* findOwnerByName(const char* name) {
    OwnerNode* owner = ownerHead;
    do {
        if(strcmp(owner->ownerName, name) == 0) {
            return owner;
        }
        owner = owner->next;
    } while (owner != ownerHead);
    
    return NULL;
}

// Search for min name owner
OwnerNode* findMinOwnerName() {
    // Set first owner as min
    OwnerNode* owner = ownerHead;
    OwnerNode* minOwner = owner;

    // While there are owners to check iterate them
    while(1) {
        // If new owner found set it as min
        if(strcmp(minOwner->ownerName, owner->ownerName) > 0) {
            minOwner = owner;
        }
        // If no more owners left to check break loop
        if(owner->next == ownerHead) {
            break;
        }
        // Iterate to next
        owner = owner->next;
    }

    return minOwner;
}


// PRINTING FUNCTIONS
// Print all existing owners
void printAllOwners() {
    int index = 1;

    OwnerNode* owner = ownerHead;

    do {
        printf("%d. %s\n", index, owner->ownerName);
        index++;
        owner = owner->next;
        // While next owner exist, iterate to it
    } while(owner != ownerHead);

    return;
}

// Print number of owners in given direction
void printOwnersCircular(void) {
    // Validate owners exist
    if(!ownerHead) {
        printf("No owners.\n");
        return;
    }

    // Get direction input
    printf("Enter direction (F or B): ");
    char* input = getDynamicInput();

    // Validate correct direction input
    while(input[0] != 'f' && input[0] != 'F' && input[0] != 'b' && input[0] != 'B') {
        printf("Invalid direction, must be L or R.\n");
        printf("Enter direction (F or B): ");
        input = getDynamicInput();
    }

    // Get number of prints input
    int prints = readIntSafe("How many prints? ");

    OwnerNode* owner = ownerHead;
    // Iterate owners for given number
    for(int i = 1; i <= prints; i++) {
        printf("[%d] %s\n", i, owner->ownerName);

        // Choose next owner based on direction
        if(input[0] == 'f' || input[0] == 'F') {
            owner = owner->next;
        } else {
            owner = owner->prev;
        }
    }
    // Free direction input
    free(input);
}

// Print a single Pokemon node
void printPokemonNode(PokemonNode* node) {
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

// Print BFS
void displayBFS(PokemonNode* root) {
    BFSGeneric(root, printPokemonNode);
}

// Print post order
void preOrderTraversal(PokemonNode* root) {
    preOrderGeneric(root, printPokemonNode);
}

// Print in order
void inOrderTraversal(PokemonNode* root) {
    inOrderGeneric(root, printPokemonNode);
}

// Print post order
void postOrderTraversal(PokemonNode* root) {
    postOrderGeneric(root, printPokemonNode);
}

// Print alphabetical
void displayAlphabetical(PokemonNode* root) {
    // Collect all pokemon to a NodeArray
    NodeArray* array = initNodeArray(5);
    collectAll(root, array);
    // Sort array
    sortArray(array);

    // Print sorted array
    for(int i = 0; i < array->size; i++) {
        printPokemonNode(array->nodes[i]);
    }

    // Free array with nodes
    free(array->nodes);
    free(array);
}


// QUEUE ARRAY FUNCTIONS
// Initialize NodeArray
NodeArray* initNodeArray(int cap) {
    NodeArray* array = (NodeArray*) malloc(sizeof(NodeArray));
    array->size = 0;
    array->capacity = cap;
    array->nodes = malloc(sizeof(PokemonNode) * cap);
    
    return array;
}

// Add node to NodeArray
void addNode(NodeArray* array, PokemonNode* node) {
    // Add node
    array->nodes[array->size] = node;
    array->size++;

    // Resize if needed
    if(array->capacity < array->size) {
        array->capacity += 5;
        array->nodes = realloc(array->nodes, sizeof(PokemonNode) * array->capacity);
    }
}

// Enter a node to the queue
void enQueue(Queue* queue, PokemonNode* node) {
    // Create new node
    QueueNode* queueNode = (QueueNode*) malloc(sizeof(QueueNode));
    // Insert data
    queueNode->node = node;

    // If first node
    if(!queue->rear) {
        queue->front = queueNode;
        queue->rear = queueNode;
        return;
    }
    // Wire nodes
    queue->rear->next = queueNode;
    queue->rear = queueNode;
}

// Return first node on queue
PokemonNode* deQueue(Queue* queue) {
    // Validate there is a node
    if(!queue->front) {
        return NULL;
    }
    QueueNode* node = queue->front;
    
    // Rewire
    if(queue->front == queue->rear) {
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        queue->front = queue->front->next;   
    }

    // Collect data, free node and return data
    PokemonNode* data = node->node;
    node->node = NULL;
    free(node);
    return data;
}

// Collect all owner's Pokemon to NodeArray
void collectAll(PokemonNode* root, NodeArray* array) {
    // Base case
    if(!root) {
        return;
    }

    // Check left
    if(root->left) {
        collectAll(root->left, array);
    }
    //Check right
    if(root->right) {
        collectAll(root->right, array);
    }
    // Use add node
    addNode(array, root);
}

// Sort array by name
void sortArray(NodeArray* array) {
    PokemonNode* node,* nextNode;
    int current = 0;

    // Optimization
    int swap = 1;
    while(swap) {
        PokemonNode* temp;
        swap = 0;
        current = 0;
        // Get current and next nodes
        node = array->nodes[current];
        nextNode = array->nodes[current + 1];
        while(nextNode != NULL) {
            // If names unordered swap nodes
            if(strcmp(node->data->name, nextNode->data->name) > 0) {
                swap = 1;
                temp = node;
                array->nodes[current] = array->nodes[current + 1];
                array->nodes[current + 1] = temp;
            }
            // Iterate to next nodes
            current++;
            node = array->nodes[current];
            nextNode = array->nodes[current + 1];
        }
    }
    
    return;
}


// POKEMON SPECIFIC FUNCTIONS
// Add new Pokemon to pokedex
void addPokemon(OwnerNode* owner) {
    // Get input
    int id = readIntSafe("Enter ID to add: ");

    // Continue only if Pokemon exist
    if(owner->pokedexRoot && searchPokemonBFS(owner->pokedexRoot, id)) {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n", id);
        return;
    }

    // Gather data
    PokemonData* data = (PokemonData*) malloc(sizeof(PokemonData));
    *data = pokedex[id-1];

    // Create the Pokemon node
    PokemonNode* pokemon = createPokemonNode(data);

    // Add Pokemon node to pokedex
    if(!owner->pokedexRoot) {
        owner->pokedexRoot = pokemon;
    } else {
        insertPokemonNode(owner->pokedexRoot, pokemon);
    }
    
    printf("Pokemon %s (ID %d) added.\n", pokemon->data->name, pokemon->data->id);

}

// Free Pokemon from pokedex
void freePokemon(OwnerNode* owner) {
    // Validate pokedex exist
    if(!owner->pokedexRoot) {
        printf("No Pokemon to release.\n");
        return;
    }

    // Get input
    int id = readIntSafe("Enter Pokemon ID to release: ");
    // Continue only if Pokemon exists in pokedex
    if(!searchPokemonBFS(owner->pokedexRoot, id)) {
        printf("No Pokemon with ID %d found.\n", id);
        return;
    }

    // Remove Pokemon node from the BST
    removeNodeBST(owner->pokedexRoot, owner->pokedexRoot, owner, id);
    
    printf("Removing Pokemon %s (ID %d).\n", pokedex[id-1].name, id);
}

// Evolve pokemon in pokedex
void evolvePokemon(OwnerNode* owner) {
    // Validate pokedex exists
    if(!owner->pokedexRoot) {
        printf("Cannot evolve. Pokedex empty.\n");
        return;
    }

    //Get input
    int index = readIntSafe("Your choice: ");

    // Find requested pokemon
    PokemonNode* pokemon = searchPokemonBFS(owner->pokedexRoot, index);

    // If Pokemon not exist
    if(!pokemon) {
        printf("No Pokemon with ID %d found.\n", index);
    // If Pokemon cannot evolve
    } else if(pokemon->data->CAN_EVOLVE == CANNOT_EVOLVE) {
        printf("%s (ID %d) cannot evolve.", pokemon->data->name, pokemon->data->id);
    } else {
        *pokemon->data = pokedex[index + 1];
    }

    return;
}

// Make fight with 2 Pokemons
void pokemonFight(OwnerNode* owner) {
    // Validate pokedex exists
    if(!owner->pokedexRoot) {
        printf("Pokedex is empty.");
        return;
    }

    // Get input
    int index1 = readIntSafe("Enter ID of the first Pokemon: ");
    int index2 = readIntSafe("Enter ID of the second Pokemon: ");

    // Find requested Pokemons
    PokemonNode* pokemon1 = searchPokemonBFS(owner->pokedexRoot, index1);
    PokemonNode* pokemon2 = searchPokemonBFS(owner->pokedexRoot, index2);

    // If missing at least one
    if(!pokemon1 || !pokemon2) {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }

    // Calculate scores
    float score1 = (pokemon1->data->attack * 1.5) + (pokemon1->data->hp * 1.2);
    float score2 = (pokemon2->data->attack * 1.5) + (pokemon2->data->hp * 1.2);

    printf("Pokemon 1: %s (Score = %.2f)\n", pokemon1->data->name, score1);
    printf("Pokemon 2: %s (Score = %.2f)\n", pokemon2->data->name, score2);

    // Determine winner
    PokemonNode* winner = score1 > score2 ? pokemon1 : (score1 < score2 ? pokemon2 : NULL);

    if(winner) {
        printf("%s wins!\n", winner->data->name);
    } else {
        printf("It's a tie!\n");
    }
}


// OWNER SPECIFIC FUNCTIONS
// Add new owner
void openPokedexMenu() {
    char* name;
    int starter, id;

    // Get name
    printf("Your name: ");
    name = getDynamicInput();

    // Validate name not exist
    if(ownerHead && findOwnerByName(name)) {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        free(name);
        return;
    }

    // Choose starter
    printf("Choose Starter:\n\
    1. Bulbasaur\n\
    2. Charmander\n\
    3. Squirtle\n");
    starter = readIntSafe("Your choice: ");
    // Get Pokemon ID by user choice
    id = 3 * starter - 3;

    PokemonData* data;
    PokemonNode* pokemon;
    
    // Gather Pokemon data
    data = (PokemonData*) malloc(sizeof(PokemonData));
    *data = pokedex[id];

    // Create new Pokemon node
    pokemon = createPokemonNode(data);

    // Create new owner with starter
    createOwner(name, pokemon);
}

// Delete owner
void deletePokedex(void) {
    // Validate owners exists
    if(!ownerHead) {
        printf("No existing Pokedexes to delete.\n");
        return;
    }

    // Print all owners
    printf("\n=== Delete a Pokedex ===\n");
    printAllOwners();

    // Get input
    int index = readIntSafe("Choose a Pokedex to delete by number: ");
    // Find requested owner
    OwnerNode* owner = findOwnerByIndex(index);

    // Delete owner
    printf("Deleting %s's entire Pokedex...\n", owner->ownerName);
    freeOwnerNode(owner);
    printf("Pokedex deleted.\n");
}

// Delete all existing owners
void freeAllOwners() {
    // Validate owners exist
    if(!ownerHead) {
        return;
    }
    OwnerNode* owner = ownerHead;
    OwnerNode* temp;

    // Continue while there are owners to delete
    while(1) {
        if(owner == owner->next) {
            freeOwnerNode(owner);
            ownerHead = NULL;
            break;
        }
        // Free owner and iterate to next
        temp = owner;
        owner = owner->next;
        freeOwnerNode(temp);
    }
}

// Merge one owner to another
void mergePokedexMenu(void) {
    // Validate owners exist
    if(!ownerHead || (ownerHead && ownerHead->next == ownerHead)) {
        printf("Not enough owners to merge.\n");
        return;
    }

    // Get input
    printf("\n=== Merge Pokedexes ===\n");
    printf("Enter name of first owner: ");
    char* name1 = getDynamicInput();
    printf("Enter name of second owner: ");
    char* name2 = getDynamicInput();

    // Find requested owners
    OwnerNode* owner1 = findOwnerByName(name1);
    OwnerNode* owner2 = findOwnerByName(name2);

    printf("Merging %s and %s...\n", name1, name2);

    // Create array for second owner pokedex
    NodeArray* array = initNodeArray(5);
    collectAll(owner2->pokedexRoot, array);

    // Add each Pokemon to first owner
    for(int i = 0; i < array->size; i++) {
        // Validate Pokemon not allready in first owner
        if(!searchPokemonBFS(owner1->pokedexRoot, array->nodes[i]->data->id)) {
            PokemonNode* pokemon = array->nodes[i];
            pokemon->left = NULL;
            pokemon->right = NULL;
            insertPokemonNode(owner1->pokedexRoot, pokemon);
        } else {
            // If Pokemon allready in first owner free it
            freePokemonNode(array->nodes[i]);
        }
    }
    printf("Merge completed.\n");

    // Free all not needed data
    owner2->pokedexRoot = NULL;
    freeOwnerNode(owner2);
    printf("Owner '%s' has been removed after merging.\n", name2);
    free(name1);
    free(name2);
    free(array->nodes);
    free(array);
}

// Sort owners alphabetically
void sortOwners(void) {
    // Check if at least 2 owners exist
    if(!ownerHead || (ownerHead && ownerHead->next == ownerHead)) {
        printf("0 or 1 owners only => no need to sort.\n");
        return;
    }

    // Find min name owner
    OwnerNode* firstOwner = findMinOwnerName();
    // Remove from current owners kist
    removeOwnerFromCircularList(firstOwner);

    OwnerNode* owner = firstOwner;
    OwnerNode* newMinOwner = NULL;
    // If more owners exist add the to new list
    while(ownerHead) {
        // Find second min name owner
        newMinOwner = findMinOwnerName();
        removeOwnerFromCircularList(newMinOwner);
        owner->next = newMinOwner;
        newMinOwner->prev = owner;
        owner = newMinOwner;
    }
    // If another owner found add to new list
    if(newMinOwner) {
        newMinOwner->next = firstOwner;
        firstOwner->prev = newMinOwner;
    }
    // When old list empty make new list head
    ownerHead = firstOwner;

    printf("Owners sorted by name.\n");
}


// DATA STRUCTURES MANIPULATING FUNCTIONS
// Insert new Pokemon node to pokedex tree
PokemonNode* insertPokemonNode(PokemonNode* root, PokemonNode* newNode) {  
    int id = newNode->data->id;

    if(root->data->id > id) {
        if(!root->left) {
            root->left = newNode;
            return newNode;
        }
        return insertPokemonNode(root->left, newNode);
    } else {
        if(!root->right) {
            root->right = newNode;
            return newNode;
        }
        return insertPokemonNode(root->right, newNode);
    }
}

// Remove Pokemon from pokedex tree
void removeNodeBST(PokemonNode* root, PokemonNode* parent, OwnerNode* owner, int id) {
    if(!root) {
        return;
    }

    PokemonNode* temp;
    InsertLocation location;

    // Check if ID to remove is in this node
    if(root->data->id == id) {
        temp = root;
        location = ROOT;
    } else if(root->left && root->left->data->id == id) {
        temp = root->left;
        location = LEFT;
    } else if(root->right && root->right->data->id == id) {
        temp = root->right;
        location = RIGHT;
    // If ID not here iterate to next one based on ID
    } else if(root->data->id > id) {
        removeNodeBST(root->left, root, owner, id);
        return;
    } else {
        removeNodeBST(root->right, root, owner, id);
        return;
    }

    // Wire correct node to parent of deleted node
    if(temp->right) {
        if(location == RIGHT) {
            parent->right = temp->right;
        } else if(location == LEFT) {
            parent->left = temp->right;
        } else {
            owner->pokedexRoot = temp->right;
        }
        if(temp->left) {
            insertPokemonNode(parent, temp->left);
        }
    } else if (temp->left) {
        if(location == RIGHT) {
            parent->right = temp->left;
        } else if(location == LEFT) {
            parent->left = temp->left;
        } else {
            owner->pokedexRoot = temp->left;
        }
    } else {
        if(location == RIGHT) {
            parent->right = NULL;
        } else if(location == LEFT) {
            parent->left = NULL;
        } else {
            owner->pokedexRoot = NULL;
        }
    }

    // Clear and free node
    temp->right = NULL;
    temp->left = NULL;
    freePokemonNode(temp);
}

// Rewire owners list for a new owner
void linkOwnerInCircularList(OwnerNode* newOwner) {
    // If this is the first make it head
    if(!ownerHead) {
        newOwner->next = newOwner;
        newOwner->prev = newOwner;
        ownerHead = newOwner;
    } else {
        ownerHead->prev->next = newOwner;
        newOwner->prev = ownerHead->prev;
        ownerHead->prev = newOwner;
        newOwner->next = ownerHead;
    }

    return;
}

// Rewire owners to remove owner
void removeOwnerFromCircularList(OwnerNode* owner) {
    // If removed owner is the head
    if(ownerHead == owner) {
        // If this is the only owner
        if(owner->next == owner) {
            ownerHead = NULL;
        } else {
            // Rewire new head
            owner->prev->next = owner->next;
            owner->next->prev = owner->prev;
            ownerHead = owner->next;
        }
    } else {
        // Rewire first and last owners
        owner->prev->next = owner->next;
        owner->next->prev = owner->prev;
    }
}


// SUB-MENUS
// Enter owner pokedex
void enterExistingPokedexMenu() {
    // Validate owners exist
    if(!ownerHead) {
        printf("No existing Pokedexes.\n");
        return;
    }

    // Print all owners
    printf("\nExisting Pokedexes:\n");
    printAllOwners();

    // Get input
    int index = readIntSafe("Choose a Pokedex by number:");

    // Find requested owner
    OwnerNode* owner = findOwnerByIndex(index);

    printf("\nEntering %s's Pokedex...\n", owner->ownerName);

    int subChoice;
    do {
        // Pokedex menu
        printf("\n-- %s's Pokedex Menu --\n", owner->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");
        // Get input
        subChoice = readIntSafe("Your choice: ");

        // Direct function base on input
        switch(subChoice) {
        case 1:
            addPokemon(owner);
            break;
        case 2:
            displayMenu(owner);
            break;
        case 3:
            freePokemon(owner);
            break;
        case 4:
            pokemonFight(owner);
            break;
        case 5:
            evolvePokemon(owner);
            break;
        case 6:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while(subChoice != 6);
}

// Display Pokemons in pokedex
void displayMenu(OwnerNode* owner) {
    // Validate pokedex exist
    if (!owner->pokedexRoot) {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");
    // Get input
    int choice = readIntSafe("Your choice: ");

    // Direct display type base on input
    switch (choice) {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
        preOrderTraversal(owner->pokedexRoot);
        break;
    case 3:
        inOrderTraversal(owner->pokedexRoot);
        break;
    case 4:
        postOrderTraversal(owner->pokedexRoot);
        break;
    case 5:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default:
        printf("Invalid choice.\n");
    }
}


// CREATE AND FREE
// Create a new owner
OwnerNode* createOwner(char* ownerName, PokemonNode* starter) {
    // Allocate memory for new owner
    OwnerNode* newOwner = (OwnerNode*) malloc(sizeof(OwnerNode));

    // Insert new owner data
    newOwner->ownerName = ownerName;
    newOwner->pokedexRoot = starter;

    // Add new owner to the list
    linkOwnerInCircularList(newOwner);
    printf("New Pokedex created for %s with starter %s.\n", newOwner->ownerName, starter->data->name);
    
    return newOwner;
}

// Free owner data
void freeOwnerNode(OwnerNode* owner) {
    // Rewire owners list to remove owner
    removeOwnerFromCircularList(owner);

    // Free all owner data
    freePokemonNode(owner->pokedexRoot);
    free(owner->ownerName);
    free(owner);
}

// Create a new Pokemon node
PokemonNode* createPokemonNode(PokemonData* data) {
    // Allocate memory for new Pokemon
    PokemonNode* pokemon = malloc(sizeof(PokemonNode));
    // Insert new Pokemon data
    pokemon->data = data;
    pokemon->left = NULL;
    pokemon->right = NULL;

    return pokemon;
}

// Free Pokemon data
void freePokemonNode(PokemonNode* node) {
    if(!node) {
        return;
    }
    // Check left
    if(node->left) {
        // Free tree from left
        freePokemonNode(node->left);
        node->left = NULL;
    }
    // Check right
    if(node->right) {
        // Free tree from right
        freePokemonNode(node->right);
        node->right = NULL;
    }
    // Free node and data
    free(node->data);
    free(node);

    return;
}

// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu() {
    int choice;
    do {
        // Print main menu
        printf("\n=== Main Menu ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        // Get input
        choice = readIntSafe("Your choice: ");

        // Pass to correct function based on input
        switch (choice) {
        case 1:
            openPokedexMenu();
            break;
        case 2:
            enterExistingPokedexMenu();
            break;
        case 3:
            deletePokedex();
            break;
        case 4:
            mergePokedexMenu();
            break;
        case 5:
            sortOwners();
            break;
        case 6:
            printOwnersCircular();
            break;
        case 7:
            printf("Goodbye!\n");
            break;
        default:
            printf("Invalid.\n");
        }
    } while (choice != 7);
}

int main() {
    // Get to main menu
    mainMenu();
    // Free all owners on exit
    freeAllOwners();
    return 0;
}
