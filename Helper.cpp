#include "Helper.h"

Helper::Helper()
{
}

Helper::~Helper()
{
}

time_t Helper::calculateArrivalTime(time_t departureDateAndTime, int flightTimeHours, int flightTimeMinutes, time_t &timeInfo) {

	// Data-structures for time conversion
	struct tm timeinfo;
	time_t rawtime;

	// Create the "struct tm" data-structure
	time(&rawtime);
	localtime_s(&timeinfo, &departureDateAndTime);
	timeinfo.tm_hour += flightTimeHours;
	timeinfo.tm_min += flightTimeMinutes;
	timeInfo = mktime(&timeinfo);

	return timeInfo;
}

// Pre-process string date and time into integer components then
// send them to utility function to get the actual time_t integer
time_t Helper::getDateTimeInteger(string date, string militaryTime) {

	// The following is to retrieve date componets: mm-dd-yyyy and convert them to integers
	int k[3] = {};
	int index = 0;
	istringstream input(date);
	for (string field; getline(input, field, '/');) {
		k[index++] = stoi(field);
	}

	// The following is to retrieve time components (separated by a colon): HH:MM and convert them to integers
	int j[2] = {};
	index = 0;
	istringstream input2(militaryTime);
	for (string field; getline(input2, field, ':');) {
		j[index++] = stoi(field);
	}

	// This sends integer components to be converted to corresponding time_t integer
	return getDateTimeInteger(k[0], k[1], k[2], j[0], j[1]);
}

// Struct that accepts pass by reference for time_t component 
struct tm* Helper::getDateTimeComponents(string dateString) {

	time_t dateTimeInteger = Helper::getDateTimeInteger(dateString, "00:00");

	return Helper::getDateTimeComponents(dateTimeInteger);
}

struct tm* Helper::getDateTimeComponents(time_t dateTimeInteger) {

	// Data-structures for time conversion
	struct tm* timeinfo = new tm;
	time_t rawtime;

	// Create the "struct tm" data-structure
	time(&rawtime);
	localtime_s(timeinfo, &dateTimeInteger);

	return timeinfo;
}

string Helper::getDateString(time_t timeInteger) {

	// Data-structures for time conversion
	struct tm timeinfo;
	time_t rawtime;

	// Create the "struct tm" data-structure
	time(&rawtime);
	localtime_s(&timeinfo, &timeInteger);

	// Insert values of time_t
	ostringstream dateString;
	dateString << setfill('0') << setw(2) << (timeinfo.tm_mon + 1) << "/"
		<< setfill('0') << setw(2) << timeinfo.tm_mday << "/"
		<< (timeinfo.tm_year + 1900);

	return dateString.str();
}

// Getter method for date and time
string Helper::getDateTimeString(time_t timeInteger) {

	// Data-structures for time conversion
	struct tm timeinfo;
	time_t rawtime;

	// Create the "struct tm" data-structure
	time(&rawtime);
	localtime_s(&timeinfo, &timeInteger);

	// Insert values of time_t
	ostringstream dateString;
	dateString << setfill('0') << setw(2) << (timeinfo.tm_mon + 1) << "/"
		<< setfill('0') << setw(2) << timeinfo.tm_mday << "/"
		<< (timeinfo.tm_year + 1900) << " "
		<< setfill('0') << setw(2) << timeinfo.tm_hour << ":"
		<< setfill('0') << setw(2) << timeinfo.tm_min;

	return dateString.str();
}

// Given integers for mon-day-year, a time integer will be generated 
// and the returned integer will be used for key map comparisons
time_t Helper::getDateTimeInteger(int month, int day, int year, int militaryHour, int militaryMinute) {

	// Data-structures for time conversion
	struct tm timeinfo2;
	time_t rawtime;

	// Create the "struct tm" data-structure
	time(&rawtime);
	localtime_s(&timeinfo2, &rawtime);
	timeinfo2.tm_year = year - 1900;  // Done to normalize with respect to 1900
	timeinfo2.tm_mon = month - 1;  // Done to normalize to a 0-based starting number
	timeinfo2.tm_mday = day;
	timeinfo2.tm_sec = 0;
	timeinfo2.tm_hour = militaryHour;  // Setting the hour
	timeinfo2.tm_min = militaryMinute;  // Setting the minute
	timeinfo2.tm_wday = 0;
	timeinfo2.tm_yday = 0;
	timeinfo2.tm_isdst = 0;

	return mktime(&timeinfo2);
}

time_t Helper::dayAfterInteger(time_t dateInteger) {

	time_t arrivalTimeInteger;
	calculateArrivalTime(dateInteger, 24, 0, arrivalTimeInteger);
	return arrivalTimeInteger;
}

// Method to clear the intellisense buffers
void Helper::clearBuffer() {

	// I wanted to clean up the intellisense buffer which is always axis(Y) + 1
	// beneath the current input parameter; offset should be initially set to "1"
	for (int offset = 1; offset <= MAX_CODES_DISPLAYED + 1; ++offset) {

		Helper::setCursor(25, getCurrentCursorLine().Y + offset, "<blank-line/>");
	}
}

// Method to handle the cursor location for console screen
// most of my research was done here: (http://www.cplusplus.com/forum/windows/121444/, https://msdn.microsoft.com/en-us/library/windows/desktop/ms682093(v=vs.85).aspx)
COORD Helper::getCurrentCursorLine() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bb;
	GetConsoleScreenBufferInfo(hConsole, &bb);
	return bb.dwCursorPosition;
}

// " "
void Helper::setCursor(int column, int line) {

	COORD coord;
	coord.X = column;
	coord.Y = line;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bb;
	GetConsoleScreenBufferInfo(hConsole, &bb);
	SetConsoleCursorPosition(hConsole, coord);
}

// " "
void Helper::setCursor(int column, int line, string text, bool formattedPrint) {

	COORD coord;
	coord.X = column;
	coord.Y = line;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bb;
	GetConsoleScreenBufferInfo(hConsole, &bb);
	SetConsoleCursorPosition(hConsole, coord);

	if (text.compare("<blank-line/>") == 0) {
		cout << setfill(' ') << setw(80) << " ";
	}
	else {
		decoratedPrint(text, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	}
	SetConsoleCursorPosition(hConsole, bb.dwCursorPosition);
}

// This is to set the cursor and print text
void Helper::setCursor(int column, int line, string text) {

	// Here I create a COORD structure and fill in its members
	// This specifies the new position of the cursor that will be set
	COORD coord;
	coord.X = column;
	coord.Y = line;

	// Obtain a handle to the console screen buffer.
	// Since this is just a standard console, we can use STD_OUTPUT_HANDLE
	// in conjunction with the GetStdHandle() to retrieve the handle.
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bb;
	GetConsoleScreenBufferInfo(hConsole, &bb);

	// Finally, the SetConsoleCursorPosition function is called
	if (!SetConsoleCursorPosition(hConsole, coord))
	{
		// GetLastError()
	}
	else if (text.compare("<blank-line/>") == 0) {
		cout << setfill(' ') << setw(80) << " ";
	}
	else {
		cout << text;
	}

	SetConsoleCursorPosition(hConsole, bb.dwCursorPosition);
}

// Method to make airport code 3 characters and all caps
// Used a list container to store the characters
string Helper::normalizeUserAirportCode(list<char> input) {

	string code = "";
	for (list<char>::iterator it = input.begin(); it != input.end(); ++it) {
		code += *it;
	}
	string trimmedCode;
	stringstream stream(code);
	stream >> trimmedCode;
	transform(trimmedCode.begin(), trimmedCode.end(), trimmedCode.begin(), ::toupper);

	return trimmedCode;
}

// Method to use console attributes for the seating chart
void Helper::printSeat(char seat) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int saved_colors = GetConsoleTextAttribute(hConsole);

	switch (seat) {
	case 'X':
		SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_RED);
		break;
	default:
		SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	}

	cout << (char)seat;
	SetConsoleTextAttribute(hConsole, saved_colors);
}

// Method to move the console cursor to the next line
void Helper::advanceCursorToNextLine(int x) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bb;
	GetConsoleScreenBufferInfo(hConsole, &bb);
	bb.dwCursorPosition.Y += 1;
	bb.dwCursorPosition.X = x == 0 ? 0 : x;
	SetConsoleCursorPosition(hConsole, bb.dwCursorPosition);
}

// Getter method to fetch the airport code
string Helper::getAirportCode(map<string, string> &_airportCodes) {
	list<char> departureAirportCode;
	char c;
	do {
		c = _getch();
		c = (c > 96 && c < 123) ? (char)(c - 32) : c;
		cout << c;
		if (c == '\b' && departureAirportCode.size() > 0) {
			departureAirportCode.pop_back();
			Helper::setCursor(Helper::getCurrentCursorLine().X, Helper::getCurrentCursorLine().Y, " ");
		}
		else if (c != '\b' && departureAirportCode.size() >= 0){
			departureAirportCode.push_back(c);
		} 
		else if (c == '\b' && departureAirportCode.size() == 0) {
			// Input error prevention if the agent backspaces too far in the pick deptDate section
			setCursor(28, getCurrentCursorLine().Y, ""); cout << " ";
		}
		clearBuffer();
		airportCodeIntellisense(Helper::normalizeUserAirportCode(departureAirportCode), _airportCodes);
	} while (c != '\r');

	string code = "";
	for (list<char>::iterator it = departureAirportCode.begin(); it != departureAirportCode.end(); ++it)
		code += *it;

	string trimmedCode;
	stringstream stream(code);
	stream >> trimmedCode;

	return trimmedCode;
}

void Helper::airportCodeIntellisense(string normalizedAirportCode, map<string, string> &_airportCodes) {

	int offset = 1;

	// Here, I iterate over all elements and print (the second member is the value)
	map<string, string>::iterator pos;
	for (pos = _airportCodes.begin(); offset <= MAX_CODES_DISPLAYED && pos != _airportCodes.end(); ++pos) {

		if (pos->first.compare(0, normalizedAirportCode.length() >= 3 ? 3 : normalizedAirportCode.length(), normalizedAirportCode) == 0) {

			// This nested if statement ensures that excessive backspaces don't list empty airportCodes
			if (pos->second.length() > 0) {  
				Helper::setCursor(28, Helper::getCurrentCursorLine().Y + offset, pos->first + " ( " + pos->second + " )\n");
			}
			offset++;
		}
	}
}

bool Helper::isAirportCodeValid(string airportCode, map<string, string> &_airportCodes) {
	if (airportCode.length() != 3) {
		return false;  // Return false if airportCode is invalid
	}

	if (_airportCodes.count(airportCode) == 1) {
		return true;  // Return true if airportCode is valid
	}

	return false;
}

bool Helper::isFlightNumberUnique(int flightNumber, map<time_t, Flight> &_allBookedFlights) {

	bool returnValue = true;  // Assuming the given flightNumber is unique

	map<time_t, Flight>::iterator pos;
	for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {

		if (_allBookedFlights[pos->first].getFlightNumber() == flightNumber) {
			// This means the flightNumber is already in the system, so the loop will break out
			returnValue = false;
			break;
		}
	}

	return returnValue;
}

bool Helper::isSeatValid(string &seatID, map<Seat, Passenger> &seats) {

	bool isValid = false;  // Assuming seat entered is invalid

	if (seatID.length() != 2) {
		return isValid;  // Return false if invalid seat number
	}

	// Here, I determine if seat is valid or not
	// Valid seats must be one of the following: A1,A2,B1,B2,C1,C2,D1,D2
	for (int k = 0; k < 8; k++) {
		if (seatID.compare(Flight::seatId[k]) == 0) {
			isValid = true;
			break;
		}
	}

	if (!isValid) {
		return isValid;
	}

	//Here, I determine if the seat is available
	if (seats.size() > 0) {
		isValid = false;
		Seat seat = Seat(seatID);
		map<Seat, Passenger>::iterator pos;
		for (pos = seats.begin(); pos != seats.end(); ++pos) {
			if (seats.find(seat) == seats.end()) {
				isValid = true;
			}
		}
	}

	return isValid;
}

// Method to get the desired departure date and time from the agent
bool Helper::getDepartureDateAndTime(vector<string> &v, time_t &selectedDateAndTime, map<time_t, Flight> &_allBookedFlights) {
	
	list<char> departureDateAndTime;
	decoratedPrint("     Pick a departure date:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	setCursor(Helper::getCurrentCursorLine().X, getCurrentCursorLine().Y, "<blank-line/>");

	char c;
	do {
		c = _getch();
		cout << c;
		if (c == '\b' && departureDateAndTime.size() > 0) {
			// The departureDateAndTime container can't be empty
			departureDateAndTime.pop_back();
			setCursor(getCurrentCursorLine().X, getCurrentCursorLine().Y, " ");
		}
		else if (c != '\b' && departureDateAndTime.size() >= 0){
			// The departureDateAndTime container can't be empty
			departureDateAndTime.push_back(c);
		}
		else if (c == '\b' && departureDateAndTime.size() == 0) {
			// This prevents the agent from backspacing into the pick departureDate section
			setCursor(28, getCurrentCursorLine().Y, ""); cout << " ";
		}
		string dateString = "";
		for (list<char>::iterator it = departureDateAndTime.begin(); it != departureDateAndTime.end(); ++it)
			dateString += *it;

		dateIntellisense(dateString, _allBookedFlights);
	} while (c != '\r');

	clearBuffer();
	string date;

	date = normalizeDate(departureDateAndTime, _allBookedFlights);

	// Here, I want to allow partial matches to be normalized to a full date string
	bool isValid = isDateValid(date, v, _allBookedFlights);

	if (isValid) {
		selectedDateAndTime = Helper::getDateTimeInteger(v[0], v[1]);
	}
	else {
		throw "The Date Entered is invalid or UNAVAILABLE.  Valid dates look like: MM/DD/YYYY HH:MM";
	}

	return isValid;
}

// This method is used frequently to create and set colored attributes to the console
void Helper::decoratedPrint(string text, WORD highlightAttributes) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const int saved_colors = Helper::GetConsoleTextAttribute(hConsole);

	SetConsoleTextAttribute(hConsole, highlightAttributes);
	cout << text;
	SetConsoleTextAttribute(hConsole, saved_colors);
}

// This method returns the current attributes of the console
WORD Helper::GetConsoleTextAttribute(HANDLE hCon)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hCon, &con_info);
	return con_info.wAttributes;
}

// This is a custom method that I created for the agent to have the ability of "recommended input completion"
// Basically, once they start typing, they will see multiple options for a given integer
void Helper::dateIntellisense(string dateAndTimeString, map<time_t, Flight> &_allBookedFlights) {

	int offset = 1;
	string currentDateAndTimeString;
	string dateString;
	string timeString;
	istringstream ss(dateAndTimeString);
	getline(ss, dateString, ' ');
	getline(ss, timeString, ' ');
	stringstream normalizedTimeString(timeString);
	normalizedTimeString >> timeString;
	string latest = "LATEST_AVAILABLE";
	int inputLength = dateAndTimeString.length();
	int timeLength = timeString.length();
	string ts = timeString.substr(0, timeLength);
	int found = timeLength > 0 ? latest.substr(0, timeLength).compare(ts) : -1;

	clearBuffer();
	map<time_t, Flight>::iterator pos;
	for (pos = _allBookedFlights.begin(); offset <= MAX_CODES_DISPLAYED && pos != _allBookedFlights.end(); ++pos) {
		currentDateAndTimeString = _allBookedFlights[pos->first].getDepartureDateAndTime();
		if (currentDateAndTimeString.compare(0, inputLength, dateAndTimeString) == 0 &&
			pos->first >= time(0)) {
			setCursor(28, getCurrentCursorLine().Y + offset, currentDateAndTimeString + "\n");
			offset++;
		}
	}

	// This will print the LATEST_AVAILABLE choice as the last option in pick departureDate section 
	if ((found == 0 || offset > 1) && inputLength >= 10) {
		setCursor(28, getCurrentCursorLine().Y + offset, dateString + " LATEST_AVAILABLE\n");
	}
}

string Helper::normalizeDate(list<char> input, map<time_t, Flight> &_allBookedFlights) {

	string latest = "LATEST_AVAILABLE";
	string date = "";
	for (list<char>::iterator it = input.begin(); it != input.end(); ++it) {
		date += *it;
	}

	string trimmedDate;
	string trimmedTime;
	string normalizedDateAndTime;
	stringstream stream(date);
	stream >> trimmedDate;  // String stream that mimics "cin >> trimmedDate"
	stream >> trimmedTime;  // String stream that mimics "cin >> trimmedTime"
	normalizedDateAndTime = trimmedDate + " " + trimmedTime;

	map<time_t, Flight>::iterator pos;
	time_t matchedDateTime = (time_t)0, thresholdDateAndTime;
	string currentDateAndTime = "";
	int count = 0;

	// Exception handling to calculate the threshold time
	try {
		 thresholdDateAndTime = getDateTimeInteger(trimmedDate, "22:00");  //10PM flights are disallowed
	}
	catch (...) {
		return "error";
	}

	// Here, I use the "compare" method for partial entries like: 12/01/2015 LA, are supported
	if (latest.compare(0, trimmedTime.length(), trimmedTime) == 0) {
		// Latest flight time
		for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {
			currentDateAndTime = _allBookedFlights[pos->first].getDepartureDateAndTime();

			// Partial date & times like: 12/01/2015 08, are supported
			if (trimmedDate.length() >= 10 && currentDateAndTime.compare(0, trimmedDate.length(), trimmedDate) == 0) {
				count++;

				// This will only save a match if its before 10PM
				if (pos->first < thresholdDateAndTime) {
					matchedDateTime = pos->first;
				}
			}
		}

		// The count represents the number of flights for the selected date
		// When count >= 6, matchedDateTime will represent the LATEST_AVAILABLE date
		if (count >= 6) {
			if (matchedDateTime < time(0)) {
				throw "The Date Selected is in the past. Selecting an available date-time after: " + getDateTimeString(time(0)) + " is allowed.";
			}

			// LATEST_AVAILABLE date should be advanced by 2-hours if the user has selected a valid arrival
			// Here, I calculate a new dynamic departure time ONLY if there is a valid arrival booked for the LATEST_FLIGHT
			time_t departureTime = matchedDateTime;
			string departureTimeString = _allBookedFlights[matchedDateTime].getDepartureDateAndTime();
			string arrivalAirport = _allBookedFlights[matchedDateTime].getArrivalAirport();

			if (_allBookedFlights[matchedDateTime].getArrivalAirport().compare("") != 0) {
				calculateArrivalTime(matchedDateTime, 2, 0, departureTime);
				if (departureTime < thresholdDateAndTime) {
					// ONLY advance the date-time if LATEST_AVAILABLE is before 10PM
					departureTimeString = getDateTimeString(departureTime);
				}
			}

			// Print the latest flight on the current date (the newly calculated-date-time)
			COORD coordinate = getCurrentCursorLine();
			decoratedPrint("     Pick a departure date:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
			setCursor(28, coordinate.Y, departureTimeString + "            ");
			return departureTimeString;
		}
		else {
			time_t nowPlusThirtyMinutes = calculateArrivalTime(time(0), 0, 30, nowPlusThirtyMinutes);
			// If the plane is on the terminal and it's still not 10PM go ahead and allow this flight to be booked
			if (nowPlusThirtyMinutes < thresholdDateAndTime) {
				COORD coordinate = getCurrentCursorLine();
				decoratedPrint("     Pick a departure date:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
				setCursor(28, coordinate.Y, getDateTimeString(nowPlusThirtyMinutes) + "            ");
				return getDateTimeString(nowPlusThirtyMinutes); 
			}
		}

	}
	else { 
		// Earlier flight times "or" not the LATEST_AVAILABLE
		for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {
			currentDateAndTime = _allBookedFlights[pos->first].getDepartureDateAndTime();

			// Support partial date matches like: 12/01/2015 08, resolve that to: 12/01/2015 08:00
			if (currentDateAndTime.compare(0, normalizedDateAndTime.length(), normalizedDateAndTime) == 0) {
				count++;
				matchedDateTime = pos->first;
			}
		}

		if (count == 1) {
			COORD coordinate = getCurrentCursorLine();
			decoratedPrint("     Pick a departure date:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
			setCursor(28, coordinate.Y, _allBookedFlights[matchedDateTime].getDepartureDateAndTime() + "            ");
			return  _allBookedFlights[matchedDateTime].getDepartureDateAndTime();
		}
		// If no matches, return the latest time available if selecteDateAndTime is < 10PM and > than latest
		// time available and > than current-date-time
		else if (count == 0) { 
			time_t latestDateTimeInteger = getLatestDepartureTime(trimmedDate, _allBookedFlights);
			time_t twoHoursFromNow = 0;
			time_t currentDateTimeInteger = time(0);
			time_t tenOclockPM = getDateTimeInteger(trimmedDate, "22:00");

			// Calculate 2 hours from the current date-time and store it in twoHoursFromNow time_t integer
			calculateArrivalTime(currentDateTimeInteger, 2, 0, twoHoursFromNow);

			try {
				time_t selectedDateAndTime = getDateTimeInteger(trimmedDate, trimmedTime);
				time_t latestArrivalTime = 0;  // Here, I need to take the latestArrivalTime into account; departureTimes while the plane is mid-flight are not allowed
				calculateArrivalTime(latestDateTimeInteger, 2, 0, latestArrivalTime);
				COORD coordinate = getCurrentCursorLine();
				string latestDateTime = getDateTimeString(latestDateTimeInteger);
				decoratedPrint("     Pick a departure date:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';

				if (currentDateTimeInteger >= latestDateTimeInteger && selectedDateAndTime < tenOclockPM && selectedDateAndTime >= latestDateTimeInteger) {  
					// The selected date-time at this point should be at least 2 hours from now AND before 10PM
					// This allows agent to pick a dynamic departure time if the plane is idle and no other flights
					// are scheduled for that day.
					return getDateTimeString(selectedDateAndTime);
				}
				// The plane is mid-flight, so the agent can't pick this time
				else if (selectedDateAndTime < latestDateTimeInteger || selectedDateAndTime < latestArrivalTime) { 
					return "Error out, this date is not available because the plane is in mid-flight";
				}
				else {
					setCursor(28, coordinate.Y, latestDateTime + " ( defaulting to LATEST_AVAILABLE time )");
					return latestDateTime;
				}
			}
			catch (...) {
				return  "error";
			}
		}
	} // End of the large if / else block :)

	return normalizedDateAndTime;
}

// Method that checks for a valid date and uses "list" containers to store the months 
bool Helper::isDateValid(string dateAndTimeString, vector<string> &v, map<time_t, Flight> &_allBookedFlights) {

	string token;
	int tokenCount = 0, dateComponent = 0;
	bool isDateStringValid = false;
	int monthsWith30[] = { 4, 6, 9, 11 };
	int monthsWith31[] = { 1, 3, 5, 7, 8, 10, 12 };
	list<int> thirtyDays(monthsWith30, monthsWith30 + sizeof(monthsWith30) / sizeof(int));
	list<int> thirtyOneDays(monthsWith31, monthsWith31 + sizeof(monthsWith31) / sizeof(int));

	int month;
	size_t index = dateAndTimeString.find(" ");
	string dateString = dateAndTimeString.substr(0, (int)index);
	string timeString = dateAndTimeString.substr(index + 1, dateAndTimeString.length() - index);
	v.clear();
	v.push_back(dateString);

	std::size_t found = timeString.find("LATEST_AVAILABLE");

	if ((dateAndTimeString.length() == 16 && index == 10) || (found != string::npos)) {

		v.push_back(timeString);
		istringstream dateStream(dateString);

		while (std::getline(dateStream, token, '/')) {

			// Exception handling that rejects anything that's not an integer
			try { 
				dateComponent = stoi(token);
			}
			catch (...) {
				return false;
			}

			switch (tokenCount) {
			case 0:  // For month component (1-12)
				isDateStringValid = (token.length() == 2 && dateComponent <= 12 && dateComponent >= 1) ? true : false;
				month = dateComponent;
				break;
			case 1:  // For day component (1-31); depends on the month
				// 31 Days: Jan, Mar, May, Jul, Aug, Oct, Dec
				if (std::find(std::begin(thirtyDays), std::end(thirtyDays), month) != std::end(thirtyDays)) {
					isDateStringValid = (token.length() == 2 && dateComponent <= 30 && dateComponent > 0) ? true : false;
				}
				// 30 Days: Apr, Jun, Sep, Nov
				else if (std::find(std::begin(thirtyOneDays), std::end(thirtyOneDays), month) != std::end(thirtyOneDays)) {
					isDateStringValid = (token.length() == 2 && dateComponent <= 31 && dateComponent > 0) ? true : false;
				}
				// February has 28/29 days
				else if (month == 2) {  
					isDateStringValid = (token.length() == 2 && dateComponent <= 29 && dateComponent > 0) ? true : false;
				}
				else {
					return false;
				}
				break;
			case 2: // The year component should be a valid integer
				isDateStringValid = (token.length() == 4 && dateComponent <= 9999 && dateComponent >= 2015) ? true : false;
				break;
			default: 
				return false;
			}

			if (!isDateStringValid) {
				return false;
			}
			tokenCount++;
		}

		if (tokenCount != 3) {
			return false;
		}

		try {
			isDateStringValid = isTimeValid(timeString);
		}
		catch (string err) {
			throw err;
		}
		catch (char *err) {
			throw err;
		}

		// Validate that the date is between current-date and the last-date in 2016
		if (!_allBookedFlights.empty() && (getDateTimeInteger(dateString, timeString) > _allBookedFlights.rbegin()->first)) {
			throw "The Date Selected is beyond the allowed date: " + getDateTimeString(_allBookedFlights.rbegin()->first);
		}
		time_t a = getDateTimeInteger(dateString, timeString);
		time_t b = dayAfterInteger(time(0));
		if (timeString.length() == 5 && (getDateTimeInteger(dateString, timeString) < time(0))) {
			throw "The Date Selected is in the past. Select an available date-time after: " + getDateTimeString(time(0)) + " is allowed.";
		}

		time_t selectedDateAndTime = getDateTimeInteger(dateString, timeString);
		time_t thresholdDateAndTime = getDateTimeInteger(dateString, "22:00");//22:00
		time_t latestDateAndTime = getLatestDepartureTime(dateString, _allBookedFlights);
		time_t twoHoursLater = 0;
		calculateArrivalTime(latestDateAndTime, 2, 0, twoHoursLater);

		if (selectedDateAndTime >= thresholdDateAndTime) {
			// Dynamic times can't be booked after 10PM
			throw "You can't book a flight on or past 10PM on: " + dateString;
		}
		else if (_allBookedFlights.find(selectedDateAndTime) == _allBookedFlights.end()
			&& selectedDateAndTime < twoHoursLater && dateString.length() != 10 && timeString.length() != 5) {

			// Additional departure times are offered if the LATEST_AVAILABLE time is selected and has a valid departure airport
			// The calculated departureTime must be before 10PM and must be exactly 2 hours after the last available flight
			// Example: If 10AM is last available departure, 12PM would be the next LATEST_AVAILABLE departure-time
			// So, here I want to throw an exception in all other cases
			throw "Your requested departure time of: " + timeString + " is UNAVAILABLE on:" + dateString + ".\nSelect 'LATEST_AVAILABLE' option for additional flights.";
		}

		string  ff = _allBookedFlights[latestDateAndTime].getArrivalAirport();
		char x = 'A' + 32;  // Converting according to ASCII chart
		
		if (!isDateStringValid) {
			decoratedPrint("\nYou entered an invalid Departure Date.  Use this format MM/DD/YYYY HH:MM | LATEST_AVAILABLE\n", FOREGROUND_RED | BACKGROUND_INTENSITY);
		}

	}
	else {
		isDateStringValid = false;
	}

	return isDateStringValid;
}

// Method that checks for a valid time 
bool Helper::isTimeValid(string timeString) {

	string token;
	istringstream ss(timeString);
	int tokenCount = 0, timeComponent = 0;
	bool isTimeStringValid = false;

	while (std::getline(ss, token, ':')) {

		// Exception handling that rejects anything that's not an integer
		try {
			timeComponent = stoi(token);
		}
		catch (...) {
			throw "Military time components must be data typpe integers.";
		}

		switch (tokenCount) {

		case 0: // The hour component (0-23)
			isTimeStringValid = (token.length() == 2 && timeComponent <= 23 && timeComponent >= 0) ? true :
				throw "Military hour components are required to be: 0 < HH <= 23";
			break;
		case 1: // The minute component (0-59)
			isTimeStringValid = (token.length() == 2 && timeComponent <= 59 && timeComponent >= 0) ? true :
				throw "Military Minute Components are required to be: 0 < MM <= 59";
			break;
		default:
			return false;
		}

		if (!isTimeStringValid) {
			return false;
		}
		tokenCount++;
	}

	if (tokenCount != 2) {
		return false;
	}

	return isTimeStringValid;
}

// Getter method to retrieve the latest departureTime for flight on given day
time_t Helper::getLatestDepartureTime(string dateString, map<time_t, Flight> &_allBookedFlights) {

	time_t dateTimeInteger = getDateTimeInteger(dateString, "00:00");
	time_t nextDayInteger = dayAfterInteger(dateTimeInteger);
	time_t latestAvailableTime = 0;
	map<time_t, Flight>::iterator pos;
	for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {
		if (pos->first > dateTimeInteger && pos->first < nextDayInteger) {
			latestAvailableTime = pos->first;
		}
	}

	return latestAvailableTime;
}

// Method to initialize data for initial departures (6 flights a day) and airport codes (all U.S. airports)
// Opens and closes the following files: initial-departures.txt, us-airport-codes.txt, output-departures.txt, scheduled-flights.txt
void Helper::initData(map<time_t, Flight> &_allBookedFlights, map<string, string> &_airportCodes) {

	ifstream inputFile("us-airport-codes.txt");  // Downloaded from http://www.airportcodes.org/
	string lineData = "";
	string field1, field2, field3, field4, field5;

	// Confirm that the file opened properly
	if (inputFile.is_open()) {
		while (getline(inputFile, lineData)) {
			istringstream input(lineData);
			getline(input, field1, '(');
			getline(input, field2, '(');
			_airportCodes[field2.substr(0, 3)] = field1;
		}
		inputFile.close();
	}
	else {
		cout << "file is not open" << endl;
	}

	inputFile.open("initial-departures.txt");
	ofstream myfile;
	myfile.open("output-departures.txt");

	// While the file is open, read in the data
	if (inputFile.is_open()) {
		while (getline(inputFile, lineData)) {
			istringstream input(lineData);
			getline(input, field1, ',');
			getline(input, field4, ',');
			getline(input, field2, ',');
			getline(input, field3, ',');
			getline(input, field5, ',');
			time_t dateInteger = getDateTimeInteger(field1, field4);
			_allBookedFlights[dateInteger] = Flight(field1 + " " + field4, field2, stoi(field3), stoi(field5));
			myfile << to_string(dateInteger) << "," << field1 << "," << field4 << "," << field2 << "," << field3 << "," << field5 << endl;
		}
		inputFile.close();
		myfile.close();

		// The following flights are hard-coded for "today's date" only
		// Initial departures has 6 scheduled flights a day starting from midnight through 10AM
		string todaysDate = getDateString(time(0));
		time_t dateInteger = getDateTimeInteger(todaysDate, "12:00");
		_allBookedFlights[dateInteger].setDepartureAirport("DAL");
		string departureDateAndTimeString = getDateTimeString(dateInteger);
		_allBookedFlights[dateInteger].setDepartureDateAndTime(departureDateAndTimeString);
		_allBookedFlights[dateInteger].setTravelMiles(500);
		_allBookedFlights[dateInteger].setFlightNumber(44445);

		todaysDate = getDateString(time(0));
		dateInteger = getDateTimeInteger(todaysDate, "14:00");
		_allBookedFlights[dateInteger].setDepartureAirport("SAT");
		departureDateAndTimeString = getDateTimeString(dateInteger);
		_allBookedFlights[dateInteger].setDepartureDateAndTime(departureDateAndTimeString);
		_allBookedFlights[dateInteger].setTravelMiles(500);
		_allBookedFlights[dateInteger].setFlightNumber(44446);

		todaysDate = getDateString(time(0));
		dateInteger = getDateTimeInteger(todaysDate, "16:00");
		_allBookedFlights[dateInteger].setDepartureAirport("DAL");
		departureDateAndTimeString = getDateTimeString(dateInteger);
		_allBookedFlights[dateInteger].setDepartureDateAndTime(departureDateAndTimeString);
		_allBookedFlights[dateInteger].setTravelMiles(500);
		_allBookedFlights[dateInteger].setFlightNumber(44447);

		todaysDate = getDateString(time(0));
		dateInteger = getDateTimeInteger(todaysDate, "18:00");
		_allBookedFlights[dateInteger].setDepartureAirport("AUS");
		departureDateAndTimeString = getDateTimeString(dateInteger);
		_allBookedFlights[dateInteger].setDepartureDateAndTime(departureDateAndTimeString);
		_allBookedFlights[dateInteger].setTravelMiles(500);
		_allBookedFlights[dateInteger].setFlightNumber(44448);

		todaysDate = getDateString(time(0));
		dateInteger = getDateTimeInteger(todaysDate, "20:00");
		_allBookedFlights[dateInteger].setDepartureAirport("DAL");
		departureDateAndTimeString = getDateTimeString(dateInteger);
		_allBookedFlights[dateInteger].setDepartureDateAndTime(departureDateAndTimeString);
		_allBookedFlights[dateInteger].setTravelMiles(500);
		_allBookedFlights[dateInteger].setFlightNumber(44449);
	}
	else {
		cout << "file is not open" << endl;
	}

	// Here, I normalize arrivals by setting the arrivalAirport and arrivalTime
	time_t nextDepartureTime, arrivalTime;
	string temp = "";
	map<time_t, Flight>::iterator pos;
	Flight f;
	int x = 0;
	string todaysDate = getDateString(time(0));
	string currentDate = "";
	for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {

		f = _allBookedFlights[pos->first];
		struct tm* tC = getDateTimeComponents(pos->first);

		if (x <= 4 || (x <= 9 && currentDate.compare(todaysDate) == 0)) {

			// Only the first 5 flights on a given date will have a set of arrival stats
			// The 6th departure is dynamic with respect to the user's input choice of arrival airport

			// Determine the time integer for the next departure (assume its 2 hours from last departure)
			calculateArrivalTime(pos->first, 2, 0, nextDepartureTime);
			f = _allBookedFlights[nextDepartureTime];
			struct tm* ttC = getDateTimeComponents(nextDepartureTime);

			// Calculate the arrival time to be 30 minutes before the next departure time of next flight
			calculateArrivalTime(nextDepartureTime, 0, -30, arrivalTime);
			temp = getDateTimeString(arrivalTime);
			currentDate = getDateString(pos->first);

			// These calculations are saved for the arrivalTime and arrivalAirport for the current departure
			_allBookedFlights[pos->first].setArrivalDateAndTime(temp);
			_allBookedFlights[pos->first].setArrivalAirport(_allBookedFlights[nextDepartureTime].getDepartureAirport());
		}
		// Here, I assume all arrival times are 1.5 hours after the previous departure
		else { 
			calculateArrivalTime(pos->first, 1, 30, arrivalTime);
			temp = getDateTimeString(arrivalTime);
			_allBookedFlights[pos->first].setArrivalDateAndTime(temp);
		}
		// Reset the counter when either of the following are true:
		// 1. When processing any day EXCEPT today and NOT the last departure time of 10AM
		// 2. When processing the CURRENT day and NOT the last departure time of 10PM
		// Any of the above means that all flights have been processed except the 
		// last one to have an arrivalAirport and an arrivalTime
		x = ((x > 4) && (currentDate.compare(todaysDate) != 0) || (x > 9 && currentDate.compare(todaysDate) == 0)) ? 0 : x + 1;
	}

	// Print the scheduling map after all initializing and normalizing is complete
	myfile.open("scheduled-flights.txt");

	for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {
		f = _allBookedFlights[pos->first];
		myfile << f.getDepartureAirport() << "( " << _airportCodes[f.getDepartureAirport()] << ")" << "->"
			<< f.getArrivalAirport() << "(" << _airportCodes[f.getArrivalAirport()] << ")" << ","
			<< f.getDepartureDateAndTime() << "->" << f.getArrivalDateAndTime()
			<< "," << f.getFlightNumber() << endl;
	}
	myfile.close();
}

// Method to maximize the console window upon launch (makes the seating chart and intellisense features better)
void Helper::initWindow() {

	HWND console;
	const wchar_t* brandingString = L"One - Way Airlines Reservation System";
	SetConsoleTitle(brandingString);
	console = FindWindow(NULL, brandingString);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };

	SetConsoleScreenBufferSize(hOut, NewSBSize);
	DisplayArea.Right = NewSBSize.X - 1;
	DisplayArea.Bottom = NewSBSize.Y - 1;
	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
	ShowWindow(console, SW_MAXIMIZE);
}

// This method returns a new LATEST_AVAILABLE time
time_t Helper::getLastAvailableDepartureTime(string dateString, map<time_t, Flight> &_allBookedFlights) {

	time_t currentDateInteger = getDateTimeInteger(dateString, "00:00");
	time_t nextDayInteger = dayAfterInteger(currentDateInteger);
	time_t lastDateTimeInteger;

	map<time_t, Flight>::iterator pos;
	for (pos = _allBookedFlights.begin(); pos != _allBookedFlights.end(); ++pos) {

		if (pos->first >= currentDateInteger && pos->first < nextDayInteger) {
			lastDateTimeInteger = pos->first;
		}
	}

	return lastDateTimeInteger;
}