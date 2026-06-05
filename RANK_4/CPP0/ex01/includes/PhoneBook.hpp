/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:17:40 by alex              #+#    #+#             */
/*   Updated: 2025/05/26 13:09:58 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include "Contact.hpp"

class PhoneBook {
private:
    Contact contacts[8];
    int nextIndex;
    int totalContacts;

public:
    PhoneBook();
    void addContact(const Contact&);
    void displayContacts() const;
    void displayContact(int index) const;
    int getTotalContacts() const;
};

#endif