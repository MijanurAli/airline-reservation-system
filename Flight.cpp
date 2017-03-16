#include "Flight.h"

const string Flight::seatId[] = { "A1", "A2", "B1", "B2", "C1", "C2", "D1", "D2" };

// In the main constructor, initialize all private member variables to empty
Flight::Flight() {

	departureDateAndTime_ = "";
	arrivalDateAndTime_ = "";
	departureAirport_ = "";
	arrivalAirport_ = "";
	travelMiles_ = 0;
	flightNumber_ = 0;
}

// In the first overloaded constructor, call main constructor and set private members to passed arguments
Flight::Flight(string departureDateAndTime, string departureAirport, int flightNumber, int travelMiles) {

	Flight();
	departureDateAndTime_ = departureDateAndTime;
	departureAirport_ = departureAirport;
	flightNumber_ = flightNumber;
	travelMiles_ = travelMiles;
}

// In the second overloaded constructor, call main constructor, first overloaded constructor,
// set the seatingChart map to Seat / Passenger classes, and set the arrivalAirport 
Flight::Flight(string departureDateAndTime, string departureAirport, int flightNumber, int travelMiles,
	string arrivalAirport, string passengerName, string seatRowAndNumber) {

	Flight();
	Flight(departureDateAndTime, departureAirport, flightNumber, travelMiles);
	seatingChart_[Seat(seatRowAndNumber)] = Passenger(passengerName);
	arrivalAirport_ = arrivalAirport;
}

Flight::~Flight()
{
}

// Gets the departure date and time for Flight 
string Flight::getDepartureDateAndTime() {
	return departureDateAndTime_;
}

// Sets the departure date and time for Flight
void Flight::setDepartureDateAndTime(string departureDateAndTime) {
	departureDateAndTime_ = departureDateAndTime;
}

// Sets the seating chart for specified map (Seat, Passenger)
void Flight::setSeatingChart(map<Seat, Passenger> seatingChart) {
	seatingChart_ = seatingChart;
}

// Method to check whether a specified seat exists (has been booked or not)
bool Flight::exists(Seat currentSeat) {

	bool retVal = false;
	Seat nextSeat;
	map<Seat, Passenger>::iterator pos;
	for (pos = seatingChart_.begin(); pos != seatingChart_.end(); ++pos) {
		nextSeat = pos->first;
		if (nextSeat.getSeatRow() == currentSeat.getSeatRow() && nextSeat.getSeatNumber() == currentSeat.getSeatRow()) {
			return true;
		}
	}

	return false;
}

// Gets the arrival date and time for Flight
string Flight::getArrivalDateAndTime() {
	return arrivalDateAndTime_;
}

// Sets the arrival date and time for Flight
void Flight::setArrivalDateAndTime(string arrivalDateAndTime) {
	arrivalDateAndTime_ = arrivalDateAndTime;
}

// Gets the departure airport for Flight
string Flight::getDepartureAirport() {
	return departureAirport_;
}

// Sets the departure airport for Flight
void Flight::setDepartureAirport(string departureAirport) {
	departureAirport_ = departureAirport;
}

// Gets the arrival airport for Flight
string Flight::getArrivalAirport() {
	return arrivalAirport_;
}

// Sets the arrival airport for Flight
void Flight::setArrivalAirport(string arrivalLocation) {
	arrivalAirport_ = arrivalLocation;
}

// Gets the travel miles from departure to arrival airport (I never set up a blocking mechanism
// for total miles traveled by the plane, to restrict the number of flights for a day)
int Flight::getTravelMiles() {
	return travelMiles_;
}

// Sets the travel miles from departure to arrival airport
void Flight::setTravelMiles(int mileage) {
	travelMiles_ = mileage;
}

// Gets the flight number for a Flight
int Flight::getFlightNumber() {
	return flightNumber_;

}

// Sets the flight number for a Flight
void Flight::setFlightNumber(int id) {
	flightNumber_ = id;
}

// Gets the seating chart
map<Seat, Passenger> Flight::getSeatingChart() {
	return seatingChart_;
}

// Sets the seat numbers for the seating chart
void Flight::setSeat(Seat seat, Passenger passenger) {
	seatingChart_[seat] = passenger;
}