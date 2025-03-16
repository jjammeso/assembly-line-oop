// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: July 18, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#ifndef SDDS_CUSTOMERORDER_H
#define SDDS_CUSTOMERORDER_H
#include <iostream>
#include <string>
#include <iomanip>
#include "Utilities.h"
#include "Station.h"

namespace sdds {
	class CustomerOrder {
		struct Item{
			std::string m_itemName;
			size_t m_serialNumber{ 0 };
			bool m_isFilled{ false };

			Item(const std::string& src) : m_itemName(src) {};
		};

		std::string m_name{};
		std::string m_product{};
		size_t m_cntItem{0};
		Item** m_lstItem{};

		static size_t m_widthField;

	public:
		CustomerOrder() {};
		CustomerOrder(const std::string& str);
		CustomerOrder(const CustomerOrder& src);
		CustomerOrder& operator=(const CustomerOrder& src) = delete;
		CustomerOrder(CustomerOrder&& src) noexcept;
		CustomerOrder& operator=(CustomerOrder&& src)noexcept;
		~CustomerOrder();
		bool isOrderFilled() const;
		bool isItemFilled(const std::string& itemName) const;
		void fillItem(Station& station, std::ostream& os);
		void display(std::ostream& os) const;
	};
}

#endif //SDDS_CUSTOMERORDER_H