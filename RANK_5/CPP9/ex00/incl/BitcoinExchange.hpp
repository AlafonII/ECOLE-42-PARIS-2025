/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:20:31 by alafon            #+#    #+#             */
/*   Updated: 2025/10/16 09:20:32 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange {
public:
    BitcoinExchange();
    explicit BitcoinExchange(const std::string &databasePath);
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    void loadDatabase(const std::string &databasePath);
    void processInput(const std::string &inputPath) const;

private:
    std::map<std::string, double> _rates;

    bool findRateForDate(const std::string &date, std::string &matchedDate, double &rate) const;

    static bool isValidDate(const std::string &date);
    static bool isLeapYear(int year);
    static std::string trim(const std::string &str);
    static bool parseNumber(const std::string &token, double &value);
};

#endif
