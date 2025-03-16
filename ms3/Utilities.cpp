// Name: Sonam Jamtsho
// Seneca Student ID: 171980212
// Seneca email: sjamtsho@myseneca.ca
// Date of completion: July 15, 2023
//
// I confirm that I am the only author of this file
//   and the content was created entirely by me.

#include <iostream>
#include "Utilities.h"

namespace sdds {
	//delimiter is defined
	char Utilities::m_delimiter{};

	//Sets the field width of the current object to the value of parameter
	void Utilities::setFieldWidth(size_t newWidth) {
		m_widthField = newWidth;
	}

	//Returns the field width of the current object
	size_t Utilities::getFieldWidth()const {
		return m_widthField;
	}

	//Extracts a token from string `str`
	std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
		std::string token{0};
		size_t delimPos = str.find(m_delimiter, next_pos);

		//reports an exception if a delimiter is found at `next_pos`
		if (next_pos == delimPos) {
			more = false;
			throw "Field size was 0";
		}

		//token is extracted from the string 'str'
		if (delimPos == std::string::npos) {
			token = str.substr(next_pos);
			more = false;
		}
		else {
			token = str.substr(next_pos, delimPos - next_pos);
			more = true;
		}

		removeSpace(token);
		next_pos = delimPos != std::string::npos ? delimPos + 1 : str.length();

		//updates the current object's `m_widthField` if it is less than the size of the token extracted
		if (token.length() > m_widthField) {
			m_widthField = token.length();
		}
		return token;
	}

	//Sets the delimiter for this class to the character received
	void Utilities::setDelimiter(char delimiter) {
		m_delimiter = delimiter;
	}

	//Returns the delimiter for this class
	char Utilities::getDelimiter() {
		return m_delimiter;
	}

	//Removes leading and trailing spaces
	std::string& Utilities::removeSpace(std::string& str) {
		str.erase(0, str.find_first_not_of(" "));
		str.erase(str.find_last_not_of(" ") + 1);
		return str;
	}
}
