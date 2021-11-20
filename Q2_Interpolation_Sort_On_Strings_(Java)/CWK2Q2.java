import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

/**
 *  @author Anonymous (do not change)
 *
 *  Question 2:
 *
 *  Implement interpolation search for a list of Strings in Java
 *  using the skeleton class provided. The method should return
 *  the position in the array if the string is present, or -1 if
 *  it is not present.
*/

public class CWK2Q2
{
	public static int interpolation_search(ArrayList<String> array, String item)
	{
		// Algorithm explanation: https://www.tutorialspoint.com/data_structures_algorithms/interpolation_search_algorithm.htm
		
		ArrayList<String> searchList = new ArrayList<>(array);
		Collections.sort(searchList);
		
		int low = 0;
		int mid = -1;
		int high = searchList.size() - 1;
		
		while (low != high && !searchList.get(low).equals(searchList.get(high)))
		{
			// Interpolating strings
			mid = calculateMid(searchList, high, low, item);
			
			if (searchList.get(mid).equals(item))
			{
				// Item found
				return array.indexOf(searchList.get(mid));
			}
			else if (item.compareTo(searchList.get(mid)) > 0)
			{
				low = mid + 1;
			}
			else
			{
				high = mid - 1;
			}
		}
		
		return -1;
	}
	
	/**
	 * Finds mid value of array by interpolating the given strings.
	 * It first converts the strings by reading their underlying byte arrays as 2s compliment binary.
	 * It then uses these values with the formula given here: https://www.tutorialspoint.com/data_structures_algorithms/interpolation_search_algorithm.htm
	 *
	 * @param array Array to calculate mid of.
	 * @param highStringIndex Position of the string higher in the list.
	 * @param lowStringIndex Position of the string lower in the list.
	 * @param item Item we're searching for.
	 * @return Mid index.
	 */
	public static int calculateMid(ArrayList<String> array, int highStringIndex, int lowStringIndex, String item)
	{
		int maxLength = Math.max(Math.max(array.get(lowStringIndex).length(), array.get(highStringIndex).length()), item.length());
		BigInteger highInt = new BigInteger(array.get(highStringIndex).getBytes(StandardCharsets.US_ASCII), 0, maxLength);
		BigInteger lowInt = new BigInteger(array.get(lowStringIndex).getBytes(StandardCharsets.US_ASCII), 0, maxLength);
		BigInteger itemInt = new BigInteger(item.getBytes(StandardCharsets.US_ASCII), 0, maxLength);
		
		int diff = highInt.subtract(lowInt).intValue();
		
		// Preventing divide by 0.
		if (diff != 0)
		{
			return lowStringIndex + (highStringIndex - lowStringIndex) * (itemInt.subtract(lowInt)).intValue() / diff;
		}
		
		return 0;
	}
	
	public static void main(String[] args)
	{
		ArrayList<String> testList = new ArrayList<String>();
		testList.add("Hello");
		testList.add("World");
		testList.add("How");
		testList.add("Are");
		testList.add("You");

		int result = interpolation_search(testList, "How");
		System.out.println("Result = " + result);
	}
}
