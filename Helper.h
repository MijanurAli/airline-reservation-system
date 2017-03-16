#pragma once
#define MAX_CODES_DISPLAYED 10  // Max amount of lines to be printed in intellisense methods
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <windows.h>
#include <list>
#include <map>
#include <algorithm>
#include <conio.h>
#include <sstream>
#include <math.h>
#include "Flight.h"
using namespace std;

class Helper
{
public:
	Helper();
	~Helper();

	// Multipe time_t struct functions to perform searches and retrieve variables
	time_t calculateArrivalTime(time_t departureDateAndTime, int flightTimeHours, int flightTimeMinutes, time_t &timeInfo);
	time_t dayAfterInteger(time_t dateInteger);
	time_t getDateTimeInteger(int month, int day, int year, int militaryHour, int militaryMinute);
	time_t getDateTimeInteger(string date, string militaryTime);
	string getDateTimeString(time_t timeInteger);
	string getDateString(time_t timeInteger);
	struct tm* getDateTimeComponents(time_t timeInteger);
	struct tm* getDateTimeComponents(string dateString);

	// Input helper for date-and-time, airport-codes, etc.
	bool getDepartureDateAndTime(vector<string> &v, time_t &selectedDateAndTime, map<time_t, Flight> &_allBookedFlights);  	

	// Input helper for: stdout, console, cursor manipulation, etc.
	void setCursor(int column, int line, string text);
	void setCursor(int column, int line, string text, bool formattedPrint);
	void setCursor(int column, int line);

	// Methods for creating and setting colored attributes to the console screen
	void decoratedPrint(string text, WORD highlightAttributes);
	WORD GetConsoleTextAttribute(HANDLE hCon);
	void advanceCursorToNextLine(int x);
	void printSeat(char seat);

	// Input helpers for: intellisense functionality
	void clearBuffer();
	COORD getCurrentCursorLine();

	// These functions were optimized for performance by passing the map by reference
	void airportCodeIntellisense(string normalizedAirportCode, map<string, string> &_airportCodes);  
	void dateIntellisense(string normalizedAirportCode, map<time_t, Flight> &_allBookedFlights);	 

	// These helper functions are used for validation
	string normalizeUserAirportCode(list<char> input);
	string getAirportCode(map<string, string> &_airportCodes);
	string normalizeDate(list<char> input, map<time_t, Flight> &_allBookedFlights);                  
	bool isDateValid(string dateString, vector<string> &v, map<time_t, Flight> &_allBookedFlights);  
	bool isTimeValid(string timeString);
	bool isSeatValid(string &seatId, map<Seat, Passenger> &seats);
	bool isFlightNumberUnique(int flightNumber, map<time_t, Flight> &_allBookedFlights);
	bool isAirportCodeValid(string, map<string, string> &_aiportCodes);

	// These functions are used to load data and initialization helpers
	void initData(map<time_t, Flight> &_allBookedFlights, map<string, string> &_airportCodes);
	void initWindow();  

	// These functions were used to help with the overall logic
	time_t getLatestDepartureTime(string dateString, map<time_t, Flight> &_allBookedFlights);
	time_t getLastAvailableDepartureTime(string dateString, map<time_t, Flight> &_allBookedFlights);
};