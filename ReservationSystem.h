#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include "Helper.h"
using namespace std;

class ReservationSystem {
public:
	ReservationSystem();
	~ReservationSystem();

	bool displayMenu();
	bool bookFlight();
	void printReservations();
	void printManifest();
	string pickSeats(Flight currentFlight, Passenger passenger, time_t selctedDateAndTime, string &input);
	int getFlightNumber(string departureAirport, string arrivalAirport, time_t departureDateAndTime);
	int generateFlightNumber();
	void searchAvailableAirports();
	void getDateAndTime(time_t &selectedDateAndTime, vector<string> &dateTimeComponent);
	bool isSeatAvailable(char type, char row, int seatNumber);
	void confirmationOutput(Flight confirmedFlight, Seat seat, Passenger passenger);
	void printFlightInformation(Flight flight, const map<Seat, Passenger> &seatingChart, map<Seat, Passenger>::const_iterator &seat_pos, ofstream &ticketFile);
	int getMenuChoice();
	void goodBye();

private:
	map<string, Seat> seatLabel_;
	map<time_t, Flight> allBookedFlights_;
	map<string, string> airportCodes_;
	int flightNumber;
	Helper h_;
};

