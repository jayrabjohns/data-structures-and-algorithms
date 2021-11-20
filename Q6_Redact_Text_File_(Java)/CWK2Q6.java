import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Scanner;

/**
 *  @author Anonymous (do not change)
 *
 *  Question 6:
 *
 *  Implement in Java a similar algorithm to that in Q5, i.e. given a
 *  block of text your algorithm should be able to redact words from
 *  a given set and outputs the result to a file called â€œresult.txtâ€�.
 *  However, in this implementation of the algorithm all
 *  redactable words will be proper nouns (i.e. a name used for an
 *  individual person, place, or organisation, spelled with an initial
 *  capital letter) and your algorithm should take into account that
 *  the list of redactable words might not be complete. For example,
 *  given the block of text:
 *      It was in July, 1805, and the speaker was the well-known Anna
 *      Pavlovna Scherer, maid of honor and favorite of the Empress
 *      Marya Fedorovna. With these words she greeted Prince Vasili
 *      Kuragin, a man of high rank and importance, who was the first
 *      to arrive at her reception. Anna Pavlovna had had a cough for
 *      some days. She was, as she said, suffering from la grippe;
 *      grippe being then a new word in St. Petersburg, used only by
 *      the elite.
 *
 *  and the redactable set of words
 *      Anna Pavlovna Scherer, St. Petersburg, Marya Fedorovna
 *
 *  the output text should be
 *      It was in ****, 1805, and the speaker was the well-known ****
 *      ******** *******, maid of honor and favorite of the *******
 *      ***** *********. With these words she greeted ****** ******
 *      *******, a man of high rank and importance, who was the first
 *      to arrive at her reception. **** ******** had had a cough for
 *      some days. She was, as she said, suffering from la grippe;
 *      grippe being then a new word in *** **********, used only by
 *      the elite.
 *
 *  You should test your program using the example files provided.
*/

public class CWK2Q6
{
	/**
	 * Reads all redacted words from given redact file.
	 * Splits text into words and punctuation, replaces all words in the redact file with *s.
	 * Also replaces all capitalised words which aren't at the start of the sentence.
	 * Replaces redacted text into a file called result.txt
	 *
	 * @param textFilename File to redact.
	 * @param redactWordsFilename File containing words to redact.
	 */
	public static void redactWords(String textFilename, String redactWordsFilename)
	{
		HashSet<String> redactedWords = getFileWordsAsHashSet(redactWordsFilename);
		String textToRedact = getFileText(textFilename);
		
		// Using regex look ahead to split line into words and keeping punctuation as separate items
		String[] tokens = textToRedact.split("((?<=[\\s.,!?():\\-])|(?=[\\s.,!?():\\-]))");
		addNounsToRedactSet(redactedWords, tokens);
		
		// Checking each item if it should be redacted
		StringBuilder redactedContents = new StringBuilder();
		for (int i = 0; i < tokens.length; i++)
		{
			String token = tokens[i];
			boolean skipNext = false;
			
			// Checking if full stop is part of the match
			if (redactedWords.contains(token + '.'))
			{
				token += '.';
				skipNext =  i + 1 < tokens.length && tokens[i + 1].equals(".");
			}
			
			char[] chars = token.toCharArray();
			
			// Redacting words, taking into account plurals
			if (redactedWords.contains(token)
					|| (token.endsWith("s'") && redactedWords.contains(token.substring(token.length() - 1))
					|| (token.endsWith("'s") && redactedWords.contains(token.substring(token.length() - 2)))))
			{
				
				// Creating string of same length containing stars
				for (int j = 0; j < token.length(); j++)
				{
					redactedContents.append(Character.isWhitespace(chars[j]) ? chars[j] : '*');
				}
			}
			else
			{
				// Converting empty strings to new lines
				redactedContents.append(token.length() > 0 ? token : System.lineSeparator());
			}
			
			// Skipping if necessary
			if (skipNext)
			{
				i++;
			}
		}
		
		// Writing result to file
		try (FileWriter fileWriter = new FileWriter("./result.txt"))
		{
			fileWriter.write(redactedContents.toString());
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
	}
	
	/**
	 * Adds all nouns from a given array of tokens to the given hashset
	 *
	 * @param redactedWords The hashset to place the found nouns
	 * @param tokens Array of tokens which may or may not be nouns
	 */
	private static void addNounsToRedactSet(HashSet<String> redactedWords, String[] tokens)
	{
		// Adding Capitalised words (excluding the start to sentences) to the redact list
		HashSet<String> sentenceEndings = new HashSet<>(Arrays.asList(".", "?", "!"));
		for (int i = 0; i < tokens.length; i++)
		{
			String token = tokens[i];
			int newlineLen = System.lineSeparator().length();
			
			boolean startOfSentence = i - 2 >= 0 && sentenceEndings.contains(tokens[i - 2]);
			
			// Start of a sentence where the first two tokens are ignored (because there may be some other non space character such a piece of punctuation)
			boolean alternateStartOfSentence = i - 3 >= 0 && sentenceEndings.contains(tokens[i - 3]);
			
			boolean startOfParagraph = i - 1 - newlineLen >= 0
					&& tokens[i - 1].equals("\n")
					&& tokens[i - 1 - newlineLen].equals("\n");
			
			// Start of a paragraph where the first token is ignored (because it maybe be a piece of punctuation e.g. '"')
			boolean alternateStartOfParagraph = i - 2 - newlineLen >= 0
					&& tokens[i - 2].equals("\n")
					&& tokens[i - 2 - newlineLen].equals("\n");
			
			// > 1 to also ignore single letter words, e.g. I
			if (token.length() > 1 && Character.isUpperCase(token.charAt(0))
					&& !startOfSentence
					&& !alternateStartOfSentence
					&& !startOfParagraph
					&& !alternateStartOfParagraph)
			{
				redactedWords.add(token);
			}
		}
	}
	
	/**
	 * Reads all strings from a file separated by white space, and inserts them into a hash set.
	 *
	 * @param fileName Name of file to read.
	 * @return A hash set containing all words
	 */
	private static HashSet<String> getFileWordsAsHashSet(String fileName)
	{
		HashSet<String> lines = new HashSet<>();
		try (Scanner scanner = new Scanner(new File(fileName)))
		{
			while (scanner.hasNextLine())
			{
				String[] words = scanner.nextLine().split("\\s+");
				for (int i = 0; i < words.length; i++)
				{
					lines.add(words[i]);
				}
			}
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		
		return lines;
	}
	
	/**
	 * Gets all text from a given file.
	 *
	 * @param fileName File to read from.
	 * @return String containing all characters from the file.
	 */
	private static String getFileText(String fileName)
	{
		String text = "";
		try
		{
			text = Files.readString(Paths.get(fileName), StandardCharsets.US_ASCII);
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		
		return text;
	}
	
	public static void main(String[] args)
	{
		String inputFile = "./warandpeace.txt";
		String redactFile = "./redact.txt";
		redactWords(inputFile, redactFile);
	}
}
