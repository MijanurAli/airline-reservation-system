#pragma once
#include <string>
#include <tuple>
#include "Passenger.h"
#define FIRST_CLASS 0
#define ECONOMY_CLASS 1
using namespace std;

class Seat {
public:
	Seat();
	~Seat();
	Seat(string input);
	int getSeatType();
	void setSeatType(int type);
	char getSeatRow() const;
	void setSeatRow(char row);
	int getSeatNumber() const;
	void setSeatNumber(int number);
	string printerFriendlyString();  
	double getPrice();
	void setPrice(double price);
	string printerFriendlySeatType();

	// Overloaded operators for this to check for booked and non-booked seats
	bool operator  <(const Seat& currentSeat) const;
	bool operator ==(const Seat& currentSeat) const;

private:
	int type_;
	char row_;
	int number_;
	double price_;
};

