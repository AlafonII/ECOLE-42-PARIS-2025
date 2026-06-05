/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:18:03 by alex              #+#    #+#             */
/*   Updated: 2025/05/26 13:12:47 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "includes/PhoneBook.hpp"
#include "includes/Contact.hpp"

static void promptField(const std::string &prompt, std::string &field) {
    do {
        std::cout << prompt;
        if (!std::getline(std::cin, field))
            std::exit(0);
        if (field.empty())
            std::cout << "Cannot be empty. Please try again." << std::endl;
    } while (field.empty());
}

int main() {
    PhoneBook phoneBook;
    std::string command;

    while (true) {
        std::cout << "Enter a command (ADD, SEARCH, EXIT): ";
        if (!std::getline(std::cin, command))
            break;
        if (command == "ADD") {
            Contact c;
            std::string input;
            promptField("First Name: ", input);
            c.setFirstName(input);
            promptField("Last Name: ", input);
            c.setLastName(input);
            promptField("Nickname: ", input);
            c.setNickname(input);
            promptField("Phone Number: ", input);
            c.setPhoneNumber(input);
            promptField("Darkest Secret: ", input);
            c.setDarkestSecret(input);
            phoneBook.addContact(c);
        }
        else if (command == "SEARCH") {
            if (phoneBook.getTotalContacts() == 0) {
                std::cout << "PhoneBook is empty." << std::endl;
                continue;
            }
            phoneBook.displayContacts();
            std::cout << "Enter index: ";
            std::string idxStr;
            if (!std::getline(std::cin, idxStr))
                break;
            if (idxStr.length() == 1 && std::isdigit(idxStr[0])) {
                int idx = idxStr[0] - '1';
                phoneBook.displayContact(idx);
            } else {
                std::cout << "Invalid index" << std::endl;
            }
        }
        else if (command == "EXIT") {
            break;
        }
    }
    return 0;
}
