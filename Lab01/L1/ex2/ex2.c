/*************************************
* Lab 1 Exercise 2
* Name: Ngo Duc Binh
* Matric No: A0180061U
* Lab Group: B01
*************************************/

#include <stdio.h>
#include <stdlib.h> //for malloc() and free()

//Declaration of a Linked List Node

typedef struct NODE{
    int data;
    struct NODE* next;
} node;

//Function Prototypes
node* insertAt(node*, int, int, int);

void printList(node*);
void destroyList(node*);



int main()
{
    node* myList = NULL;    //Empty List
    int position, input, copies;

    //Fill in code for input and processing
    while ((scanf("%i",&position) == 1)
            && (scanf("%i", &input) == 1)
            && (scanf("%i", &copies) == 1)) {
        myList = insertAt(myList, position, copies, input);
    }
    //Output code coded for you
    printf("My List:\n");
    printList(myList);
    
    destroyList(myList);
    myList = NULL;

    
    printf("My List After Destroy:\n");
    printList(myList);
 
    return 0;
}

//Actual Function Implementations
node* insertAt(node* head, int position, int copies, int newValue)
{
    node* ptr = head;
    node* prevPointer = head;
    int newHead = 1; // TRUE

    // reach the specified node
    for (int i = 0; i < position && ptr != NULL; i++) {
        // satisfying this means position to add > 0 and this is not an empty list, so there is not a new head
        if (i == 0) {
            newHead = 0;    
        }
        prevPointer = ptr;
        ptr = ptr->next;
    }

    for (int i = 0; i < copies; i++) {
        node* newNode = malloc(sizeof(node));
        newNode->data = newValue;
        newNode->next = ptr;
        ptr = newNode;
    }
    
    // ptr at this point always point to the last node being added to the list
    if (newHead == 1) {
        return ptr;
    } else {
        // connect the new list to the old list
        prevPointer->next = ptr;
        return head;
    }
}
 
void printList(node* head)
//Purpose: Print out the linked list content
//Assumption: the list is properly null terminated
{
    //This function has been implemented for you
    node* ptr = head;

    while (ptr != NULL)  {    //or you can write while(ptr)
        printf("%i ", ptr->data);
        ptr = ptr->next;
    }
    printf("\n");
}

void destroyList(node* head)
{
     
    //Fill in your code here
    //You can use the same implementation as in exercise 1
    
    while (head != NULL) {
        node* temp = head;
        head = head->next;
        free(temp);
    }
}
