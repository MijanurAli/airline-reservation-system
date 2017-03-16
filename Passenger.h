#pragma once
#include <string>
#include <time.h>
using namespace std;

class Passenger {
public:
	Passenger();
	~Passenger();
	Passenger(string fullName);
	string getFirstName();
	void setFirstName(string firstName);
	string getLastName();
	void setLastName(string firstName);
	string printerFriendlyString() const;

private:
	string firstName_;
	string lastName_;
	string id_;
	int generateRandomNumber_(int ceiling, int floor);
};


