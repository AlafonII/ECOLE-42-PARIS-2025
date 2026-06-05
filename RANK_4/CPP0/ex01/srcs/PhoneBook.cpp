/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:17:05 by alex              #+#    #+#             */
/*   Updated: 2025/05/26 13:18:52 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/PhoneBook.hpp"
#include <iostream>
#include <iomanip>

PhoneBook::PhoneBook() : nextIndex(0), totalContacts(0) {}

void	PhoneBook::addContact(const Contact &c) {
    contacts[nextIndex] = c;
    if (totalContacts < 8)
        totalContacts++;
    nextIndex = (nextIndex + 1) % 8;
}

int	PhoneBook::getTotalContacts() const {
    return totalContacts;
}

static void	printField(std::string const &str)
{
    if (str.length() > 10)
    {
        std::cout << str.substr(0, 9) << '.';
    }
    else
    {
        std::cout << std::setw(10) << str;
    }
}

void	PhoneBook::displayContacts() const
{
    std::cout << std::setw(10) << "Index"      << "|"
              << std::setw(10) << "First Name" << "|"
              << std::setw(10) << "Last Name"  << "|"
              << std::setw(10) << "Nickname"   << std::endl;

    for (int i = 0; i < totalContacts; i++)
    {
        std::cout << std::setw(10) << i + 1 << "|";
        printField(contacts[i].getFirstName());
        std::cout << "|";
        printField(contacts[i].getLastName());
        std::cout << "|";
        printField(contacts[i].getNickname());
        std::cout << std::endl;
    }
}

void	PhoneBook::displayContact(int index) const {
    if (index < 0 || index >= totalContacts) {
        std::cout << "Invalid index" << std::endl;
        return;
    }
    const Contact &c = contacts[index];
    std::cout << "First Name: "   << c.getFirstName()   << std::endl;
    std::cout << "Last Name: "    << c.getLastName()    << std::endl;
    std::cout << "Nickname: "     << c.getNickname()    << std::endl;
    std::cout << "Phone Number: " << c.getPhoneNumber() << std::endl;
    std::cout << "Darkest Secret: "<< c.getDarkestSecret()<< std::endl;
}
