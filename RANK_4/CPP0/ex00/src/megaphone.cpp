/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:26:10 by alex              #+#    #+#             */
/*   Updated: 2025/05/26 13:45:43 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <cctype>

int	main(int ac, char **av)
{
	std::string const	msg = "* LOUD AND UNBEARABLE FEEDBACK NOISE *";

	if (ac < 2)
	{
		std::cout << msg << std::endl;
		return (0);
	}
	for (int i = 1; av[i]; i++)
	{
		std::string	str(av[i]);
		for (std::string::iterator s = str.begin(); s != str.end(); s++)
			*s = std::toupper(*s);
		std::cout << str;
		if (av[i + 1])
			std::cout << ' ';
	}
	std::cout << std::endl;
	return (0);
}
