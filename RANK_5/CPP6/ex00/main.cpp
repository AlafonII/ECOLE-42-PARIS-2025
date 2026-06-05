/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:40:09 by alex              #+#    #+#             */
/*   Updated: 2025/08/28 14:40:11 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "incl/ScalarConverter.hpp"
#include <iostream>
#include <string>

static void	run(const std::string& s)
{
	std::cout << "\n=== \"" << s << "\" ===\n";
	ScalarConverter::convert(s);
}

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		ScalarConverter::convert(std::string(argv[1]));
		return 0;
	}

	// Thorough edge-case suite (quick to scan)
	run("'a'");          // quoted char
	run("z");            // single char literal (non-digit)
	run("0");            // int zero
	run("-42");          // negative int
	run("2147483647");   // INT_MAX
	run("-2147483648");  // INT_MIN
	run("2147483648");   // overflow -> impossible int

	run("0.0f");         // float with .0f
	run("-4.2f");        // negative float
	run("+123.0f");      // explicit +
	run("nanf");         // pseudo float
	run("+inff");        // pseudo float +inf
	run("-inff");        // pseudo float -inf

	run("0.0");          // double with .0
	run("4.2");          // positive double
	run("-4.2");         // negative double
	run("nan");          // pseudo double
	run("+inf");         // pseudo double +inf
	run("-inf");         // pseudo double -inf

	// Notation & format rejections
	run("42.");          // requires decimals on at least one side; still accepted here as double ('.' present)
	run(".42");          // accepted as double (leading dot)
	run("42f");          // invalid (no '.')
	run("'\\n'");        // we don't handle escaped quoted chars -> impossible
	run("a1");           // invalid
	run("--1");          // invalid
	run("1.2.3");        // invalid

	return 0;
}
