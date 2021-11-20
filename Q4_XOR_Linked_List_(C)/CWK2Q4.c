/*
 ============================================================================
 Name        : CWK2Q4.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement your own XOR Linked List (https://en.wikipedia.org/wiki/XOR_linked_list)
 in C capable of storing names. Your implementation should have the following
 functions:
    void insert_string(const char* newObj)
	int insert_before(const char* before, const char* newObj)
	int insert_after(const char* after, const char* newObj)
	int remove_string(char* result)
	int remove_after(const char *after, char *result)
	int remove_before(const char *before, char *result)
    void print_list()

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// XOR linked list: https://www.geeksforgeeks.org/xor-linked-list-a-memory-efficient-doubly-linked-list-set-1/

typedef struct listNode
{
	char *name;
	struct listNode *npx; // XOR of next & previous
} listNode;

// Required functions from the spec
void insert_string(const char *newObj);
int insert_before(const char *before, const char *newObj);
int insert_after(const char *after, const char *newObj);
int remove_string(char *result);
int remove_after(const char *after, char *result);
int remove_before(const char *before, char *result);
void print_list();

// Additional functions
int insert_to_list(int startFromTail, const char *previousValue, const char *newObj);
int remove_next_from_list(int startFromTail, const char *previousValue, char *result);

struct listNode *head = NULL;
struct listNode *tail = NULL;

void insert_string(const char *newObj)
{
	insert_to_list(0, NULL, newObj);
}

int insert_before(const char *before, const char *newObj)
{
	return insert_to_list(1, before, newObj);
}

int insert_after(const char *after, const char *newObj)
{
	return insert_to_list(0, after, newObj);
}

int remove_string(char *result)
{
	return remove_next_from_list(0, NULL, result);
}

int remove_after(const char *after, char *result)
{
	return remove_next_from_list(0, after, result);
}

int remove_before(const char *before, char *result)
{
	return remove_next_from_list(1, before, result);
}

void print_list()
{
	if (head == NULL)
	{
		return;
	}
	
	struct listNode *current = head;
	struct listNode *previous = NULL;
	struct listNode *next;
	
	// Traverses list until reaching the end
	int firstNode = 1;
	while (current != NULL)
	{
		if (firstNode)
		{
			printf("%s", current->name);
			firstNode = 0;
		}
		else
		{
			printf(" -> %s", current->name);
		}
		
		// Next = prev XOR (npx of current node)
		next = (listNode *)((uintptr_t)previous ^ (uintptr_t)current->npx);
		previous = current;
		current = next;
	}
	printf("\n");
	fflush(stdout);
}

/**
 * Inserts a new node with a given value 'after' a node with another given value.
 * 'after' is relative to the given direction of traversal.
 *
 * @param startFromTail 0->start from head : 1->start from tail.
 * @param previousValue Value of node 'before' the node to insert in the given direction.
 * @param newObj Value of the new node.
 * @return Whether a node was successfully inserted.
 */
int insert_to_list(int startFromTail, const char *previousValue, const char *newObj)
{
	struct listNode *start = startFromTail ? tail : head;
	int maxSizeBytes = 64;
	int success = 0;
	
	// Name is too large
	if (sizeof(char) * (strlen(newObj) + 1) > maxSizeBytes)
	{
		printf("Cannot add names greater than %d bytes in size.\n", maxSizeBytes);
		return 0;
	}
	
	// Empty list
	if (start == NULL && previousValue != NULL)
	{
		printf("Cannot add %s. %s cannot exist in the list because it is empty!\n", newObj, previousValue);
		return 0;
	}
	
	// Pushing new node to start
	if (previousValue == NULL)
	{
		struct listNode *newNode = (struct listNode *) malloc(sizeof(struct listNode));
		newNode->name = (char *) malloc(sizeof(char) * (strlen(newObj) + 1));
		strcpy(newNode->name, newObj);
		newNode->npx = (struct listNode *) (0 ^ (uintptr_t)head);
	
		// Dealing with empty list
		if (head == NULL)
		{
			tail = newNode;
		}
		// Inserting node at start
		else
		{
			head->npx = (struct listNode *) ((uintptr_t)newNode ^ (0 ^ (uintptr_t)head->npx));
		}
	
		head = newNode;
		success = 1;
	}
	
	// Not empty list
	else
	{
		struct listNode *current = start;
		struct listNode *previous = NULL;
		struct listNode *next = (struct listNode *) ((uintptr_t)previous ^ (uintptr_t)current->npx);
		
		// Traversing list until the current node has the given value
		while (next != NULL && strcmp(current->name, previousValue) != 0)
		{
			previous = current;
			current = next;
			next = (struct listNode *) ((uintptr_t)previous ^ (uintptr_t)current->npx);
		}
		
		// Found node
		if (strcmp(current->name, previousValue) == 0)
		{
			struct listNode *newNode = (struct listNode *) malloc(sizeof(struct listNode));
			struct listNode *temp = (struct listNode *) ((uintptr_t)current->npx ^ (uintptr_t)next);
			current->npx = (struct listNode *) ((uintptr_t)temp ^ (uintptr_t)newNode);
			
			if (next != NULL)
			{
				next->npx = (struct listNode *) ((uintptr_t)newNode ^ (uintptr_t)next->npx ^ (uintptr_t)current);
			}
			
			newNode->npx = (struct listNode *) ((uintptr_t)current ^ (uintptr_t)next);
			newNode->name = (char *) malloc(sizeof(char) * (strlen(newObj) + 1));
			strcpy(newNode->name, newObj);
			
			success = 1;
		}
		else
		{
			printf("Cannot add %s, as %s does not exist in the list.\n", newObj, previousValue);
		}
	}
	
	return success;
}

/**
 * Removes a node with a given value 'after' a node with another given value.
 * 'after' is relative to the given direction of traversal.
 *
 * @param startFromTail 0->start from head : 1->start from tail.
 * @param previousValue Value of node 'before' the node to remove in the given direction.
 * @param result Value of the removed node.
 * @return Whether a node was successfully removed.
 */
int remove_next_from_list(int startFromTail, const char *previousValue, char *result)
{
	struct listNode *start = startFromTail ? tail : head;
	int success = 0;
	
	// Empty list
	if (start == NULL)
	{
		printf("Cannot remove node from list as it i empty.\n");
		return 0;
	}
	
	// Remove Node at beginning
	if (previousValue == NULL)
	{
		struct listNode *temp = start;
		if (startFromTail)
		{
			tail = (struct listNode *) ((uintptr_t)start->npx ^ (uintptr_t)NULL);
		}
		else
		{
			head = (struct listNode *) ((uintptr_t)start->npx ^ (uintptr_t)NULL);
		}
		
		if (head != NULL)
		{
			head->npx = (struct listNode *) ((uintptr_t)NULL ^ (uintptr_t)temp ^ (uintptr_t)head->npx);
		}
		
		strcpy(result, temp->name);
		free(temp->name);
		free(temp);
		success = 1;
	}
	else
	{
		// Not empty list
		struct listNode *current = start;
		struct listNode *previous = NULL;
		struct listNode *next = (struct listNode *) ((uintptr_t)previous ^ (uintptr_t)current->npx);
		
		// Traversing list until the previous node has the given value
		while (next != NULL && (previous == NULL || strcmp(previous->name, previousValue) != 0))
		{
			previous = current;
			current = next;
			next = (struct listNode *) ((uintptr_t)previous ^ (uintptr_t)current->npx);
		}
		
		// Found node
		if (previous != NULL && strcmp(previous->name, previousValue) == 0)
		{
			struct listNode *temp = current;
			previous->npx = (struct listNode *) ((uintptr_t)next ^ (uintptr_t)current ^ (uintptr_t)previous->npx);
			
			if (next != NULL)
			{
				next->npx = (struct listNode *) ((uintptr_t)previous ^ (uintptr_t)current ^ (uintptr_t)next->npx);
			}
			else
			{
				// Setting end because we removed last node
				if (startFromTail)
				{
					head = previous;
				}
				else
				{
					tail = previous;
				}
			}
			
			// Storing result
			strcpy(result, temp->name);
			
			// Freeing resources
			free(temp->name);
			free(temp);
			temp = NULL;
			
			success = 1;
		}
		else
		{
			printf("Cannot remove relative to %s as it does not exist in the list.\n", previousValue);
		}
	}
	
	return success;
}

int main(int argc, char *argv[])
{
	insert_string("Alpha");
	print_list();
	insert_string("Bravo");
	print_list();
	insert_string("Charlie");
	print_list();
	insert_after("Bravo", "Delta");
	print_list();
	insert_before("Alpha", "Echo");
	print_list(); // Charlie -> Bravo -> Delta -> Echo -> Alpha

	char result[10];
	int ret;

	ret = remove_after("Delta",result);
	if(ret)
		printf("Removed: %s\n", result);
	print_list();

	ret = remove_before("Bravo", result);
	if(ret)
		printf("Removed: %s\n", result);
	print_list();
	
	ret = remove_string(result);
	if(ret)
		printf("Removed: %s\n", result);
	
	print_list();
}
