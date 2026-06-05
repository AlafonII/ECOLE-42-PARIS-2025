/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:23:10 by alex              #+#    #+#             */
/*   Updated: 2025/06/23 18:50:14 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>

/*
 * ---------------------------------------------------------------
 * Filename: main.cpp
 * ---------------------------------------------------------------
 * Description:
 *   Reads an input file line by line, replaces all occurrences of
 *   a substring (s1) with another substring (s2) in each line, and
 *   writes the modified lines to a new file named "<filename>.replace".
 * ---------------------------------------------------------------
 */

 /*
 * Function: replaceAll
 * --------------------
 * Creates and returns a new string where every occurrence of 'from'
 * in 'str' is replaced by 'to'.
 *
 * Parameters:
 *   str  - The original string in which replacements are performed.
 *   from - The substring to search for.
 *   to   - The substring to replace each occurrence of 'from'.
 *
 * Returns:
 *   A new std::string with all replacements applied.
 *
 * Behavior:
 *   1. Searches 'str' for 'from' repeatedly using std::string::find().
 *   2. On each match, constructs a new string by concatenating:
 *        - The portion before the match,
 *        - The replacement string 'to',
 *        - The portion after the matched substring.
 *   3. Continues until no further occurrences of 'from' are found.
 */
static std::string	replaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t	pos; // index where 'from' is found in 'str'

	// keep replacing until no occurrence of 'from' remains
	while ((pos = str.find(from)) != std::string::npos)
	{
		// rebuild: prefix + replacement + suffix
		str = str.substr(0, pos) + to + str.substr(pos + from.length());
	}
	return (str); // return the fully replaced string
}

/*
 * Function: main
 * --------------
 * Entry point of the program.
 *
 * Behavior:
 *   1. Validates that exactly three arguments are provided:
 *        - input filename
 *        - s1 (the substring to replace)
 *        - s2 (the substring to insert)
 *   2. If s1 is empty, exits with status 1.
 *   3. Opens the input file for reading; on failure, reports error.
 *   4. Creates the output file "<filename>.replace"; on failure, reports error.
 *   5. Reads each line from input, applies replaceAll, and writes it
 *      to the output file, preserving line breaks.
 *   6. Returns 0 on success, 1 on any error.
 */
int	main(int argc, char **argv)
{
	// expect exactly three arguments: filename, s1, s2
	if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <filename> <s1> <s2>\n";
		return (1);
	}

	std::string	infile  = argv[1]; // input file path
	std::string	s1      = argv[2]; // substring to search for
	std::string	s2      = argv[3]; // substring to replace with

	if (s1.empty())
		return (1); // avoid infinite loop on empty search string

	std::ifstream	in(infile.c_str()); // open input file
	if (!in) // check open success
	{
		std::cerr << "Error: cannot open file '" << infile << "'\n";
		return (1);
	}

	// create output filename by appending ".replace"
	std::string	outfilename = infile + ".replace";
	std::ofstream	out(outfilename.c_str()); // open output file
	if (!out) // check creation success
	{
		std::cerr << "Error: cannot create file '" << outfilename << "'\n";
		return (1);
	}

	std::string	line; // buffer for each line
	// read input line by line and write replaced result
	while (std::getline(in, line))
		out << replaceAll(line, s1, s2) << '\n';

	return (0); // success
}
