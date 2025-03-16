// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: August 05, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include <fstream>
#include <algorithm>
#include "LineManager.h"

namespace sdds {
    // Constructor for LineManager class
	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {

        Utilities ut;
        ut.setDelimiter('|');

        // Open the file for reading
        std::ifstream ifs(file);
        if (!ifs) {
            throw(std::string("File does not exist."));
        }
        try {
            // Read lines from the file and configure active stations on the assembly line
            while (ifs.good()) {
                size_t activeStn{}, nextStn{};
                bool more{ true }, EOL{ false };
                std::string line{};
                size_t pos{ 0 };

                std::getline(ifs, line);
                // Extract the first token from the line
                std::string token = ut.extractToken(line, pos, more);

                // Find the corresponding station in the 'stations' vector based on the token
                auto it = std::find_if(begin(stations), end(stations), [=](const Workstation* station){
                        return (station->getItemName() == token);
                    });

                activeStn = it - begin(stations);

                if (more) {
                    // If there is more in the line, extract the next token
                    token = ut.extractToken(line, pos, more);

                    // Find the corresponding station for the next token
                    auto it2 = std::find_if(begin(stations), end(stations), [=](const Workstation* station){
                            return (station->getItemName() == token);
                        });
                    nextStn = it2 - begin(stations);
                }
                else { 
                    // If there are no more tokens, set the end of line (EOL) flag
                    EOL = true;
                }

                // Set the next station for the active station
                stations[activeStn]->setNextStation((EOL) ? nullptr : stations[nextStn]);

                m_activeLine.push_back(stations[activeStn]);
            }

            // Find the first station in the assembly line and store its address in m_firstStation
            auto it = std::find_if(begin(stations), end(stations), [=](const Workstation* station){
                    bool unique = std::none_of(begin(stations), end(stations), [=](const Workstation* station2){
                            auto* ptr = station2->getNextStation();
                            return (station->getItemName() == ((ptr) ? ptr->getItemName() : ""));
                        });
                    return unique;
                });

            size_t first = it - begin(stations);
            m_firstStation = stations[first];

            // Store the total number of customer orders in m_cntCustomerOrder
            m_cntCustomerOrder = g_pending.size();

        }
        catch (...) {
            throw(std::string("Error: Construction of the Line Manager failed."));
        }
	}

    // Reorder the stations in the assembly line
	void LineManager::reorderStations() {

        Workstation* activeStn{};
        Workstation* nextStn{};
        std::vector<Workstation*> reorderedStations;

        activeStn = m_firstStation;
        nextStn = activeStn->getNextStation();
        reorderedStations.push_back(activeStn);

        while (nextStn) {
            reorderedStations.push_back(nextStn);
            activeStn = nextStn;
            nextStn = activeStn->getNextStation();
        }

        // Update the m_activeLine vector with the reordered stations
        m_activeLine = reorderedStations;
	}

    //Perform one iteration of operations on all workstations in the assembly line
	bool LineManager::run(std::ostream& os) {
        static size_t calls = 0;
        calls++;

        os << "Line Manager Iteration: " << calls << std::endl;

        if (g_pending.size()) {
            // If there are pending customer orders, move one order to the first station
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        // Perform the fill operation for each workstation in the assembly line
        for (Workstation* e : m_activeLine) {
            e->fill(os);
        }

        // Attempt to move orders from one station to another in the assembly line
        for (Workstation* e : m_activeLine) {
            e->attemptToMoveOrder();
        }

        // Return true if all customer orders are either completed or incomplete
        return m_cntCustomerOrder == g_completed.size() + g_incomplete.size();
	}

    // Display the status of all active stations in the assembly line
	void LineManager::display(std::ostream& os) const {
        std::for_each(begin(m_activeLine), end(m_activeLine), [&](const Workstation* station){
                station->display(os);
            });
	}
}
