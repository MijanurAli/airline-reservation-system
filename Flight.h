#pragma once
#include "Seat.h"
#include <time.h>
#include <string>
#include <list>
#include <map>
using namespace std;

class Flight {
public:
	Flight();
	~Flight();

	// Multiple overloaded constructors are used (one to initialize private members and the other for booking a flight)
	Flight(string departureDateAndTime, string departureAirport, int flightNumber, int travelMiles);
	Flight(string departureDateAndTime, string departureAirport, int flightNumber, int travelMiles, string arrivalAirport, string passengerName, string seatRowAndNumber);
	
	string getDepartureDateAndTime(); 
	void setDepartureDateAndTime(string departureDateAndTime);
	string getArrivalDateAndTime();
	void setArrivalDateAndTime(string arrivalDateAndTime);
	string getDepartureAirport();
	void setDepartureAirport(string location);
	string getArrivalAirport();
	void setArrivalAirport(string location);
	int getTravelMiles();
	void setTravelMiles(int mileage);
	int getFlightNumber();
	void setFlightNumber(int id);
	map<Seat, Passenger>  getSeatingChart();
	void setSeatingChart(map<Seat, Passenger> seatingChart);
	void setSeat(Seat seat, Passenger passenger);
	bool exists(Seat currentSeat);
	const static string seatId[];

private:
	string departureDateAndTime_;
	string arrivalDateAndTime_;
	string departureAirport_;
	string arrivalAirport_;
	int travelMiles_;
	int flightNumber_;
	map<Seat, Passenger> seatingChart_;
};

