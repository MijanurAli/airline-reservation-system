#include "Seat.h"

Seat::Seat()
{
}

// Overloaded constructor to set up the seat attributes
Seat::Seat(string input) {

	row_ = (char)0;
	number_ = (char)0;

	if (input.compare("") != 0) {
		if (input[0] == 'A') {
			type_ = FIRST_CLASS;
			price_ = 1000.0;

		}
		else {
			type_ = ECONOMY_CLASS;
			price_ = 500.0;

		}
		row_ = (char)input[0];
		number_ = input[1] - 48;  // Converting per ASCII chart
	}
}

Seat::~Seat()
{
}

// In order to compare the row and seat numbers together, I used the <tuple> library
// and the "tie" method. Research for this was done here http://www.cplusplus.com/reference/tuple/tie/
bool Seat::operator <(const Seat& currentSeat) const {
	return tie(row_, number_) < tie(currentSeat.row_, currentSeat.number_);
}

bool Seat::operator ==(const Seat& currentSeat) const {
	return row_ == currentSeat.getSeatRow() || number_ == currentSeat.getSeatNumber();
}

// Gets the seat type chosen in ReservationSystem 
int Seat::getSeatType() {
	return type_;
}

// Sets the seat type chosen in ReservationSystem
void Seat::setSeatType(int type) {
	type_ = type;
}

// Gets the row chosen in ReservationSystem
char Seat::getSeatRow() const {
	return row_;
}

// Sets the row chosen in ReservationSystem
void Seat::setSeatRow(char row) {
	row_ = row;
}

// Gets the seat number chosen in ReservationSystem
int Seat::getSeatNumber() const {
	return number_;
}

// Sets the seat number chosen in ReservationSystem
void Seat::setSeatNumber(int number) {
	number_ = number;
}

// Printer method to print the seat-type
string Seat::printerFriendlySeatType() {
	return type_ == FIRST_CLASS ? "1st-CLASS" : "ECONOMY";
}

// Method comprised of string type: row:number
string Seat::printerFriendlyString() {
	return string(1,getSeatRow()) + to_string(getSeatNumber());
}

// Gets the price for the seat chosen in ReservationSystem
double Seat::getPrice() {
	return price_;
}

// Sets the price for the seat chosen in ReservationSystem
void Seat::setPrice(double price) {
	price_ = price;
}