/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:20:26 by alafon            #+#    #+#             */
/*   Updated: 2025/10/17 11:35:24 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/BitcoinExchange.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(const std::string &databasePath) {loadDatabase(databasePath);}
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) : _rates(other._rates) {}
BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other)
		_rates = other._rates;
	return *this;
}
BitcoinExchange::~BitcoinExchange() {}

// Prints a formatted error message describing why a line from the input failed.
static void printErrorDetailed(std::size_t lineNumber, const std::string &input, const std::string &message) {
    std::cout << "Error: " << message << " (line " << lineNumber << ": " << input << ")" << std::endl;
    std::cout << std::endl;
}

// Displays the computation for a successful lookup along with the matching rate.
static void printSuccessDetailed(const std::string &date, double value, const std::string &matchedDate, double rate) {
    std::ios::fmtflags oldFlags = std::cout.flags();
    std::streamsize oldPrecision = std::cout.precision();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Closest exchange data: " << matchedDate << " -> " << rate << std::endl;
    std::cout << "[" << date << "] " << value << " * " << rate << " = " << rate * value << std::endl;
    std::cout.flags(oldFlags);
    std::cout.precision(oldPrecision);
    std::cout << std::endl;
}

// Reads the CSV database and stores validated exchange rates by date.
void BitcoinExchange::loadDatabase(const std::string &databasePath) {
    std::ifstream db(databasePath.c_str());
    if (!db.is_open())
        throw std::runtime_error("Error: could not open database file.");

    _rates.clear();
    std::string line;
    while (std::getline(db, line)) {
        if (line.empty())
            continue;
        std::string::size_type comma = line.find(',');
        if (comma == std::string::npos)
            throw std::runtime_error("Error: bad database entry => " + line);
        std::string date = trim(line.substr(0, comma));
        std::string valueToken = trim(line.substr(comma + 1));
        if (date == "date")
            continue;
        if (!isValidDate(date))
            throw std::runtime_error("Error: invalid date in database => " + date);
        double rate;
        if (!parseNumber(valueToken, rate))
            throw std::runtime_error("Error: invalid rate in database => " + valueToken);
        if (rate < 0)
            throw std::runtime_error("Error: negative rate in database => " + valueToken);
        _rates[date] = rate;
    }
    if (_rates.empty())
        throw std::runtime_error("Error: database is empty.");
}

// Processes each line of the input file and attempts to compute the requested exchanges.
void BitcoinExchange::processInput(const std::string &inputPath) const {
    std::ifstream input(inputPath.c_str());
    if (!input.is_open())
        throw std::runtime_error("Error: could not open file.");

    std::string line;
    bool firstLine = true;
    std::size_t lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (line.empty())
            continue;
        std::string trimmed = trim(line);
        if (firstLine) {
            std::string collapsed;
            collapsed.reserve(trimmed.size());
            for (std::string::size_type i = 0; i < trimmed.size(); ++i) {
                unsigned char c = static_cast<unsigned char>(trimmed[i]);
                if (!std::isspace(c))
                    collapsed += static_cast<char>(std::tolower(c));
            }
            if (collapsed == "date|value" || collapsed == "date,value" || collapsed == "date,exchange_rate") {
                firstLine = false;
                continue;
            }
        }
        firstLine = false;

        std::cout << "Input: " << trimmed << std::endl;

        std::string::size_type delimiterPos = trimmed.find('|');
        if (delimiterPos == std::string::npos)
            delimiterPos = trimmed.find(',');
        if (delimiterPos == std::string::npos) {
            printErrorDetailed(lineNumber, trimmed, "missing separator.");
            continue;
        }
        std::string date = trim(trimmed.substr(0, delimiterPos));
        std::string valueToken = trim(trimmed.substr(delimiterPos + 1));
        if (date.empty() || valueToken.empty()) {
            printErrorDetailed(lineNumber, trimmed, "missing date or value.");
            continue;
        }
        if (!isValidDate(date)) {
            printErrorDetailed(lineNumber, trimmed, "invalid date.");
            continue;
        }
        double value;
        if (!parseNumber(valueToken, value)) {
            printErrorDetailed(lineNumber, trimmed, "invalid number.");
            continue;
        }
        if (value < 0) {
            printErrorDetailed(lineNumber, trimmed, "not a positive number.");
            continue;
        }
        if (value > 1000) {
            printErrorDetailed(lineNumber, trimmed, "too large a number.");
            continue;
        }
        std::string matchedDate;
        double rate;
        if (!findRateForDate(date, matchedDate, rate)) {
            printErrorDetailed(lineNumber, trimmed, "no exchange data available.");
            continue;
        }
        printSuccessDetailed(date, value, matchedDate, rate);
    }
}

// Retrieves the exact or closest prior rate for the supplied date.
bool BitcoinExchange::findRateForDate(const std::string &date, std::string &matchedDate, double &rate) const {
    if (_rates.empty())
        return false;
    std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);
    if (it != _rates.end() && it->first == date) {
        matchedDate = it->first;
        rate = it->second;
        return true;
    }
    if (it == _rates.begin())
        return false;
    if (it == _rates.end() || it->first > date)
        --it;
    matchedDate = it->first;
    rate = it->second;
    return true;
}

// Validates that a date string has the YYYY-MM-DD format and represents a real day.
bool BitcoinExchange::isValidDate(const std::string &date) {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    for (size_t i = 0; i < date.size(); ++i) {
        if (i == 4 || i == 7)
            continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i])))
            return false;
    }
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    if (month < 1 || month > 12)
        return false;
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year))
        days = 29;
    if (day < 1 || day > days)
        return false;
    return true;
}

// Determines whether a given year is a leap year.
bool BitcoinExchange::isLeapYear(int year) {
    if (year % 400 == 0)
        return true;
    if (year % 100 == 0)
        return false;
    return year % 4 == 0;
}

// Strips leading and trailing whitespace from a string.
std::string BitcoinExchange::trim(const std::string &str) {
    std::string::size_type start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
        ++start;
    std::string::size_type end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
        --end;
    return str.substr(start, end - start);
}

// Attempts to parse a numeric token into a double without trailing junk.
bool BitcoinExchange::parseNumber(const std::string &token, double &value) {
    if (token.empty())
        return false;
    std::istringstream iss(token);
    iss >> value;
    if (iss.fail())
        return false;
    char leftover;
    if (iss >> leftover)
        return false;
    return true;
}
