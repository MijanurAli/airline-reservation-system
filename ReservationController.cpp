// Capstone Project - COSC 1437
// Mijanur Ali 
#include "ReservationSystem.h"

bool menu(ReservationSystem *reservationSystem) {

	int choice = reservationSystem->getMenuChoice();
	switch (choice) {
	case 1:
		reservationSystem->searchAvailableAirports();
		break;
	case 2:
		reservationSystem->printManifest();
		break;
	case 3:
		reservationSystem->bookFlight();
		break;
	default: // Cleanup of the dynamic pointer for ReservationSystem
		reservationSystem->goodBye();
		delete reservationSystem;
		reservationSystem = NULL;
		return false;
	}
	return true;
}

void interactiveDisplay() {
	SetConsoleTitle(L"CAPSTONE PROJECT");
	system("color 80");
	cout << "\n\n\n\n\n\n\n\n\n\n\t\t     ONE-WAY AIRLINES: FLIGHTS FOR DAYS :)";
	Sleep(3000);
	system("cls");
	cout << "\n\n\n\n\n\n\n\t\t          DEVELOPED BY: ";
	Sleep(500);
	cout << "MIJANUR ALI";
	Sleep(3000);
	system("cls");
}

int main() {
	ReservationSystem *reservationSystem = new ReservationSystem();
	interactiveDisplay();
	bool go = true;
	// Control loop that keeps going until go = false
	do { 
		go = menu(reservationSystem);
	} while (go);
}