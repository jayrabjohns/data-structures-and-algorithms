/*
 ============================================================================
 Name        : CWK2Q7.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement a Columnar Transposition Cipher in C to encrypt a message of any
 length. A Columnar Transposition Cipher is transposition cipher that follows
 a simple rule for mixing up the characters in the plaintext to form the
 ciphertext.

 As an example, to encrypt the message ATTACKATDAWN with the keyword KEYS,
 we first write out our message as shown below,
	K	E	Y	S
	A	T	T	A
	C	K	A	T
	D	A	W	N

 Note: if the message to encode does not fit into the grid, you should pad
 the message with x's or random characters for example, ATTACKNOW with the
 keyword KEYS might look like below,
	K	E	Y	S
	A	T	T	A
	C	K	N	O
	W	X	X	X

 Once you have constructed your table, the columns are now reordered such
 that the letters in the keyword are ordered alphabetically,
	E	K	S	Y
	T	A	A	T
	K	C	T	A
	A	D	N	W

 The ciphertext is now read off along the columns, so in our example above,
 the ciphertext is TAATKCTAADNW.

 You should demonstrate your implementation by encrypting the file in the
 folder Q7 using the keyword - LOVELACE.

 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Required functions from the spec
void encrypt_columnar(const char *message_filename, const char *key, char **result);

// Additional functions
char * read_all_text_without_whitespace(const char *filename);
int is_upper_case(char chr);

void encrypt_columnar(const char *message_filename, const char *key, char **result)
{
	char *file_contents = read_all_text_without_whitespace(message_filename);
	size_t keyLen = strlen(key);
	size_t fileLength = strlen(file_contents);
	size_t rowCount = (fileLength / keyLen) + ((fileLength % keyLen) != 0);// equivalent to math.ceil
	
	// Creating table columns
	char **table = (char **) malloc(rowCount * sizeof(char *));
	for (int i = 0; i < rowCount; i++)
	{
		table[i] = (char *) malloc(keyLen * sizeof(char));
	}
	
	// Filling table with characters
	size_t contentsLen = strlen(file_contents);
	for (int row = 0, charCount = 0; row < rowCount; row++)
	{
		for (int col = 0; col < keyLen; col++, charCount++)
		{
			table[row][col] = (char)(charCount < contentsLen ? file_contents[charCount] : 'X');
		}
	}
	
	// Initialising column order
	int *colOrder = (int *) malloc(keyLen * sizeof(int));
	for (int i = 0; i < keyLen; i++)
	{
		colOrder[i] = i;
	}
	
	// Calculating intended column order with bubble sort
	for (int i = 0; i < keyLen - 1; i++)
	{
		int changed = 0;
		for (int j = 0; j < keyLen - 1 - i; j++)
		{
			int val1 = is_upper_case(key[colOrder[j]]) ? 'a' - 'A' + key[colOrder[j]] : key[colOrder[j]];
			int val2 = is_upper_case(key[colOrder[j + 1]]) ? 'a' - 'A' + key[colOrder[j + 1]] : key[colOrder[j + 1]];
			
			// Swapping values
			if (val1 > val2)
			{
				int temp = colOrder[j];
				colOrder[j] = colOrder[j + 1];
				colOrder[j + 1] = temp;
				changed = 1;
			}
		}
		
		// Early out
		if (!changed)
		{
			break;
		}
	}
	
	// Creating new encrypted string.
	char *ret = (char *) malloc(((keyLen * rowCount) + 1) * sizeof(char));
	for (int i = 0, charCount = 0; i < rowCount; i++)
	{
		// Copying values
		for (int j = 0; j < keyLen; j++, charCount++)
		{
			ret[charCount] = table[i][colOrder[j]];
		}
	}
	ret[keyLen * rowCount] = '\0';
	*result = ret;
	
	// Freeing resources
	for (int i = 0; i < rowCount; ++i)
	{
		free(table[i]);
	}
	free(table);
	free(colOrder);
}

/**
 * Reads all text from a given text file and removes whitespace & newlines.
 *
 * @param filename The name of the file to read.
 * @return Buffer of correct size containing all text from the given file
 */
char * read_all_text_without_whitespace(const char *filename)
{
	size_t maxContentsLen = 128 + 1;
	char *contents = (char *) malloc(maxContentsLen * sizeof(char));
	FILE *fileP = fopen(filename, "r");
	
	// Checking file exists
	if (!fileP)
	{
		perror(filename);
		exit(1);
	}
	
	char chr;
	size_t charsRead = 0;
	
	// Read chars from file, resizing buffer if needed
	while ((chr = (char) getc(fileP)) != EOF)
	{
		if (charsRead >= maxContentsLen - 1)
		{
			maxContentsLen = (maxContentsLen - 1) * 2 + 1;
			contents = (char *) realloc(contents, maxContentsLen * sizeof(char));
			if (!contents)
			{
				fprintf(stderr, "Can't read file '%s', not enough memory.", filename);
				exit(1);
			}
		}
		
		// Including non whitespace characters
		if (chr != '\n' && chr != '\r' && chr != ' ')
		{
			contents[charsRead] = chr;
			charsRead++;
		}
	}
	
	contents[charsRead] = '\0';
	fclose(fileP);
	
	// Resizing to correct size;
	contents = realloc(contents, (charsRead + 1) * sizeof(char));
	
	return contents;
}

/**
 * Checks if an ASCII character is upper case.
 *
 * @param chr Character to check.
 * @return 1 if upper case, 0 otherwise.
 */
int is_upper_case(const char chr)
{
	return chr >= 'A' && chr <= 'Z';
}

int main(int argc, char *argv[]) 
{
	const char *example_message = "./text.txt";
	const char *example_key = "LOVELACE";
	char *encrypted_message = NULL;
	encrypt_columnar(example_message, example_key, &encrypted_message);
	printf("Encrypted message = %s\n", encrypted_message);
	
	// insert more code here :-)
	free(encrypted_message);
	
	return EXIT_SUCCESS;
}
