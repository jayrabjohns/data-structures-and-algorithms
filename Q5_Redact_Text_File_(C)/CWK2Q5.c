/*
 ============================================================================
 Name        : CWK2Q5.c
 Author      : Anonymous (DO NOT CHANGE)
 Description :
 Implement an algorithm in C which given a file containing a block of text as
 input, redacts all words from a given set of “redactable” words (also from a
 file), and outputs the result to a file called “result.txt”. For example,
 given the block of text:
    The quick brown fox jumps over the lazy dog

 and the redactable set of words:
    the, jumps, lazy

 the output text in “result.txt” should be
    *** quick brown fox ***** over *** **** dog

 Note that the number of stars in the redacted text is the same as the number
 of letters in the word that has been redacted, and that capitalization is
 ignored. You should not use any of the string libraries to answer this
 question. You should also test your program using the example files
 provided.
 ============================================================================
*/

#include <stdio.h>
#include <stdlib.h>

// Required functions from the spec
void redact_words(const char *text_filename, const char *redact_words_filename);

// Additional functions
char ** read_all_words(const char *filename, int include_delimiters);
int write_all_text(const char *filename, char **contents, size_t contents_len);
void free_tokens(char **tokens, size_t size);
int string_equals(const char *str1, const char *str2, int ignore_case);
void redacted_string(char **to_redact, char redact_character);

long max_tokens = 0;

void redact_words(const char *text_filename, const char *redact_words_filename)
{
	char **tokens = read_all_words(text_filename, 1);
	long tokens_len = max_tokens;
	
	char **redacted_words = read_all_words(redact_words_filename, 0);
	long redacted_words_len = max_tokens;
	
	// Redacting all words in tokens also found in redacted_words
	for (int i = 0; i < tokens_len; i++)
	{
		for (int j = 0; j < redacted_words_len; j++)
		{
			if (string_equals(tokens[i], redacted_words[j], 1))
			{
				int x = 1;
			}
			
			if (string_equals(tokens[i], redacted_words[j], 1))
			{
				redacted_string(&tokens[i], '*');
				break;
			}
		}

//		printf("Token %d:'%s'\n", i, tokens[i]);
//		fflush(stdout);
	}
	
	write_all_text("./result.txt", tokens, tokens_len);
	
	// Freeing resources
	free_tokens(tokens, tokens_len);
	free_tokens(redacted_words, redacted_words_len);
}

/**
 * Reads all words from a given file.
 * Words are defined as strings separated by common punctuation / new line.
 *
 * @param filename File to read from.
 * @param include_delimiters Whether to include delimiters as tokens.
 * @return char ** of size max_tokens containing all words. Returns NULL if there isn't enough memory, or if the file isn't readable.
 */
char ** read_all_words(const char *filename, int include_delimiters)
{
	// Opening file and checking it exists
	FILE *fileP = fopen(filename, "r");
	if (fileP == NULL)
	{
		perror(filename);
		return NULL;
	}
	
	max_tokens = 64L;
	size_t max_token_length = 32 + 1;
	
	// Initialising token buffer
	char **tokens = (char **) malloc(sizeof(char *) * max_tokens);
	if (tokens == NULL)
	{
		fprintf(stderr, "Cannot read file '%s', not enough memory.", filename);
		return NULL;
	}
	
	// Initialising each token
	for (int i = 0; i < max_tokens; i++)
	{
		tokens[i] = (char *) malloc(sizeof(char) * max_token_length);
		if (tokens[i] == NULL)
		{
			fprintf(stderr, "Cannot read file '%s', not enough memory.", filename);
			if (i - 1 >= 0)
			{
				free_tokens(tokens, i - 1);
			}
			return NULL;
		}
	}
	
	char chr;
	size_t char_count = 0;
	long token_count = 0;
	int last_token_was_delimiter = 0;
	
	// Read chars from file, resizing buffer if needed
	while ((chr = (char) getc(fileP)) != EOF)
	{
		// Allocating more memory when max number of tokens is exceeded
		// Since we may be saving 2 tokens at a time, we need to make sure there is always at least space for 2 tokens
		if (token_count >= max_tokens - 1)
		{
			// Size doubles
			long new_max_tokens = max_tokens * 2;
			char **new_tokens = (char **) realloc(tokens, /*sizeof(*tokens)*/sizeof(char *) * new_max_tokens);
			if (new_tokens == NULL)
			{
				fprintf(stderr, "Cannot read file '%s', not enough memory.", filename);
				free_tokens(tokens, max_tokens);
				return NULL;
			}
			
			for (size_t i = max_tokens; i < new_max_tokens; i++)
			{
				new_tokens[i] = (char *) malloc(sizeof(char) * max_token_length);
				if (new_tokens[i] == NULL)
				{
					fprintf(stderr, "Cannot read file '%s', not enough memory.", filename);
					free_tokens(tokens, max_tokens + i);
					return NULL;
				}
			}
			
			tokens = new_tokens;
			max_tokens = new_max_tokens;
		}
		
		// Allocating more memory when max number of characters read for this token.
		if (char_count >= max_token_length - 1)
		{
			// Size doubles, leaving one character for null terminator
			size_t new_max_token_length = (max_token_length - 1) * 2 + 1;
			char *new_token = (char *) realloc(tokens[token_count], sizeof(char) * new_max_token_length);
			
			// Checking if there was enough memory
			if (new_token == NULL)
			{
				fprintf(stderr, "Cannot read file '%s', not enough memory.", filename);
				free_tokens(tokens, max_tokens);
				return NULL;
			}
			
			tokens[token_count] = new_token;
			max_token_length = new_max_token_length;
		}
		
		// Split at specified delimiters
		if (chr == '\n' || chr == ' ' || chr == ',' || chr == '.' || chr == '-' || chr == '?' || chr == '!' || chr == '(' || chr == ')')
		{
			// Adding non delimiter characters
			if (!last_token_was_delimiter)
			{
				// Adding null terminator
				tokens[token_count][char_count++] = '\0';
				
				// Shrinking to correct size
				if (char_count < max_token_length)
				{
					tokens[token_count] = realloc(tokens[token_count], char_count);
				}
				
				token_count++;
				char_count = 0;
			}
			
			// Adding delimiters
			if (include_delimiters)
			{
				// Storing delimiter as new string
				tokens[token_count][0] = chr;
				tokens[token_count][1] = '\0';
				
				// Shrinking to correct size
				if (2 < max_token_length)
				{
					tokens[token_count] = realloc(tokens[token_count], sizeof(char) * 2);
				}
				
				token_count++;
				last_token_was_delimiter = 1;
			}
		}
		else
		{
			// Adding char to token
			tokens[token_count][char_count++] = chr;
			last_token_was_delimiter = 0;
		}
	}
	
	// Shrinking to actual size being used
	if (token_count < max_tokens)
	{
		for (size_t i = token_count; i < max_tokens; i++)
		{
			free(tokens[i]);
		}
		
		tokens = (char **) realloc(tokens, sizeof(char *) * token_count);
		max_tokens = token_count;
	}
	
	fclose(fileP);
	return tokens;
}

/**
 * Writes all strings to a given file sequentially without added new lines.
 *
 * @param filename File to write to.
 * @param contents String to write to file.
 * @param contents_len The number of string to write.
 * @return 1 if contents was successfully written to file. 0 otherwise.
 */
int write_all_text(const char *filename, char **contents, size_t contents_len)
{
	// Creates new file if one doesn't exist, and opens it for writing
	FILE *fileP = fopen(filename, "wb");
	if (fileP == NULL)
	{
		perror(filename);
		return 0;
	}
	
	// Writing all strings to file
	for (int i = 0; i < contents_len; ++i)
	{
		fprintf(fileP, "%s", contents[i]);
	}
	
	fflush(fileP);
	fclose(fileP);
	return 1;
}

/**
 * Frees all memory associated with a char **.
 *
 * @param tokens char ** to free.
 * @param size Size of the given char **.
 */
void free_tokens(char **tokens, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		free(tokens[i]);
	}
	free(tokens);
}

/**
 * Checks if an ASCII character is upper case.
 *
 * @param chr Character to check.
 * @return 1 if upper case, 0 otherwise.
 */
int is_upper_case(char chr)
{
	return chr >= 'A' && chr <= 'Z';
}

/**
 * Checks whether two strings have the same value.
 *
 * @param str1 String to compare.
 * @param str2 String to compare.
 * @param ignore_case Whether case should be ignored.
 * @return 1 if both strings have the same value, 0 otherwise (including if they're both null).
 */
int string_equals(const char *str1, const char *str2, int ignore_case)
{
	int equal = 0;
	
	if (str1 != NULL && str2 != NULL)
	{
		int char_pos = 0;
		equal = 1;
		
		// Runs through both strings checking each character is the same.
		// str1[char_pos] != '\0' && str2[char_pos] != '\0'
		while (str1[char_pos] != '\0' && str2[char_pos] != '\0')
		{
			char chr1 = (char)(ignore_case && is_upper_case(str1[char_pos]) ? (str1[char_pos] + 'a' - 'A') : str1[char_pos]);
			char chr2 = (char)(ignore_case && is_upper_case(str2[char_pos]) ? (str2[char_pos] + 'a' - 'A') : str2[char_pos]);
			
			if(chr1 == chr2)
			{
				char_pos++;
			}
			else if (chr1 != chr2
			|| chr1 == '\0' && chr2 != '\0'
			|| chr1 != '\0' && chr2 == '\0')
			{
				equal = 0;
				break;
			}
		}
		
		// Checking if strings were same length
		if (str1[char_pos] == '\0' && str2[char_pos] != '\0'
			  || str1[char_pos] != '\0' && str2[char_pos] == '\0')
		{
			equal = 0;
		}
	}
	
	return equal;
}

/**
 * Replaces all non space characters in a given string with a given character.
 *
 * @param to_redact String to redact.
 * @param redact_character Character to replace with.
 */
void redacted_string(char **to_redact, char redact_character)
{
	if (*to_redact != NULL)
	{
		size_t char_count = 0;
		while ((*to_redact)[char_count] != '\0')
		{
			if ((*to_redact)[char_count] != ' ')
			{
				(*to_redact)[char_count] = redact_character;
			}
			char_count++;
		}
	}
}

int main(int argc, char *argv[])
{
	const char *input_file = "./debate.txt";
	const char *redact_file = "./redact.txt";
	redact_words(input_file, redact_file);
	return EXIT_SUCCESS;
}
