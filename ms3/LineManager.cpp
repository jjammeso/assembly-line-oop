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
		std::ifstream fileObj(file);
		Utilities utilObj;
		utilObj.setDelimiter('|');
		size_t pos{ 0 };
		bool more{ true };

		try {
			if (!fileObj.is_open()) {
				throw "Failed to open file";
			}
			else {
				std::string str;
				while (std::getline(fileObj, str)) {
					size_t pos{ 0 };
					std::string token = utilObj.extractToken(str, pos, more);

					auto it = std::find_if(begin(stations), end(stations), [=](const Workstation* station) {
						return (token == station->getItemName());
						});
					if (it == stations.end()) {
						throw std::string("Error: Station not found for token: " + token);
					}
					if (more) {
						std::string nxtStn = utilObj.extractToken(str, pos, more);
						auto itNxtStn = std::find_if(begin(stations), end(stations), [=](const Workstation* station) {
							return (nxtStn == station->getItemName());
							});
						if (itNxtStn == stations.end()) {
							throw std::string("Error: Next station not found for token: " + nxtStn);
						}
						(*it)->setNextStation(*itNxtStn);

					}
					m_activeLine.push_back(*it);
				}

				m_cntCustomerOrder = g_pending.size();

				auto it = std::find_if(begin(m_activeLine), end(m_activeLine), [this](const Workstation* station) {
					bool unique = std::none_of(begin(m_activeLine), end(m_activeLine), [=](const Workstation* station2) {
						Workstation* next = station2->getNextStation();
						return next && station->getItemName() == next->getItemName();
						});
					return unique;
					});

				if (it != m_activeLine.end()) {
					m_firstStation = *it;
				}
				else {
					throw std::string("Error: First station could not be identified.");
				}
			}
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
