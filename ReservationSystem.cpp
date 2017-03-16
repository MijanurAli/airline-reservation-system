#include "ReservationSystem.h"

ReservationSystem::ReservationSystem() {
	// Creating an instance of Helper to initialize the data
	Helper h = Helper();
	h_.initWindow();
	h_.initData(allBookedFlights_, airportCodes_);

	// These labels are used to build the seating-chart picker
	seatLabel_["A1"] = Seat("A1");
	seatLabel_["A2"] = Seat("A2");
	seatLabel_["B1"] = Seat("B1");
	seatLabel_["B2"] = Seat("B2");
	seatLabel_["C1"] = Seat("C1");
	seatLabel_["C2"] = Seat("C2");
	seatLabel_["D1"] = Seat("D1");
	seatLabel_["D2"] = Seat("D2");
}

ReservationSystem::~ReservationSystem() 
{
}

bool ReservationSystem::displayMenu() {
	return false;
}

// Method to create a new random flightNumber if one doesn't exist
int ReservationSystem::generateFlightNumber() {

	int flightNumber = 0;
	int floor = 10000, ceiling = 99999, range = (ceiling - floor);

	// If flightNumber doesn't exist, return a random one
	do {
		srand((unsigned)time(NULL));
		flightNumber = floor + int((range * rand()) % (ceiling + 1));
	} while (!h_.isFlightNumberUnique(flightNumber, allBookedFlights_));

	// Ternary operation to verify the generated flightNumber
	return flightNumber <= 0 ? flightNumber * -1 : flightNumber;
}

int ReservationSystem::getFlightNumber(string departureAirport, string arrivalAirport, time_t departureDateAndTime) {

	// Run flightNumber through random generator
	int flightNumber = generateFlightNumber();

	if (departureDateAndTime == 0) {
		return flightNumber;
	}

	// Iterator to search from beginning to end of the allBookedFlights map
	map<time_t, Flight>::iterator pos;
	for (pos = allBookedFlights_.begin(); pos != allBookedFlights_.end(); ++pos) {

		// The pos->first key is the time_t integer
		// The pos->second value is the object
		// If departureAirport, arrivalAirport, and departureTime match the current record, get the flightNumber and return it
		time_t key = pos->first;
		if (allBookedFlights_[key].getDepartureAirport().compare(departureAirport) == 0
			&& allBookedFlights_[key].getArrivalAirport().compare(arrivalAirport) == 0
			&& key == departureDateAndTime) {
			flightNumber = allBookedFlights_[pos->first].getFlightNumber();
			break;
		}
	}
	return flightNumber;
}

void ReservationSystem::getDateAndTime(time_t &selectedDateAndTime, vector<string> &dateAndTimeComponent) {

	bool isValid = false;

	// Exception handling to verify that the departure date and time are valid
	do {
		try {
			isValid = h_.getDepartureDateAndTime(dateAndTimeComponent, selectedDateAndTime, allBookedFlights_);
		}
		catch (char* msg) {
			h_.decoratedPrint("\n\n" + string(msg) + "\n", FOREGROUND_RED | BACKGROUND_INTENSITY);
		}
		catch (string msg) {
			h_.decoratedPrint("\n\n" + msg + "\n", FOREGROUND_RED | BACKGROUND_INTENSITY);
		}
		catch (...) {}  // This will catch the error, but continue looping
	} while (!isValid);
}

// The most vital method that creates a passenger, seat, and books the flight
bool ReservationSystem::bookFlight() {

	// Creating empty string variables to use for searching
	string i1 = "", i2 = "", i3 = "", i4 = "", i5 = "";
	string *i6 = new string("");

	system("cls");  // Clear the console screen
	h_.decoratedPrint(" Welcome to the One-Way Airlines Reservation System ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);  cout << endl;
	h_.decoratedPrint(" ================================================== ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	cout << endl << endl;

	// This will take the date and validate it
	bool isValid = false;
	Flight currentFlight;

	// For user input: get the desired departure
	time_t selectedDateAndTime = 0;
	vector<string> dateAndTimeComponent(2); // giving the date and time component two arguments
	getDateAndTime(selectedDateAndTime, dateAndTimeComponent);

	// This if statement is a reference to the Flight object to operate on
	if (allBookedFlights_.find(selectedDateAndTime) == allBookedFlights_.end()) {
		// If user's input reaches end of map, a new destination is created (LATEST_AVAILABLE)
		time_t latestAvailable = h_.getLastAvailableDepartureTime(dateAndTimeComponent[0], allBookedFlights_);
		string f = allBookedFlights_[latestAvailable].getArrivalAirport();
		string ff = h_.getDateTimeString(selectedDateAndTime);
		string fff = h_.getDateTimeString(latestAvailable);
		currentFlight = Flight(h_.getDateTimeString(selectedDateAndTime), allBookedFlights_[latestAvailable].getDepartureAirport(), 0, 500);
	}
	else {
		// Get the pre-existing destination with pre-set departure/arrival information
		currentFlight = allBookedFlights_[selectedDateAndTime];
	}
	
	h_.clearBuffer();  // This is to clear the intellisense buffer
	h_.advanceCursorToNextLine(0);

	// This will take the airportCode and validate it
	h_.decoratedPrint("     The departure airport:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	i3 = currentFlight.getDepartureAirport();

	currentFlight.setDepartureAirport(i3);
	h_.clearBuffer();
	h_.setCursor(28, h_.getCurrentCursorLine().Y, i3 + " ( " + airportCodes_[i3] + " )");
	h_.advanceCursorToNextLine(0);

	// This if statement will verify the user input for airportCode and create a new destination of their choice
	if (currentFlight.getArrivalAirport().size() != 3 ||
		allBookedFlights_.find(selectedDateAndTime) == allBookedFlights_.end()) {
		do {
			if (!i4.empty()) {
				h_.clearBuffer();
				if (i4.compare(i3) == 0) {
					h_.decoratedPrint("\n\nThe Arrival and Departure Airports should be unique.", BACKGROUND_INTENSITY | FOREGROUND_RED);
				}
				else {
					h_.decoratedPrint("\n\nYou entered an invalid Airport Code, use a valid 3-character code like: DFW, ATL, TPA, etc.", BACKGROUND_INTENSITY | FOREGROUND_RED);
				}
				cout << endl;
			}
			h_.decoratedPrint("   Pick an arrival airport:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
			i4 = h_.getAirportCode(airportCodes_);

			// Since this is a new dynamic flight, a new flight record is created, 
			// the arrival airport is set and then stored in the allBookedFlights_ map
			time_t nextDepartureTime = 0;
			h_.calculateArrivalTime(selectedDateAndTime, 2, 0, nextDepartureTime);
			allBookedFlights_[nextDepartureTime] = Flight(h_.getDateTimeString(nextDepartureTime), i4, 0, 500);
			currentFlight.setArrivalAirport(i4);
		} while (i4.compare(i3) == 0 || !h_.isAirportCodeValid(i4, airportCodes_));
	}
	// This flight has a departure/arrival airport
	else {
		h_.decoratedPrint("       The arrival airport:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
		i4 = currentFlight.getArrivalAirport();
	}

	h_.clearBuffer();
	h_.setCursor(28, h_.getCurrentCursorLine().Y, i4 + " ( " + airportCodes_[i4] + " )");
	h_.advanceCursorToNextLine(0);

	h_.decoratedPrint("Enter the Passenger's Name:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	cin.ignore();  // this is a weird C++ quirk that I researched here: http://stackoverflow.com/questions/6649852/getline-not-working-properly-what-could-be-the-reasons
	getline(cin, i5);  // Passing the passenger's name to the Passenger class
	Passenger passenger = Passenger(i5);

	// Using ternary operation to check the flight number and then set it
	int flightNumber = currentFlight.getFlightNumber() != 0 ? currentFlight.getFlightNumber() : generateFlightNumber();
	currentFlight.setFlightNumber(flightNumber);

	// Using time_t to set the arrival date and time
	time_t arrivalTime = h_.calculateArrivalTime(selectedDateAndTime, 1, 30, arrivalTime);
	string arrivalTimeString = h_.getDateTimeString(arrivalTime);
	currentFlight.setArrivalDateAndTime(h_.getDateTimeString(arrivalTime));

	// This loads the seat picker and asks the agent to select a seat number 
	map<Seat, Passenger> seats = allBookedFlights_[selectedDateAndTime].getSeatingChart();
	do {
		pickSeats(currentFlight, passenger, selectedDateAndTime, *i6);
	} while (!h_.isSeatValid(*i6, seats));  // Used a string reference to block selection of unavailable seat

	// This is to save the seating chart
	string seat = *i6;
	Seat selectedSeat = Seat(*i6);
	seats[selectedSeat] = passenger;
	currentFlight.setSeatingChart(seats);

	// Update the map to store booked flights
	allBookedFlights_[selectedDateAndTime] = currentFlight;

	// This will print a booking confirmation to the screen
	system("cls");  // Clear the console screen
	confirmationOutput(currentFlight, selectedSeat, passenger);

	// This will print a booked flight to an external file (TicketInfo.txt)
	printReservations();

	return false;
}

// Method to print a booking confirmation for the agent after all required fields are input
void ReservationSystem::confirmationOutput(Flight confirmedFlight, Seat seat, Passenger passenger) {

	stringstream price;
	price << fixed << setprecision(2) << seat.getPrice();
	h_.decoratedPrint("***************************************************************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("******   E - Ticket Confirmation for One-Way Airlines   *******", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("***************************************************************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**                FLIGHT# : " + to_string(confirmedFlight.getFlightNumber()), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**              PASSENGER : " + passenger.printerFriendlyString(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**  DEPARTURE DATE / TIME : " + confirmedFlight.getDepartureDateAndTime(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**    ARRIVAL DATE / TIME : " + confirmedFlight.getArrivalDateAndTime(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**      DEPARTURE AIRPORT : " + confirmedFlight.getDepartureAirport() + " ( " + airportCodes_[confirmedFlight.getDepartureAirport()] + " ) ", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**        ARRIVAL AIRPORT : " + confirmedFlight.getArrivalAirport() + " ( " + airportCodes_[confirmedFlight.getArrivalAirport()] + " ) ", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**                  PRICE : $" + price.str(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**              SEAT TYPE : " + seat.printerFriendlySeatType(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**                   SEAT : " + seat.printerFriendlyString(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("***************************************************************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("*************         NON - REFUNDABLE      *******************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("***************************************************************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	cout << endl;
	system("pause");
}

// Method to print booking info to a text file, in a set format (called from another method)
void ReservationSystem::printFlightInformation(Flight flight, const map<Seat, Passenger> &seatingChart, map<Seat, Passenger>::const_iterator &seat_pos, ofstream &ticketFile) {
	
	// Iterating through the seat chart, which has booking confirmations
	for ( ; seat_pos != seatingChart.end(); ++seat_pos) {
		Seat seat_key = seat_pos->first;
		ticketFile << "\n";
		ticketFile << "***************************************************************" << endl;
		ticketFile << "******  E - Ticket Confirmation for One - Way Airlines  *******" << endl;
		ticketFile << "***************************************************************" << endl;
		ticketFile << "**                FLIGHT# : " << flight.getFlightNumber() << endl;
		ticketFile << "**              PASSENGER : " << seatingChart.at(seat_key).printerFriendlyString() << endl;
		ticketFile << "**  DEPARTURE DATE / TIME : " << flight.getDepartureDateAndTime() << endl;
		ticketFile << "**    ARRIVAL DATE / TIME : " << flight.getArrivalDateAndTime() << endl;
		ticketFile << "**      DEPARTURE AIRPORT : " << flight.getDepartureAirport() << " ( " << airportCodes_[flight.getDepartureAirport()] << " ) " << endl;
		ticketFile << "**        ARRIVAL AIRPORT : " << flight.getArrivalAirport() << " ( " << airportCodes_[flight.getArrivalAirport()] << " ) " << endl;
		ticketFile << "**                  PRICE : $" << fixed << setprecision(2) << seat_key.getPrice() << endl;
		ticketFile << "**              SEAT TYPE : " << seat_key.printerFriendlySeatType() << endl;
		ticketFile << "**                   SEAT : " << seat_key.printerFriendlyString() << endl;
		ticketFile << "***************************************************************" << endl;
		ticketFile << "***********           NON - REFUNDABLE          ***************" << endl;
		ticketFile << "***************************************************************" << endl;
		ticketFile << "\n";
	}
}

// Method to open a specified text file, write ticket info to it (from printFlightInfo method) and close the file
void ReservationSystem::printReservations() {

	ofstream ticketFile("TicketInfo.txt");
	if (ticketFile.is_open()) {

		Seat seat_key;
		map<Seat, Passenger> seatingChart;
		map<time_t, Flight>::iterator pos;  // Iterator for the allBookedFlights map that uses date/time as the key
		map<Seat, Passenger>::iterator seat_pos;  // Iterator for Passenger map that uses a seat as the key
		for (pos = allBookedFlights_.begin(); pos != allBookedFlights_.end(); ++pos) {
			time_t key = pos->first;
			const map<Seat, Passenger> seatingChart = allBookedFlights_[key].getSeatingChart();
			printFlightInformation(allBookedFlights_[key], seatingChart, seatingChart.begin(), ticketFile);
		}
		ticketFile.close();
	}
	else {
		cout << "Error opening requested file." << endl;
	}
}

// Method that checks whether a seat has been booked or not
bool ReservationSystem::isSeatAvailable(char type, char row, int seatNumber) {
	return false;
}

// Method that allows agent to choose a date, time, flight, and create a passenger by passing the seatID to bookFlight()
string ReservationSystem::pickSeats(Flight currentFlight, Passenger passenger, time_t selectedDateAndTime, string &seatId) {

	int k = 1;  // Row one
	int j = 2;  // Row two
	char x = 'X';  // Used on seating chart when seat is booked
	string seat = "";
	string flightNumberString = to_string(currentFlight.getFlightNumber());
	system("cls");  // Clear the console screen

	// The decoratedPrint() method is used here to color the seating chart
	h_.decoratedPrint("ROW - A = 1st Class\tALL other ROWS = ECONOMY Class", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	cout << endl;
	h_.decoratedPrint("         Passenger :", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(passenger.printerFriendlyString(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("           Flight #:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(flightNumberString + "", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("Departure Date/Time:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(currentFlight.getDepartureDateAndTime(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("       Departs from:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(currentFlight.getDepartureAirport() + " ( " + airportCodes_[currentFlight.getDepartureAirport()] + " )", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("        Arriving to:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(currentFlight.getArrivalAirport() + " ( " + airportCodes_[currentFlight.getArrivalAirport()] + " )", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("  Arrival Date/Time:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(currentFlight.getArrivalDateAndTime(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("           Distance:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
	h_.decoratedPrint(to_string(currentFlight.getTravelMiles()) + " miles", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;

	cout << "\t\t\t       <" << endl;
	cout << "\t\t\t      <<" << endl;
	cout << "\t\t\t     <<<" << endl;
	cout << "\t\t\t    <<<<" << endl;
	cout << "\t\t\t    <<<<\t\t           <" << endl;
	cout << "\t\t\t   <<<<<\t\t         <<" << endl;
	cout << "\t\t\t   <<<<<\t\t       <<<" << endl;
	cout << "\t\t   /==== A ===== B ===== C ===== D ====<<<" << endl;
	Seat seatInput = Seat(seatId);
	map<Seat, Passenger> sChart = allBookedFlights_[selectedDateAndTime].getSeatingChart();

	// Because the "<" operator was overloaded in Seat class, the find function can be leveraged
	// This for loop is used to search through seats and determine if they've been booked or not,
	// unbooked seats will be lettered/numbered and booked seats will have an 'X'
	for (int k = 0; k < 8; k++) {
		switch (k) {
		case 0:
			cout << "\t   <<<<<<<]]     ";
			sChart.find(seatLabel_["A1"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('1');
			cout << "       ";
			break;
		case 1:
			sChart.find(seatLabel_["B1"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('1');
			cout << "       ";
			break;
		case 2:
			sChart.find(seatLabel_["C1"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('1');
			cout << "       ";
			break;
		case 3:
			sChart.find(seatLabel_["D1"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('1');
			cout << "     <<<" << endl;
			cout << "\t<<<<<<<<<<]]     ";
			break;
		case 4:
			sChart.find(seatLabel_["A2"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('2');
			cout << "       ";
			break;
		case 5:
			sChart.find(seatLabel_["B2"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('2');
			cout << "       ";
			break;
		case 6:
			sChart.find(seatLabel_["C2"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('2');
			cout << "       ";
			break;
		case 7:
			sChart.find(seatLabel_["D2"]) != sChart.end() ? h_.printSeat('X') : h_.printSeat('2');
			cout << "     <<<" << endl;
			break;
		}
	}
	cout << "\t\t   \\===================================<<<" << endl;
	cout << "\t\t\t   <<<<<\t\t       <<<" << endl;
	cout << "\t\t\t   <<<<<\t\t         <<" << endl;
	cout << "\t\t\t    <<<<   ";
	h_.decoratedPrint(" ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	h_.decoratedPrint(" = Available Seat", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "      < " << endl;
	cout << "\t\t\t    <<<<   ";
	h_.decoratedPrint(" ", BACKGROUND_RED | BACKGROUND_INTENSITY);
	h_.decoratedPrint(" = Unavailable Seat", FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << endl;
	cout << "\t\t\t     <<<" << endl;
	cout << "\t\t\t      <<" << endl;
	cout << "\t\t\t       <" << endl;

	// Error message if seat is unavailable or agent inputs wrong seat format
	if (!seatId.empty()) {
		h_.decoratedPrint("Seat Unavailable or You entered an invalid Seat-ID.\nUse Seat-IDs like A1, A2, B2, etc.\n", FOREGROUND_RED | BACKGROUND_INTENSITY);
	}
	h_.decoratedPrint("Select an Available Seat:", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << " ";
	cin >> seatId;

	return seatId;
}

// Method for the agent to see what airport destinations are available, which also resides in the project's
// Resource File folder (us-aiport-codes.txt)
void ReservationSystem::searchAvailableAirports() {

	string i4 = "";
	system("cls");  // Clear the console screen

	// Utilizing the decoratedPrint() method again for the console
	h_.decoratedPrint(" Welcome to One-Way Reservations System Airports ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);  cout << endl;
	h_.decoratedPrint(" =============================================== ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	cout << endl << endl;
	do {
		if (!i4.empty()) {
			h_.clearBuffer();
			h_.decoratedPrint("\n\nYou entered an invalid Airport Code, use a valid 3-character code like: DFW, ATL, TPA, etc.", BACKGROUND_INTENSITY | FOREGROUND_RED);
			cout << endl;
		}
		h_.decoratedPrint(" Search Available Airports:", BACKGROUND_GREEN | BACKGROUND_INTENSITY); cout << ' ';
		i4 = h_.getAirportCode(airportCodes_);
	} while (!h_.isAirportCodeValid(i4, airportCodes_));
	h_.clearBuffer();
	cout << endl << endl;
	h_.decoratedPrint("You selected " + i4 + " which is located in : " + airportCodes_[i4], BACKGROUND_GREEN | BACKGROUND_INTENSITY);  cout << endl;
	system("pause");
}

void ReservationSystem::goodBye() {

	system("cls");  // Clear the console screen

	// Get's the current console window size
	const wchar_t* brandingString = L"One - Way Airlines Reservation System";
	SetConsoleTitle(brandingString);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD consoleSize = GetLargestConsoleWindowSize(hOut);

	// X and Y are the axis locations of the console screen 
	int x = (int) (consoleSize.X / 2.60);
	int y = (int) (consoleSize.Y / 2.85);
	h_.setCursor(x, y);

	h_.decoratedPrint(" ================================================== ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint(" Exiting The One-Way Airlines Reservation System    ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint(" The ReservationSystem Pointer will be deallocated  ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint(" ================================================== ", BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.advanceCursorToNextLine(x);
}

int ReservationSystem::getMenuChoice() {

	system("cls");  // Clear the console screen
	int choice = 4;  // This will default to the exit choice

	// Create a console title and maximize the console screen
	const wchar_t* brandingString = L"One - Way Airlines Reservation System";
	SetConsoleTitle(brandingString);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD consoleSize = GetLargestConsoleWindowSize(hOut);

	// X and Y are the axis locations of the console screen 
	int x = (int)(consoleSize.X / 2.50);
	int y = (int)(consoleSize.Y / 2.80);
	h_.setCursor(x, y);

	h_.decoratedPrint("1. Show Available Destinations", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint("2. Display Flight Manifest", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint("3. Book Flight", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint("4. Exit", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	h_.advanceCursorToNextLine(x);
	h_.advanceCursorToNextLine(x);
	h_.decoratedPrint("choice: ", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cin >> choice;

	return choice;
}

// Method to print the flight manifest (shows flight info, passenger name, and seat number)
void ReservationSystem::printManifest() {

	system("cls");  // Clears the console screen

	int counter = 1;  // Counter to number the passengers in the manifest
	int header = 0;  // To make sure the "Flight Manifest" header only prints once

	Seat seat_key;
	map<Seat, Passenger> seatingChart;
	map<time_t, Flight>::iterator pos;
	map<Seat, Passenger>::iterator seat_pos;
	cout << endl;
	h_.decoratedPrint("***************************************************************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("**********\tFlight Manifest for One-Way Airlines\t*******", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	h_.decoratedPrint("***************************************************************", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
	
	// Iterate through all flights
	for (pos = allBookedFlights_.begin(); pos != allBookedFlights_.end(); ++pos) {
		time_t key = pos->first;
		seatingChart = allBookedFlights_[key].getSeatingChart();
		// Iterate through the seating chart to check for booked passengers
		for (seat_pos = seatingChart.begin(); seat_pos != seatingChart.end(); ++seat_pos) {

			if (header == 0) {
				cout << endl << endl;
				h_.decoratedPrint("--------------------------------------------------", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
				h_.decoratedPrint("Passengers for Flight #: " + to_string(allBookedFlights_[key].getFlightNumber()) + " (From " + allBookedFlights_[key].getDepartureAirport() + " to " + allBookedFlights_[key].getArrivalAirport() + ")", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
				h_.decoratedPrint("              Departing: " + allBookedFlights_[key].getDepartureDateAndTime(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
				h_.decoratedPrint("               Arriving: " + allBookedFlights_[key].getArrivalDateAndTime(), FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
				h_.decoratedPrint("--------------------------------------------------", FOREGROUND_GREEN | FOREGROUND_INTENSITY); cout << endl;
				header++;
			}

			seat_key = seat_pos->first;
			cout << endl;
			h_.decoratedPrint(to_string(counter) + ") " + seatingChart[seat_key].printerFriendlyString() + " (Seat " + seat_key.printerFriendlyString() + ")", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			counter++;
		}
		header = 0;
	}
	cout << endl << endl;
	system("pause");
}