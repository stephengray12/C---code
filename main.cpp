#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <limits>


// struct representing a mission.
struct Mission {
    float hoursSpent = 0;
    int numRefuelings = 0;
    int missionStartStardate = 0;
    int missionEndStardate = 0;
};

// struct representing a starship
struct Starship {
    int shipID;
    std::string shipName;
    float dailyRate;
    float fuelCapacity;
    std::vector<Mission> missionLog;
};

// Helper function to get string from user.
std::string getString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

// Helper function to get integer from user.
int getInt(const std::string& prompt) {
    int input;
    std::cout << prompt;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

// Helper function to get float from user.
float getFloat(const std::string& prompt) {
    float input;
    std::cout << prompt;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

// Helper Function to get current time from user.
int getEpochTime() {
    return static_cast<int>(std::time(nullptr));
}

// Function to create new starship based on user input.
Starship makeStarship() {
    Starship newShip;
    newShip.shipID = getInt("Enter Starship ID: ");
    newShip.shipName = getString("Enter Starship Name: ");
    newShip.dailyRate = getFloat("Enter Daily Rate: ");
    newShip.fuelCapacity = getFloat("Enter Fuel Capacity: ");
    return newShip;
}

// Function to find the startship by I.D. from list of starships.
Starship* findStarship(int id, std::vector<Starship>& starships) {
    for (Starship& ship : starships) {
        if (ship.shipID == id) {
            return &ship;
        }
    }
    return nullptr; // Starship not found
}

// Function to start a mission of a starship.
// If one is in progress prompts you to end it first.
void startMission(Starship& ship) {
    if (!ship.missionLog.empty() && ship.missionLog.back().missionEndStardate == 0) {
        std::cout << "Mission already in progress. End the current mission first.\n";
        return;
    }
// Adds the newly created mission back to list.
    Mission newMission;
    newMission.missionStartStardate = getEpochTime();
    ship.missionLog.push_back(newMission);
    std::cout << "Mission started at stardate " << newMission.missionStartStardate << "\n";
}


// Function to end a mission.
// If no mission is in progress it errors to tell you there is no mission to end.
void endMission(Starship& ship) {
    if (ship.missionLog.empty() || ship.missionLog.back().missionEndStardate != 0) {
        std::cout << "No ongoing mission to end.\n";
        return;
    }
// pushes the ended mission to list with the current time.
    ship.missionLog.back().missionEndStardate = getEpochTime();
    std::cout << "Mission ended at stardate " << ship.missionLog.back().missionEndStardate << "\n";
}


// Function to refuel the starship. 
void refuel(Starship& ship) {
// Checks if there is an ongoing mission to refuel.    
    if (ship.missionLog.empty() || ship.missionLog.back().missionEndStardate != 0) {
        std::cout << "No ongoing mission to refuel.\n";
        return;
    }
    
// Incrementing the refueling count for the current mission.
    ship.missionLog.back().numRefuelings++;
    std::cout << "Starship refueled " << ship.missionLog.back().numRefuelings << " times.\n";
}


// Function to calculate cost of the mission based on time duration.
double calculateMissionCost(const Starship& ship, int missionIndex) {
    const Mission& mission = ship.missionLog[missionIndex];
    if (mission.missionEndStardate == 0) return 0.0; // Mission hasn't ended
    double duration = mission.missionEndStardate - mission.missionStartStardate;
    double cost = ship.dailyRate * duration + mission.numRefuelings * ship.fuelCapacity * 5;
    return cost;
}


// Function to Calculate the total cost of the all missions.
double calculateTotalCost(const Starship& ship) {
    double totalCost = 0.0;
    for (size_t i = 0; i < ship.missionLog.size(); i++) {
        totalCost += calculateMissionCost(ship, i);
    }
    return totalCost;
}


// Function to print mission log for a starship with mission details.
void printMissionLog(const Starship& ship) {
    if (ship.missionLog.empty()) {
        std::cout << "No missions logged for this ship.\n";
        return;
    }

// Looping through all mission results and printing there details.
    for (size_t i = 0; i < ship.missionLog.size(); ++i) {
        const Mission& mission = ship.missionLog[i];
        std::cout << "Mission " << i + 1 << ":\n";
        std::cout << "  Start Stardate: " << mission.missionStartStardate << "\n";
        std::cout << "  End Stardate: " << (mission.missionEndStardate ? std::to_string(mission.missionEndStardate) : "Ongoing") << "\n";
        std::cout << "  Refuelings: " << mission.numRefuelings << "\n";
        if (mission.missionEndStardate != 0) {
            std::cout << "  Cost: " << calculateMissionCost(ship, i) << "\n";
        }
    }
    std::cout << "Total Cost: " << calculateTotalCost(ship) << "\n";
}

// Display for Main menu.
void displayMenu() {
    std::cout << "\n1. Create a new starship\n";
    std::cout << "2. Start a mission\n";
    std::cout << "3. End a mission\n";
    std::cout << "4. Refuel a starship\n";
    std::cout << "5. View mission log\n";
    std::cout << "6. Exit\n";
}

int main() {
    std::vector<Starship> starships;
    int choice;

    do {
        displayMenu();
        choice = getInt("Choose an option: ");

// Using switch statements to navigate main menu.
        switch (choice) {
            case 1: {
                Starship newShip = makeStarship();
                starships.push_back(newShip);
                std::cout << "Starship created.\n";
                break;
            }
            case 2: {
                int shipID = getInt("Enter Starship ID: ");
                Starship* ship = findStarship(shipID, starships);
                if (ship) {
                    startMission(*ship);
                } else {
                    std::cout << "Starship not found.\n";
                }
                break;
            }
            case 3: {
                int shipID = getInt("Enter Starship ID: ");
                Starship* ship = findStarship(shipID, starships);
                if (ship) {
                    endMission(*ship);
                } else {
                    std::cout << "Starship not found.\n";
                }
                break;
            }
            case 4: {
                int shipID = getInt("Enter Starship ID: ");
                Starship* ship = findStarship(shipID, starships);
                if (ship) {
                    refuel(*ship);
                } else {
                    std::cout << "Starship not found.\n";
                }
                break;
            }
            case 5: {
                int shipID = getInt("Enter Starship ID: ");
                Starship* ship = findStarship(shipID, starships);
                if (ship) {
                    printMissionLog(*ship);
                } else {
                    std::cout << "Starship not found.\n";
                }
                break;
            }
            case 6: {
                std::cout << "Exiting program.\n";
                break;
            }
            default:
                std::cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}

