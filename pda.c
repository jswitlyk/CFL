#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>



/*structs for SLL stack*/
struct Node { //each node contains a character and pointer to next node
    char item; 
    struct Node *next;
};

struct List {
    struct Node *head;
    struct Node *tail;
};

struct List newListJjs62() {
    struct List list;
    list.head = NULL;
    list.tail = NULL;
    return list;
}

/*prototype functions*/
int popPushJjs62(struct List *list, char popChar, char pushChar);
int isOperatorJjs62(char character);
int isEmptyJjs62(struct List *list);
char popJjs62(struct List *list);
void pushJjs62(struct List *list, char item);
void clearJjs62(struct List *list);

int main(int argc, char **argv) {

    /*print course info, etc.*/
    printf("Project 1 for CS 341\n");
    printf("Section number: 010\n");
    printf("Semester: Spring 2024\n");
    printf("Written by Jeremy Switlyk (jjs62)\n");
    printf("Instructor: Marvin Nakayama, marvin@njit.edu\n\n");

    /*the number of strings to process*/
    int n;

    /*index to track progress*/
    int strIdx = 1; 

    /*//ascii character to represent epsilon*/
    const char EPSILON = '#';

    /*enumerate all possible states in the PDA*/
    typedef enum pdaState {Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7, Q8, Q9, CRASH} pdaState;

    /*names of states for printing*/
    const char *stateNames[] = {"Q0", "Q1", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8", "Q9", "CRASH"};

    /*variable to store current enumerated state*/
    pdaState state;

    /*initialize stack*/
    struct List stack = newListJjs62();

    

    /*will be true if user has passed arguments*/
    if(argc > 1)
        /*assign passed argument to n*/
        n = atoi(argv[1]);
    else {
        /*prompt user for number of strings to process otherwise*/
        printf("Enter the number of strings to process, n, where n > 0 ");
        printf("or n = 0 to exit: ");
        scanf("%d", &n); //assign scanned user input to n
    }
     if( n == 0) 
        return 0; //exit with code 0 if the user has entered a 0
    printf("n = %d\n", n); //print chosen value of n
     char string[50]; //character array to hold current string
    for(int i = 1; i <= n; i++){ //for each string
        if(argc > 1)
            strcpy(string, argv[i]); //copy string to char array if they were passed as arguments
        else {
            printf("Enter a string: \n"); //prompt user to enter a string to process if they were not passed as arguments
            scanf("%s", string); //assign the entered string to the char array
        }
        printf("Processing String (%d / %d): %s\n", strIdx, n, string); //print progress
        state = Q0; //starting state
        for(int j = 0; j < strlen(string) + 1; j++) { //for each character in the current string
            printf("\nCurrent State: %s\n", stateNames[state]); //print current state
            printf("Read: %c\n", string[j]); //print read character
            switch(state) { // switch with cases for each state
                case Q0:
                    if(string[j] == 'a') {
                        popPushJjs62(&stack, EPSILON, 'a'); //pop epsilon, push 'a'
                        state = Q1;
                        printf("State After Transition: %s\n", stateNames[state]); //print next state
                    }
                    else state = CRASH; //crash if no valid symbols can be read
                    break;
                case Q1:
                    switch(string[j]) {
                        case 'b':
                            popPushJjs62(&stack, EPSILON, 'b'); //push b
                            printf("State After Transition: %s\n", stateNames[state]);
                            break;
                        case 'a':
                            popPushJjs62(&stack, EPSILON, 'a'); //push a
                            state = Q2;
                            printf("State After Transition: %s\n", stateNames[state]);
                            break;
                        default: state = CRASH;
                    }
                    break;
                case Q2:
                    if(string[j] == '(')
                        popPushJjs62(&stack, EPSILON, '('); //push (
                    else if(isdigit(string[j])) {
                        popPushJjs62(&stack, EPSILON, EPSILON);
                        state = Q3;
                    }
                    else if(string[j] == '.') {
                        popPushJjs62(&stack, EPSILON, EPSILON); //pushes and pops nothing, but prints appropriate messages
                        state = Q7;
                    }
                    else state = CRASH;
                    printf("State After Transition: %s\n", stateNames[state]);
                    break;
                case Q3:
                    if(isdigit(string[j]))
                        popPushJjs62(&stack, EPSILON, EPSILON);
                    else if(string[j] == '.') {
                        popPushJjs62(&stack, EPSILON, EPSILON);
                        state = Q4;
                    }
                    else state = CRASH;
                    printf("State After Transition: %s\n", stateNames[state]);
                    break;
                case Q4:
                    if(isdigit(string[j]))
                        popPushJjs62(&stack, EPSILON, EPSILON);
                    else if(string[j] == ')') {
                        if(popPushJjs62(&stack, '(', EPSILON) == 1) //check for errors whenever popping from stack
                            state = CRASH; //set state to crash if function returns error code 1
                        else state = Q9;
                    }
                    else if(isOperatorJjs62(string[j])) {
                        popPushJjs62(&stack, EPSILON, EPSILON);
                        state = Q2;
                    }
                    else if (string[j] == 'a') {
                        if(popPushJjs62(&stack, 'a', EPSILON) == 1) //pop a, check for error code
                            state = CRASH;
                        else state = Q5;
                    }
                    else state = CRASH;
                    printf("State After Transition: %s\n", stateNames[state]);
                    break;
                case Q5:
                    switch(string[j]) {
                        case 'b':
                            if(popPushJjs62(&stack, 'b', EPSILON) == 1) //pop b, check for error code
                                state = CRASH;
                            printf("State After Transition: %s\n", stateNames[state]);
                            break;
                        case 'a':
                            if(popPushJjs62(&stack, 'a', EPSILON) == 1) //pop a, check for error code
                                state = CRASH;
                            else state = Q6;
                            printf("State After Transition: %s\n", stateNames[state]);
                            break;
                        default: state = CRASH;
                    }
                    break;
                case Q7:
                    if(isdigit(string[j])) {
                        popPushJjs62(&stack, EPSILON, EPSILON); //push and pop epsilon and print messages
                        state = Q8;
                    }
                    else state = CRASH;
                    printf("State After Transition: %s\n", stateNames[state]);
                    break;
                case Q8:
                    if(isdigit(string[j]))
                        popPushJjs62(&stack, EPSILON, EPSILON);
                    else if(string[j] == ')') {
                        if(popPushJjs62(&stack, '(', EPSILON) == 1) //pop ( and check for error
                            state = CRASH;
                        else state = Q9;
                    }
                    else if(isOperatorJjs62(string[j])) {
                        popPushJjs62(&stack, EPSILON, EPSILON);
                        state = Q2;
                    }
                    else if (string[j] == 'a') {
                        if(popPushJjs62(&stack, 'a', EPSILON) == 1) //pop a and check for error
                            state = CRASH;
                        else state = Q5;
                    }
                    else state = CRASH;
                    printf("State After Transition: %s\n", stateNames[state]);
                    break;
                case Q9:
                    if(string[j] == ')') {
                        if(popPushJjs62(&stack, '(', EPSILON) == 1) //pop ( and check for error
                            state = CRASH;
                    }
                    else if(isOperatorJjs62(string[j])) {
                        popPushJjs62(&stack, EPSILON, EPSILON);
                        state = Q2;
                    }
                    else if (string[j] == 'a') {
                        if(popPushJjs62(&stack, 'a', EPSILON) == 1) //pop a and check for error
                            state = CRASH;
                        else state = Q5;
                    }
                    else state = CRASH;
                    printf("State After Transition: %s\n", stateNames[state]);
                    break;
                case CRASH:
                    printf("The PDA has crashed.\n"); //print message if PDA crashes
                    break;
                }

                
            }
            if(state == Q6) {
                    if(!isEmptyJjs62(&stack))
                        printf("The String Has Been Rejected due to Non-empty Stack\n\n\n"); //check stack emptiness after processing string
                    else printf("String Accepted\n\n\n"); //accept string if stack is empty in Q6
            }
            else printf("The String Has Been Rejected due to a Crash\n\n\n"); //Print if string rejected due to crash
            clearJjs62(&stack); // empty stack and free memory
            strIdx++; //increment string index         
    }
    return 0;
}

int popPushJjs62(struct List *list, char popChar, char pushChar) {
    /*function that pops and pushes characters to the stack and returns error codes*/
    int error = 0;
    if(popChar != '#'){ //if char to pop is not epsilon
        if(isEmptyJjs62(list)) { //print error and set error code if attempting to pop an empty stack
            printf("Cannot Pop an Empty Stack\n"); 
            error = 1;
        }
        char top = popJjs62(list);
        if (top != popChar) { //print error and set error code if popped character is incorrect
            printf("Popped Character Does not Match Expected\n");
            error = 1;
        }
        else {
            printf("Popped: %c\n", popChar); //print if popped sucessfully
        }
    }
    else printf("Popped: Epsilon\n"); //print message if popping epsilon
    if(pushChar != '#'){
        pushJjs62(list, pushChar);
        printf("Pushed: %c\n", pushChar);
    }
    else printf("Pushed: Epsilon\n");
    return error;
}

int isOperatorJjs62(char character) { //check if character exists in the set of arithmetic operators, PSI
    /*the set of arithmetic operators*/
    const char PSI[] = {'+', '-', '*', '/'};
    for(size_t i = 0; i < sizeof(PSI) / sizeof(char); i++)
        if(character == PSI[i])
            return 1;
    return 0;
}

/*Stack Implementation*/

int isEmptyJjs62(struct List *list) { //check for empty list
    return list->head == NULL;
}

char popJjs62(struct List *list) {
    struct Node *node = list->head; //node to pop will be the head
    char item = node->item;
    list->head = node->next; //assign the next node as the new head node
    if(isEmptyJjs62(list))
        list->tail = NULL;
    free(node); //free memory when node is popped
    return item;
}

void pushJjs62(struct List *list, char item) {
    struct Node *node = malloc(sizeof(struct Node)); //allocate new node on the heap when item is pushed
    node->item = item;
    node->next = list->head; 
    if(isEmptyJjs62(list))
        list->tail = node;
    list->head = node;
}

void clearJjs62(struct List *list) { //pop nodes until list is empty
    while(!isEmptyJjs62(list))
        popJjs62(list);
}