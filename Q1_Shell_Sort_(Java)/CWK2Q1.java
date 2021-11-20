import java.util.ArrayList;

/**
 *  @author Anonymous (do not change)
 *
 *  Question 1:
 *
 *  Implement the Shellsort algorithm (https://en.wikipedia.org/wiki/Shellsort) 
 *  for an array of up to 1000 signed doubles in Java. Your solution must use 
 *  concrete gaps of [1, 3, 7, 15, 31, 63, 127, 255, 511]. Your solution must 
 *  print the (partially) sorted array after each gap on a new line in the form:
 *      [a0, a1, a2, a3, ..., an]
 *  Where an is the nth element in the (partially) sorted array (please note 
 *  the space after the commas), and each element should be formatted to 2 
 *  decimal places (e.g. 1.00).
 *
*/

public class CWK2Q1
{
	public static void shell_sort(ArrayList<Double> array)
	{
		// Algorithm explanation: https://www.tutorialspoint.com/data_structures_algorithms/shell_sort_algorithm.htm
		
		// Calculating initial interval
		int initialInterval = 1;
		while (initialInterval < array.size() / 2)
		{
			initialInterval = 2 * initialInterval + 1;
		}
		
		// Increasing interval by (n-1)/2 each iteration.
		for (int interval = initialInterval; interval > 0; interval = (interval - 1) / 2)
		{
			for (int outer = interval; outer < array.size(); outer++)
			{
				Double valueToInsert = array.get(outer);
				int inner;
				
				// Shift element right
				for (inner = outer; inner > interval - 1 && array.get(inner - interval) >= valueToInsert ; inner -= interval)
				{
					array.set(inner, array.get(inner - interval));
				}
				
				array.set(inner, valueToInsert);
			}
			
			printDoubleList(array);
		}
	}
	
	/**
	 * Prints all values from a given list to 2 d.p. in the form [a0, a1, a2, ..., an].
	 *
	 * @param list List to print.
	 */
	private static void printDoubleList(ArrayList<Double> list)
	{
		StringBuilder listString = new StringBuilder();
		listString.append('[');
		
		for (int i = 0; i < list.size() - 1; i++)
		{
			listString.append(String.format("%.2f, ", list.get(i)));
		}
		
		listString.append(String.format("%.2f]", list.get(list.size() - 1)));
		
		System.out.println(listString);
	}
	
	public static void main(String[] args)
	{
		ArrayList<Double> testList = new ArrayList<Double>();
		testList.add(3.4);
		testList.add(6.55);
		testList.add(-12.2);
		testList.add(1.73);
		testList.add(140.98);
		testList.add(-4.18);
		testList.add(52.87);
		testList.add(99.14);
		testList.add(73.202);
		testList.add(-23.6);
		
		shell_sort(testList);
		System.out.println(testList);
	}
}
