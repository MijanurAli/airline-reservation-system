#include "Passenger.h"

Passenger::Passenger()
{
}

Passenger::~Passenger()
{
}

// Overloaded the constructor to assign a passenger's name (agent also has 
// the option to utilize the random id generator, if desired)
Passenger::Passenger(string fullName) {

	size_t index = fullName.find(" ");
	firstName_ = fullName.substr(0, (int)index);
	
	if (index != fullName.length() - 1) {
		lastName_ = fullName.substr(index + 1, fullName.length() - index);
	}
	
	id_ = to_string(fullName[0] + generateRandomNumber_(99999, 10000));
}

// Gets the passengers first name
string Passenger::getFirstName() {
	return firstName_;
}

// Sets the passengers first name
void Passenger::setFirstName(string firstName) {
	firstName_ = firstName;
}

// Gets the passengers last name
string Passenger::getLastName() {
	return lastName_;
}

// Sets the passengers last name
void Passenger::setLastName(string lastName) {
	lastName_ = lastName;
}

// Prints the passengers full name (last name, first name)
string Passenger::printerFriendlyString() const {
	return lastName_ + ", " + firstName_;
}

// Method to generate a random ID for the passenger (not being utilized)
int Passenger::generateRandomNumber_(int ceiling, int floor) {
	int range = (ceiling - floor);

	srand((unsigned)time(NULL));
	return floor + int((range * rand()) % (ceiling + 1));
}