/*
 ============================================================================
 Name        : CWK2Q3.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement your own Hash Table in C for storing and searching names, i.e. char
 arrays. In the event of collisions, you should use linear probing with an
 interval of 1. The hash function should be the sum of the ASCII values of the
 string modulo the size of the underlying data structure. Your Hash Table
 implementation should have the following interface:
	int hash_function(const char *key)
	void resize_map(int new_size)
	void add_to_map(const char *name)
	int remove_from_map(const char *name)
	int search_map(const char *name)
	void print_map()

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Required functions from the spec
int hash_function(const char *key);
void resize_map(int new_size);
void add_to_map(const char *name);
int remove_from_map(const char *name);
int search_map(const char *name);
void print_map();

// Additional functions
int next_free_space(int start, const char *value);
int get_index(const char *name);
void free_map(char **map, size_t size);


char **hash_map = NULL;  // this is where you should store your names
int hash_map_size = 0;
int populated_size = 0;
int max_name_size = 128;

int hash_function(const char *key)
{
    // Computing hash
    int sum = 0;
    int keyLen = strlen(key);
    for (int i = 0; i < keyLen; i++)
    {
        sum += key[i];
    }

    return hash_map_size > 0 ? sum % hash_map_size : -1;
}

void resize_map(int new_size)
{
	int final_size = new_size > populated_size ? new_size : populated_size;
	
	// Creating new map
	char **new_map = (char **) malloc(final_size * sizeof(char *));
	if (new_map == NULL)
	{
		fprintf(stderr, "Cannot resize to size %d, not enough memory.\n", new_size);
		return;
	}
	
	for (int i = 0; i < final_size; ++i)
	{
		new_map[i] = (char *) malloc(max_name_size * sizeof(char));
		if (new_map[i] == NULL)
		{
			fprintf(stderr, "Cannot resize to size %d, not enough memory.\n", new_size);
			free_map(new_map, i);
			return;
		}
		
		strcpy(new_map[i], "");
	}
	
	char **temp = hash_map;
	int old_size = hash_map_size;
	hash_map = new_map;
	hash_map_size = final_size;
	populated_size = 0;
	
	// Moving entries
	if (temp != NULL)
	{
		for (int i = 0; i < old_size; ++i)
		{
			if (temp[i] != NULL && strcmp(temp[i], "") != 0)
			{
				add_to_map(temp[i]);
			}
		}
		
		// Freeing old map
		free_map(temp, old_size);
	}
}

void add_to_map(const char *name)
{
	if (hash_map_size == 0)
	{
		resize_map(10);
	}
	
	// Resizing if load factor is greater than 0.7.
	if ((populated_size + 1) / (float)hash_map_size> 0.7f)
	{
		resize_map(hash_map_size * 2);
	}
	
    int index = hash_function(name);
	index = index != -1 ? next_free_space(index, name) : -1;
	
    if (index != -1 && hash_map != NULL && hash_map[index] != NULL)
    {
		strcpy(hash_map[index], name);
		populated_size++;
    }
}

int remove_from_map(const char *name)
{
    int index = get_index(name);
    
    if (index != -1 && hash_map != NULL && hash_map[index] != NULL && strcmp(hash_map[index], name) == 0)
    {
		strcpy(hash_map[index], "");
        populated_size--;
        return 1;
    }
    
    return 0;
}

int search_map(const char *name)
{
    return get_index(name) != -1 ? 1 : 0;
}

void print_map()
{
	if (hash_map != NULL)
	{
		puts("");
		for (int i = 0; i < hash_map_size; i++)
		{
			if (hash_map[i] != NULL && strcmp(hash_map[i], "") != 0)
			{
				puts(hash_map[i]);
			}
		}
		puts("");
		fflush(stdout);
	}
}

/**
 * Starting from a given point, gets the next free space using linear probing and an interval of 1.
 *
 * @param start Where to start searching
 * @return The next free space. Returns -1 if no space exists, or if value is a duplicate entry.
 */
int next_free_space(int start, const char *value)
{
	int free_space = -1;
	
	if (start < hash_map_size && hash_map != NULL)
	{
		for (int i = start, namesChecked = 0; namesChecked < hash_map_size + start; i++, namesChecked++)
		{
			// Repositioning at the start of the map
			if (i >= hash_map_size)
			{
				i = 0;
			}
			
			if (hash_map[i] != NULL)
			{
				// Checking if it's a duplicate
				if (strcmp(hash_map[i], value) == 0)
				{
					break;
				}
				else if (strcmp(hash_map[i], "") == 0)
				{
					free_space = i;
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	
	return free_space;
}

/**
 * Gets the index of a given name in the hash map
 *
 * @param name The name to find in the hash map
 * @return The names index. Returns -1 if the name does not exist.
 */
int get_index(const char *name)
{
	int index = hash_function(name);
	
	if (index != -1 && hash_map != NULL)
	{
		for (int i = index, namesChecked = 0; namesChecked < hash_map_size + index; i++, namesChecked++)
		{
			if (i >= hash_map_size)
			{
				i = 0;
			}
			
			if (hash_map[i] != NULL && strcmp(hash_map[i], name) == 0)
			{
				return i;
			}
		}
	}
	
	return -1;
}

/**
 * Frees all resources associated with a given map.
 *
 * @param map Map to free.
 * @param size Size of given map.
 */
void free_map(char **map, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		free(map[i]);
	}
	
	free(map);
}

int main(int argc, char *argv[])
{
    char *stringOne = "#Hello world";
    char *stringTwo = "How are you?";
    char *stringThree = "Be the best you...!!";
    char *stringFour = "Be kind to yourself";
    char *stringFive = "Principles of Programming 2";

    resize_map(6);
    add_to_map(stringOne);
    add_to_map(stringTwo);
    add_to_map(stringOne);
    add_to_map(stringThree);
    add_to_map(stringFour);
    add_to_map(stringFive);
    print_map();

    int ret = search_map(stringOne);
    if(ret)
        printf("Found %s!\n", stringOne);

    remove_from_map(stringThree);
    
    ret = search_map(stringFive);
    if(ret)
        printf("Found %s!\n", stringFive);
    print_map();

    add_to_map(stringThree);
    print_map();

    return EXIT_SUCCESS;
}
