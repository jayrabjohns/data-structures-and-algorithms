/** 
 *  @author Anonymous (do not change)
 *  
 *  Question 8: 
 *  
 *  You are given the following information, but you may prefer 
 *  to do some research for yourself.
 *   •	1 Jan 1900 was a Monday.
 *   •	Thirty days has September, April, June and November. All the rest 
 *      have thirty-one, saving February alone, which has twenty-eight, rain 
 *      or shine. And on leap years, twenty-nine.
 *   •	A leap year occurs on any year evenly divisible by 4, but not on a 
 *      century unless it is divisible by 400.
 *      
 *  How many Tuesdays fell on the first of the month during the twentieth 
 *  century (1 Jan 1901 to 31 Dec 2000)?
 *      
 *  Note, this problem is inspired by Project Euler so, as stated in the 
 *  rules of Project Euler, your solution should return an answer under 
 *  60 seconds.
*/

public class CWK2Q8
{
	public static int howManyTuesdays()
	{
		int[] monthDays = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		int tuesdays = 0;
		int weekOffset = 0;
		
		for (int currentYear = 1900; currentYear <= 2000; currentYear++)
		{
			for (int monthIndex = 0; monthIndex < monthDays.length ; monthIndex++)
			{
				int numberOfDays = monthDays[monthIndex];
				
				// Checking for leap years
				if (monthIndex == 1 && (currentYear % 400 == 0 || currentYear % 100 != 0 && currentYear % 4 == 0))
				{
					numberOfDays = 29;
				}
				
				if (weekOffset == 1 && currentYear >= 1901)
				{
					tuesdays++;
				}
				
				weekOffset = (weekOffset + numberOfDays) % 7;
			}
		}
		
		return tuesdays;
	}
	
	public static void main(String[] args)
	{
		int result = CWK2Q8.howManyTuesdays();
		System.out.println("Number of Tuesdays = " + result);
	}
}
