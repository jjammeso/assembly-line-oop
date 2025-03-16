// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: July 15, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#ifndef SDDS_STATION_H
#define SDDS_STATION_H
#include <string>

namespace sdds {
	class Station {

		int m_id{};
		std::string m_name{}, m_desc{};
		size_t m_serialNumber{}, m_quantity{};
		static size_t m_widthField, m_idGenerator;

	public:
		Station(const std::string& str);
		const std::string& getItemName() const;
		size_t getNextSerialNumber();
		size_t getQuantity() const;
		void updateQuantity();
		void display(std::ostream& os, bool full) const;
	};
}

#endif //SDDS_STATION_H