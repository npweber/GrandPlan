#pragma once
#ifndef Utility_h
#define Utility_h

//Utility.h: Header of Main Backend Utilities

#include <string>
#include <sstream>
#include <time.h>

using namespace std;

//Calender Constants
const int monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const int monthWeeks[] = { 4, 4, 5, 4, 4, 5, 4, 5, 4, 4, 5, 4 };
const int yearStarts[] = { 0, -1, -3, -4, -5, 1, -1, -2, -3, -4, 1, 0 };

//Position Variables - Holds data of the currently loaded week:
//position: Current hour in week
//weekStart: Hour in year the week starts at
//weekEnd: Hour in year the week ends at
int position;
int weekStart;
int weekEnd;

//Datestamp Variables
int datestampYear;
int datestampMonth;
int datestampWeek;
int datestampDay;
int datestampHour;

//String represented integers function
string convertStringDigits(int i) {
	string stringDigit = to_string(i);
	if (stringDigit.size() < 2)
		stringDigit = "0" + stringDigit;
	return stringDigit;
}

//Grab the hour from a timestamp
string getHH(string timeStamp) {
	return timeStamp.substr(11, 2);
}

//Grab the day from a timestamp
string getDD(string timeStamp) {
	int dayDigit;
	stringstream(timeStamp.substr(8, 2)) >> dayDigit;
	return convertStringDigits(dayDigit);
}

//Get the month number from a timestamp
string getMM(string timeStamp) {
	//Retrieve month from timestamp
	string month = timeStamp.substr(4, 3);
	//Find the right number of month
	if (month == "Jan")
		return "01";
	else if (month == "Feb")
		return "02";
	else if (month == "Mar")
		return "03";
	else if (month == "Apr")
		return "04";
	else if (month == "May")
		return "05";
	else if (month == "Jun")
		return "06";
	else if (month == "Jul")
		return "07";
	else if (month == "Aug")
		return "08";
	else if (month == "Sep")
		return "09";
	else if (month == "Oct")
		return "10";
	else if (month == "Nov")
		return "11";
	else if (month == "Dec")
		return "12";
	return "-1";
}

//Gets the year from a timestamp
string getYY(string timeStamp) {
	return timeStamp.substr(20, 4);
}

//Check if the year is a leap year
bool isLeap(int year) {
	//Look for the leap years from 2019-2030
	if (year == 2020 || year == 2024 || year == 2028)
		return 1;
	return 0;
}

//Gets the days from the beginning of the week (Monday)
int getStartOffset(int year, int month, int day) {
	//Create a "time.h" object that resembles the day and month provided
	tm timeInfo = {};
	timeInfo.tm_year = year - 1900;
	timeInfo.tm_mon = month - 1;
	timeInfo.tm_mday = day;
	mktime(&timeInfo);
	//Grab the days from Sunday (tm_wday)
	//If it is 0, it's actually six; because, we are counting from Monday.
	if (timeInfo.tm_wday == 0)
		return 6;
	//Substract one to rollback the offset count to counting from Monday instead of Sunday
	return timeInfo.tm_wday - 1;
}

//Converts a day on a calander to the amount of days progressed thru a year
int convertToProgressed(int year, int month, int day) {
	//Create time object
	tm timeInfo = {};
	timeInfo.tm_year = year - 1900;
	timeInfo.tm_mon = month - 1;
	timeInfo.tm_mday = day;
	mktime(&timeInfo);
	//Return the days progressed in the year along with the days from the first Monday if the year does NOT start on Monday
	return timeInfo.tm_yday + getStartOffset(year, 1, 1);
}

//Adds a certain amount of days to the date provided thru year/month/day, returning a resultant datestamp on the day that was the amount of days ahead provided
//NOTE: Only supports adding up to 7 days
string addToDatestamp(string dateStamp, int amount) {
	//Time variables from datestamp
	int year;
	int month;
	int day;
	stringstream(dateStamp.substr(2, 4)) >> year;
	stringstream(dateStamp.substr(9, 2)) >> month;
	stringstream(dateStamp.substr(14, 2)) >> day;

	//Add amount of days to the given day
	int newDay = day + amount;

	//Handle the day being below 1 AND not appropriate for the calander
	if (newDay < 1) {
		//Rollback to the previous month (which it would be IF the day turns less than 1)
		month--;

		//Handle the month rolling back in Jan and becoming Dec AND in the last year
		if (month < 1) {
			year--;
			month = 12;
		}

		//Add the negative days to the number of days in the previous month, giving the new day of the year for the week start, Monday
		newDay += monthDays[month - 1];

		//Handle the year being a leap year to add an extra day to Feb, if needed, to correct the days rolling back from the start of the next month
		if (isLeap(year) && month == 2)
			newDay++;
	}

	//Handle the day being above the end of the month AND rolling into the next month/year
	if (newDay > monthDays[month - 1]) {
		//Subtract the days of the overextended month to define the day in the next month
		newDay -= monthDays[month - 1];

		//Handle the year being a leap year to add an extra day to Feb, if needed, to correct the days into the next month
		if (isLeap(year) && month == 2)
			newDay--;

		//Roll into the next month
		month++;
		
		//Handle the month rolling forward in Dec and becoming Jan AND in next year
		if (month > 12) {
			year++;
			month = 1;
		}
	}

	//Return resultant dateStamp
	return "Y:" + to_string(year) + ";M:" + convertStringDigits(month) + ";D:" + convertStringDigits(newDay) + ";H:00";
}

//Defines datestamp time variables, given a weekStart
void defineDatestampVars(int inputHour, int inputDay, int weekProgression, int inputYear) {
	//The datestamp hour is determined by an input hour
	datestampHour = inputHour;
	//The datestamp day is determined by an input day
	datestampDay = inputDay;
	//The datestamp year is determined by an input year
	datestampYear = inputYear;

	//Dividing the weekStart by 168 yields the amount of weeks that have gone by
	int weeksIn = (weekProgression - ((inputYear - 2019) * 8736)) / 168;

	//Cycle thru amounts of weeks in months
	for (int m = 0; m < 12; m++) {
		//Check that the weeksIn, being reduced by the amount of weeks going thru each month, falls below 0
		weeksIn -= monthWeeks[m];
		if (weeksIn < 0) {
			//When it does, by logic, that month iteration IS the month of that number of weeks in
			datestampMonth = m + 1;
			//By having the month AND the difference, the amount of weeks into that month can be found
			datestampWeek = weeksIn + monthWeeks[m] + 1;
			break;
		}
	}
}

//Determines the week the datestamp is pointing to, so the week can be setup, in app.
void determineposition(string dateStamp) {
	//Time variables from datestamp
	int year;
	int month;
	int day;
	int hour;
	stringstream(dateStamp.substr(2, 4)) >> year;
	stringstream(dateStamp.substr(9, 2)) >> month;
	stringstream(dateStamp.substr(14, 2)) >> day;
	stringstream(dateStamp.substr(19, 2)) >> hour;

	//Calaculate position in the yearspan 2019-2030 by
	//finding the days into the year + the added index progression of years since 2019 + the hour into the day, converted to hours
	position = (convertToProgressed(year, month, day) * 24) + ((year - 2019) * 8736) + hour;

	//Find the start of the week that the provided day was in by reducing the day of the year by the offset of that day to monday
	//(Effectively finding Monday's date, allowing for the day to be below 1)
	int monday = day - getStartOffset(year, month, day);

	//Grant the week to be the first, when Monday falls on a day in December that can only be part of the next year
	if (month == 12 && monday >= 27) {
		weekStart = (year - 2018) * 8736;
		weekEnd = weekStart + 168;
		datestampYear = year + 1;
		datestampMonth = 1;
		datestampWeek = 1;
		//Declare the position, this time, to be the offset from that Monday (since it's part of the first week)
		if (year == 2018) {
			position = (getStartOffset(year, month, day) * 24) + hour;
			weekStart = 0;
			weekEnd = 168;
		}
		return;
	}

	//Handle the day being below 1 AND not appropriate for the calander
	if (monday < 1) {
		//Also detect when the rollback is in Jan, meaning it's safe to call the week the first in the year
		if (month == 1) {
			weekStart = (year - 2019) * 8736;
			weekEnd = weekStart + 168;
			datestampYear = year;
			datestampMonth = 1;
			datestampWeek = 1;
			return;
		}

		//Rollback to the previous month (which it would be IF the day turns less than 1)
		month--;

		//Add the negative days to the number of days in the previous month, giving the new day of the year for the week start, Monday
		monday += monthDays[month - 1];
	}

	//Convert the week start to hours and add the index progression of the years since 2019 to differentiate the years holding that week
	weekStart = (convertToProgressed(year, month, monday) * 24) + ((year - 2019) * 8736);
	//Declare the week end by adding a week's worth of hours
	weekEnd = weekStart + 168;
	//Use the week start AND the year to define the datestamp time variables
	defineDatestampVars(hour, day, weekStart, year);
}
#endif