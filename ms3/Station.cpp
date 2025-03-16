// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: July 15, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#include <iostream>
#include <iomanip>
#include "Utilities.h"
#include "Station.h"

namespace sdds {
	size_t Station::m_widthField{ 0 },Station::m_idGenerator{ 0 };

	//Custom 1-argument constructor uses a `Utilities` object (defined locally) to extract token
	Station::Station(const std::string& str) {
		Utilities ut{};
		size_t pos{};
		bool more{ true };
		m_id = ++m_idGenerator;

		m_name = ut.extractToken(str, pos, more);
		m_serialNumber = stoi(ut.extractToken(str, pos, more));
		m_quantity = stoi(ut.extractToken(str, pos, more));
		m_widthField = ut.getFieldWidth() > m_widthField ? ut.getFieldWidth() : m_widthField;
		m_desc = ut.extractToken(str, pos, more);
	}

	//Returns the name of the current `Station` object
	const std::string& Station::getItemName() const {
		return m_name;
	}

	//returns the next serial number to be used on the assembly line
	size_t Station::getNextSerialNumber() {

		return m_serialNumber++;
	}

	//Returns the remaining quantity of items in the `Station` object
	size_t Station::getQuantity() const {
		return m_quantity;
	}

	//Subtracts 1 from the available quantity without droping below 0.
	void Station::updateQuantity() {
		m_quantity > 0 ? --m_quantity : m_quantity;
	}

	//Displays information of the current station using ostream object.
	void Station::display(std::ostream& os, bool full) const {
		os << std::right << std::setfill('0') << std::setw(3) << m_id << " | ";
		os << std::left << std::setfill(' ')<< std::setw(m_widthField) << m_name << " | ";
		os << std::right << std::setfill('0')<< std::setw(6) << m_serialNumber << " | ";

		if (full) {
			os << std::setfill(' ')<< std::right<< std::setw(4) << m_quantity << " | ";
			os << std::left<< m_desc;
		}
		os << std::endl;
	}
}