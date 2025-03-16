// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: July 18, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.
#include "CustomerOrder.h"

namespace sdds {
	// Definition of the static member variable m_widthField (initialized to 0)
	size_t CustomerOrder::m_widthField{};

	// Constructor: Takes a string containing customer order details.
	CustomerOrder::CustomerOrder(const std::string& str) {
		Utilities ut;
		size_t pos{};
		bool more = true;

		// Extracts customer name and order name from the input string
		m_name = ut.extractToken(str, pos, more);
		m_product = ut.extractToken(str, pos, more);

		size_t rewind = pos;

		// Count the number of items in the order
		while (more) {
			ut.extractToken(str, pos, more);
			m_cntItem++;
		}

		// If there are items in the order, allocate memory for them and assign them to the array of pointers.
		if (m_cntItem > 0) {
			more = true;
			m_lstItem = new Item * [m_cntItem] {};

			for (size_t i{ 0 }; i < m_cntItem; i++) {
				m_lstItem[i] = new Item(ut.extractToken(str, rewind, more));
				m_widthField = ut.getFieldWidth() > m_widthField ? ut.getFieldWidth() : m_widthField;
			}
		}
	}

	// Copy constructor: It is disallowed and throws an error if called..
	CustomerOrder::CustomerOrder(const CustomerOrder& src) {
		throw "Error!";
	}

	// Move constructor: Moves the content from the source object to the current object.
	CustomerOrder::CustomerOrder(CustomerOrder&& src) noexcept {
		*this = std::move(src);
	}

	// Move assignment operator: Moves the content from the source object to the current object.
	CustomerOrder& CustomerOrder::operator=(CustomerOrder&& src)noexcept {
		if (this != &src) {
			// Deletes the existing items in the current object
			if (m_cntItem > 0) {
				for (size_t i = 0; i < m_cntItem; i++) {
					delete m_lstItem[i];
				}
			}
			delete[] m_lstItem;
			// Move data from the source object to the current object
			m_cntItem = src.m_cntItem;
			m_lstItem = src.m_lstItem;
			m_name = src.m_name;
			m_product = src.m_product;

			// Reset the source object's data to a safe state
			src.m_cntItem = 0;
			src.m_lstItem = nullptr;
		}
		return *this;
	}

	// Destructor: Deallocates memory for the dynamic array of Item pointers and the Items themselves.
	CustomerOrder::~CustomerOrder() {
		for (size_t i = 0; i < m_cntItem; i++) {
			delete m_lstItem[i];
		}
		delete[] m_lstItem;
		m_lstItem = nullptr;
	}

	// Checks if all the items in the order are filled
	bool CustomerOrder::isOrderFilled() const {
		bool filled = true;
		for (size_t i = 0; i < m_cntItem; i++) {
			if (!m_lstItem[i]->m_isFilled) {
				filled = false;
			}
		}
		return filled;
	}

	// Checks if a specific item in the order is filled
	bool CustomerOrder::isItemFilled(const std::string& itemName) const {
		bool filled = true;
		for (size_t i = 0; i < m_cntItem; i++) {
			if (m_lstItem[i]->m_itemName == itemName && !m_lstItem[i]->m_isFilled) {
				filled = false;
			}
		}
		return filled;
	}

	// Fills an item in the order from a given station, if the station has enough quantity.
	void CustomerOrder::fillItem(Station& station, std::ostream& os) {
		bool found = false;

		for (size_t i = 0; i < m_cntItem && !found; i++) {
			if (station.getItemName() == m_lstItem[i]->m_itemName && m_lstItem[i]->m_isFilled==false) {
				if (station.getQuantity()>0) {
					m_lstItem[i]->m_isFilled = found = true;

					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();

					station.updateQuantity();

					os << "    Filled ";
					os << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
				}
				else {
					// If the station doesn't have enough quantity, print an error message
					os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
				}
			}
		}
	}

	// Displays the customer name, product name, and all items in the order with their details.
	void CustomerOrder::display(std::ostream& os) const {
		os << m_name << " - " << m_product << std::endl;
		for (size_t i = 0; i < m_cntItem; i++) {
			os << std::setfill('0')<<"[" << std::setw(6) << std::right << m_lstItem[i]->m_serialNumber << "] ";
			os << std::setfill(' ') << std::left<< std::setw(m_widthField) << m_lstItem[i]->m_itemName;
			os << " - ";
			os << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED")<<std::endl;
		}
	}
}