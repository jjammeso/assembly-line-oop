// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: August 05, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#include "Workstation.h"

namespace sdds {
	std::deque<CustomerOrder> g_pending{};//holds the orders to be placed onto the assembly line at the first station.
	std::deque<CustomerOrder> g_completed{};//holds the orders that have been removed from the last station and have been completely filled.
	std::deque<CustomerOrder> g_incomplete{};//holds the oreders that have been removed from the last station and could not be filled completely.

	//Custom 1-argument constructor
	Workstation::Workstation(const std::string& str) :Station(str){}

	//Fills the order at the front of the queue
	void Workstation::fill(std::ostream& os) {
		if (m_orders.size()) {
			m_orders.front().fillItem(*this, os);
		}
	}

	//Attemps to move the order at the fornt of the queue to the next station
	bool Workstation::attemptToMoveOrder() {
		bool result = false;
		if (m_orders.size()) {
			//Checks whether order is filled or whether it is available in the station
			if (m_orders.front().isItemFilled(getItemName()) || !getQuantity()) {
				result = true;
				if (m_pNextStation) {
					*m_pNextStation += std::move(m_orders.front());
				}
				else {
					if (m_orders.front().isOrderFilled()) {
						g_completed.push_back(std::move(m_orders.front()));
					}
					else {
						g_incomplete.push_back(std::move(m_orders.front()));
					}
				}
				m_orders.pop_front();
			}
		}
		return result;
	}
	
	//Sets the next workstation
	void Workstation::setNextStation(Workstation* station) {
		m_pNextStation = station;

	}

	//returns the address of next workstation
	Workstation* Workstation::getNextStation() const {
		return m_pNextStation;

	}

	//Displays the item name of the current workstaiton and next workstation's item name
	void Workstation::display(std::ostream& os) const {
		os << getItemName()<< " --> ";
		os << (m_pNextStation != nullptr ? m_pNextStation->getItemName() : "End of Line");
		os << std::endl;
	}
	
	//Moves the CustormerOrder passed to the back of the queue.
	Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
		m_orders.push_back(std::move(newOrder));
		return *this;
	}
}
