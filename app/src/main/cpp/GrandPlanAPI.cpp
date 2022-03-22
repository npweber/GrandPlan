#include "GrandPlanAPI.h"
#include "model/DataUnit.h"
#include "utility/Utility.h"

#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

//GrandPlanAPI.cpp: API Containing All Functions transferred to Java Android UI via JNI

using namespace std;

//Array that holds all plannerData loaded in the current week
DataUnit plannerData[168];

//Retrieves the data from ".txt" file and loads it into the array
JNIEXPORT void JNICALL Java_tech_npwd_gp_backend_Frameworks_dataRetrieval(JNIEnv *env, jobject, jstring dateStamp) {
	
	//Clear old data before loading
	for (int i = 0; i < 168; i++) {
		plannerData[i].title = "";
		plannerData[i].description = "";
	}

	//Determine what data to load based on its index in time postion
	//(Read more to find what that means...)
	jboolean isCopy;
	determineposition(env->GetStringUTFChars(dateStamp, &isCopy));

	//Open datafile
	ifstream in;
	in.open("/data/user/0/tech.npwd.gp/cache/plannerData.txt");
	
	//Read data
	string line;
	int index = -1;
	while (getline(in, line)) {
		//Find if theres a new entry
		char first = line.front();
		char second;
		if(line.size() >= 3)
			second = line.at(2);
		if (first != ' ') {
			//Init a new DataUnit at the specified index, only if the globalIndex 
			//read from the entry is within the range of hour index we are looking at.
			int globalIndex;
			stringstream(line.substr(0, line.find(":"))) >> globalIndex;
			if (globalIndex < weekStart || globalIndex > weekEnd) 
				continue;
			//Get the index WITHIN the week, if indeed it belongs in the array
			index = globalIndex - weekStart;
			//Then initialize the DataUnit object at that index
			plannerData[index] = DataUnit();
			continue;
		}
		//Read the info of that index's event into the newly formed DataUnit
		else if (second == 't' && index != -1) {
			plannerData[index].title = line.substr(line.find(":") + 1, line.size() - 1);
			continue;
		}
		else if (second == 'd' && index != -1) {
			plannerData[index].description = line.substr(line.find(":") + 1, line.size() - 1);
			continue;
		}
		else continue;
	}
}

//Adds in an event to the storage by writing to file
JNIEXPORT void JNICALL Java_tech_npwd_gp_backend_Frameworks_encodeEvent(JNIEnv *env, jobject, jint inputIndex, jstring inputTitle, jstring inputDescription) {
	//Grab the parameters from their Java Formats
	string title, description;
	jboolean isCopy;
	title = env->GetStringUTFChars(inputTitle, &isCopy);
	description = env->GetStringUTFChars(inputDescription, &isCopy);
	int index = weekStart + (int)inputIndex;

	//Open a stream
	ofstream out;
	out.open("/data/user/0/tech.npwd.gp/cache/plannerData.txt", out.app);

	//Write the data
	out << index << ":\n" << "  title:" << title << "\n  description:" << description << endl;
	out.close();
}

//Removes a certain indexed event from storage
JNIEXPORT void JNICALL Java_tech_npwd_gp_backend_Frameworks_removeEvent(JNIEnv *env, jobject, jint inputIndex) {
	//Open an ifstream first to read all the lines in the file before changing
	ifstream in;
	in.open("/data/user/0/tech.npwd.gp/cache/plannerData.txt");

	//Control variables that mark where to stop changing
	bool shouldSkip = 0;
	int skippedLineCount = 3;

	//fileContents: Buffer variable for fileData
	string fileContents = "";
	string line;
	//Read the current file
	while (getline(in, line)) {
		//We inputted an index corresponding to an event to remove.
		//So, if the index is found, we "skip" , and, do not read the line.
		//By doing so, when the buffer overrites the file, this index will "disappear"
		if (line.find(to_string(weekStart + (int)inputIndex)) != string::npos)
			shouldSkip = 1;
		if (shouldSkip && skippedLineCount > 0) {
			//skippedLineNum: Allows the lines after the index line to be skipped
			//twice, and then the function returns to reading the file
			skippedLineCount--;
			continue;
		}
		if (skippedLineCount == 0)
			shouldSkip = 0;
		fileContents.append(line + "\n");
	}
	in.close();

	//Now, open a ofstream
	ofstream out;
	out.open("/data/user/0/tech.npwd.gp/cache/plannerData.txt");

	//And, the file is updated, without the event present before
	out << fileContents;
	out.close();
}

//From the specified index, these functions grab the title or description corresponding to an event in the array and returns it for the interface
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_titles(JNIEnv *env, jobject, jint index) {
	return (*env).NewStringUTF(plannerData[index].title.c_str());
}

JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_descriptions(JNIEnv *env, jobject, jint index) {
	return (*env).NewStringUTF(plannerData[index].description.c_str());
}

//Retrieves the current date from a C++ library
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_currentDate(JNIEnv *env) {
	//Format a "time_t"
	auto endTime = chrono::system_clock::now();
	time_t time = chrono::system_clock::to_time_t(endTime);

	//Output a datestamp
	string dateStamp = "Y:" + getYY(ctime(&time)) + ";M:" + getMM(ctime(&time)) + ";D:" + getDD(ctime(&time)) + ";H:" + getHH(ctime(&time));
    return (*env).NewStringUTF(dateStamp.c_str());
}

//Gets the Monday of the week of the currentDate
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_currentDateMonday(JNIEnv* env, jobject, jstring currentDate) {
	//Input Java String Parameter
	jboolean isCopy;
	string dateStamp = env->GetStringUTFChars(currentDate, &isCopy);

	//Get the year, month, and day from datestamp
	int year;
	int month;
	int day;
	stringstream(dateStamp.substr(9, 2)) >> month;
	stringstream(dateStamp.substr(14, 2)) >> day;
	stringstream(dateStamp.substr(2, 4)) >> year;

	//Return Monday Datestamp
	string mondayDatestamp = addToDatestamp(dateStamp, -getStartOffset(year, month, day));
	return (*env).NewStringUTF(mondayDatestamp.c_str());
}

//"Fabricates" a dateStamp based on the parameters of "inputYear" & "inputmonth" & "numWeek" (currentWeekNumber)
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_fabricateStamp(JNIEnv* env, jobject, jint inputYear, jint inputMonth, jint numWeek) {
	//Use the already loaded variables IF some are left null
	if (inputYear == -1)
		inputYear = datestampYear;
	if (inputMonth == -1)
		inputMonth = datestampMonth;
	if (numWeek == -1)
		numWeek = datestampWeek;

	//Find the starting day progressed
	//(Find the number that corresponds with a day before Jan 1 that IS a Monday)
	//(i.e. IF Jan 1 IS Wed, then the starting Monday progressed days IS -1)
	//(Stored in a array of hardcoded values for a 10-year span)
	int days = yearStarts[inputYear - 2019];

	//IF the days progressed IS below 1 (Jan 1), automatically return a timestamp of the last years day as the first Monday of the year
	if (days < 1 && inputMonth == 1 && numWeek == 1) {
		//Convert to Dec Day
		int rolledBackDay = days + monthDays[11];
		//Roll back year
		inputYear--;

		string firstWeekStamp = "Y:" + convertStringDigits(inputYear) + ";M:12;D:" + convertStringDigits(rolledBackDay) + ";H:00";
		return (*env).NewStringUTF(firstWeekStamp.c_str());
	}

	//Count up to the month that is specified
	for (int m = 1; m < inputMonth; m++)
		days += monthWeeks[m - 1] * 7;

	//Count up to the week in the month the number specifies 
	days += (numWeek - 1) * 7;

	//Since these are "days progressed", convert to an actual day in a month
	int month = -1;
	int monthDaysPassed = 0;
	//Iterate thru "monthDays" & add them if the do not exceed the progressed days
	for (int i = 0; i < 12; i++)
		if ((monthDaysPassed + monthDays[i]) < days) {
			monthDaysPassed += monthDays[i];
			//Account for a leap year IF it is one
			if (i == 1 && isLeap(inputYear))
				monthDaysPassed++;
		}
		//If they do, declare the month to be the current iteration
		else {
			month = i + 1;
			break;
		}

	//Find the days of that month by reducing "days progressed" by the "monthDaysPassed"
	int day = days - monthDaysPassed;

	//Lastly, output the values found in a formatted stamp
	string dateStamp = "Y:" + convertStringDigits(inputYear) + ";M:" + convertStringDigits(month) + ";D:" + convertStringDigits(day) + ";H:00";
	return (*env).NewStringUTF(dateStamp.c_str());
}

//Export the current datestamp's hour
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_hour(JNIEnv* env) {
	return datestampHour;
}

//Export the current datestamp's day
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_day(JNIEnv* env) {
	return datestampDay;
}

//Export the current datestamp's week
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_week(JNIEnv* env) {
	return datestampWeek;
}

//Export the current datestamp's month
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_month(JNIEnv* env) {
	return datestampMonth;
}

//Export the current datestamp's year
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_year(JNIEnv* env) {
	return datestampYear;
}

//Gets the same value that the supporting function "getStartOffset()" gets,
//except it does it based on parameters specified by the interface, so the interface can access the return value.
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_weekStartOffset(JNIEnv *env, jobject, jint day, jint month, jint year) {
	//Output a string format of the supporting function's output
	return getStartOffset(year, month, day);
}

//A Java Exported call of addToDatestamp(), returning the modified datestamp
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_addToDatestamp(JNIEnv* env, jobject, jstring inputDatestamp, jint amount) {
	//Input Java String Parameter
	jboolean isCopy;
	string dateStamp = env->GetStringUTFChars(inputDatestamp, &isCopy);
	//Return modified datestamp
	return (*env).NewStringUTF(addToDatestamp(dateStamp, amount).c_str());
}

//Translate datestamp format to regular date usage
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_translateDatestamp(JNIEnv* env, jobject, jstring inputDatestamp) {
	//Input Java String Parameter
	jboolean isCopy;
	string dateStamp = env->GetStringUTFChars(inputDatestamp, &isCopy);

	//Put the string values to int
	int month;
	int day;
	int year;
	stringstream(dateStamp.substr(9, 11)) >> month;
	stringstream(dateStamp.substr(14, 16)) >> day;
	stringstream(dateStamp.substr(4, 6)) >> year;

	//Construct the new format AND return
	string translatedFormat = to_string(month) + "/" + to_string(day) + "/" + to_string(year);
	return (*env).NewStringUTF(translatedFormat.c_str());
}

//Convert dateStamp hours to regular hour format
JNIEXPORT jstring JNICALL Java_tech_npwd_gp_backend_Frameworks_hourFormat(JNIEnv* env, jobject, jint inputHour) {
	int hour = (int)inputHour;
	string amPm = "AM";

	if (hour == 12)
		amPm = "PM";

	if (hour > 12) {
		hour -= 12;
		
		if(hour != 12)
			amPm = "PM";
	}

	//Return hour format
	string hourFormat = to_string(hour) + amPm;
	return (*env).NewStringUTF(hourFormat.c_str());
}

//Check if a month contains a fifth week
JNIEXPORT jboolean JNICALL Java_tech_npwd_gp_backend_Frameworks_isFiveWeeks(JNIEnv* env, jobject, jint inputMonth) {
	if (monthWeeks[(int)inputMonth - 1] == 5)
		return (jboolean)true;
	else
		return (jboolean)false;
}

//Get the color code for a specific day's event fill background
JNIEXPORT jint JNICALL Java_tech_npwd_gp_backend_Frameworks_colorOfDay(JNIEnv* env, jobject, jint offset) {
	switch ((int)offset) {
		case 0: return -65536;
		case 1: return -23296;
		case 2: return -256;
		case 3: return -16711936;
		case 4: return -16711681;
		case 5: return -16776961;
		case 6: return -1146130;
	}

	return -1;
}