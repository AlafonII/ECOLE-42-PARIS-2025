/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 14:38:33 by alex              #+#    #+#             */
/*   Updated: 2025/09/09 11:04:06 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ScalarConverter.hpp"

// ScalarConverter helpers
// This module inspects a string literal and prints it as char/int/float/double.
// It handles normal numbers (int/float/double), quoted/single chars, and the
// special pseudo literals: nan/±inf (with or without 'f').

static bool	isQuotedChar(const std::string& s)
{
	// Match a single-quoted character like:  'a'  or  '%'
	return s.size() == 3 && s[0] == '\'' && s[2] == '\'';
}

static bool	isSingleNonDigit(const std::string& s)
{
	// Accept exactly one non-digit character (e.g., "a", "*").
	// cast to unsigned char before isdigit to avoid UB on negative char values.
	return s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0]));
}

static bool	isPseudoFloat(const std::string& s)
{
	// Float pseudo-literals
	return s == "nanf" || s == "+inff" || s == "-inff";
}

static bool	isPseudoDouble(const std::string& s)
{
	// Double pseudo-literals
	return s == "nan" || s == "+inf" || s == "-inf";
}

static bool	isIntLiteral(const std::string& s)
{
	// Shape check for a base-10 integer: optional sign, then at least one digit
	if (s.empty()) return false;
	size_t i = 0;
	if (s[i] == '+' || s[i] == '-') ++i;
	if (i >= s.size()) return false; // only a sign, no digits
	for (; i < s.size(); ++i)
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	return true;
}

static bool	isFloatLiteral(const std::string& s)
{
	// Accept either pseudo-floats or a decimal with trailing 'f'
	if (isPseudoFloat(s)) return true;
	// Must be at least 2 chars and end with 'f' (e.g., "1.0f")
	if (s.size() < 2 || s[s.size() - 1] != 'f') return false;

	std::string core = s.substr(0, s.size() - 1); // drop trailing 'f'
	size_t i = 0;
	if (core[i] == '+' || core[i] == '-') ++i;

	bool hasDigit = false;         // have we seen any digit at all?
	bool hasDot = false;           // exactly one dot required
	bool hasDigitAfterDot = false; // track if there was a digit after the dot

	for (; i < core.size(); ++i)
	{
		char c = core[i];
		if (c == '.')
		{
			if (hasDot) return false; // multiple dots
			hasDot = true;
		}
		else if (std::isdigit(static_cast<unsigned char>(c)))
		{
			hasDigit = true;
			if (hasDot) hasDigitAfterDot = true;
		}
		else return false; // invalid character
	}
	// Require a dot and at least one digit somewhere
	return hasDot && (hasDigit || hasDigitAfterDot);
}

static bool	isDoubleLiteral(const std::string& s)
{
	// Accept either pseudo-doubles or a decimal WITHOUT trailing 'f'
	if (isPseudoDouble(s)) return true;
	if (s.empty()) return false;

	size_t i = 0;
	if (s[i] == '+' || s[i] == '-') ++i;

	bool hasDigit = false;
	bool hasDot = false;
	bool hasDigitAfterDot = false;

	for (; i < s.size(); ++i)
	{
		char c = s[i];
		if (c == '.')
		{
			if (hasDot) return false; // multiple dots
			hasDot = true;
		}
		else if (std::isdigit(static_cast<unsigned char>(c)))
		{
			hasDigit = true;
			if (hasDot) hasDigitAfterDot = true;
		}
		else return false; // invalid character
	}
	// Require a dot and at least one digit somewhere
	return hasDot && (hasDigit || hasDigitAfterDot);
}

static bool	isDisplayableChar(int v)
{
	// Printable ASCII range
	return v >= 32 && v <= 126;
}

static void	printChar(double v, bool valid)
{
	std::cout << "char: ";
	// Not valid, not finite, or outside ASCII code range → impossible
	if (!valid || !std::isfinite(v) || v < 0.0 || v > 127.0)
		std::cout << "impossible\n";
	// Non-printable (control characters, DEL) → "Non displayable"
	else if (!isDisplayableChar(static_cast<int>(v)))
		std::cout << "Non displayable\n";
	else
		// Truncate to int, then cast to char, and show quoted
		std::cout << '\'' << static_cast<char>(static_cast<int>(v)) << '\'' << "\n";
}

static void	printInt(double v, bool valid)
{
	std::cout << "int: ";
	// Guard against invalid/NaN/±inf and int out-of-range
	if (!valid || !std::isfinite(v) || v < static_cast<double>(INT_MIN) || v > static_cast<double>(INT_MAX))
		std::cout << "impossible\n";
	else
		// Truncate toward zero like a C-style cast/int conversion
		std::cout << static_cast<int>(v) << "\n";
}

static void	printFloat(float v, bool valid, bool isPseudo, const std::string& src)
{
	std::cout << "float: ";
	if (!valid)
	{
		// e.g., parsing failed
		std::cout << "impossible\n";
		return;
	}
	if (isPseudo)
	{
		// Print canonical pseudo tokens
		if (src == "nan" || src == "nanf") std::cout << "nanf\n";
		else if (src[0] == '-') std::cout << "-inff\n";
		else std::cout << "+inff\n";
		return;
	}
	// Normal float: print fixed with one decimal and 'f' suffix (e.g., 42.0f)
	std::cout << std::fixed << std::setprecision(1) << v << 'f' << "\n";
	// Reset formatting flags for future prints
	std::cout.unsetf(std::ios::floatfield);
}

static void	printDouble(double v, bool valid, bool isPseudo, const std::string& src)
{
	std::cout << "double: ";
	if (!valid)
	{
		std::cout << "impossible\n";
		return;
	}
	if (isPseudo)
	{
		// Print canonical pseudo tokens
		if (src == "nan" || src == "nanf") std::cout << "nan\n";
		else if (src[0] == '-') std::cout << "-inf\n";
		else std::cout << "+inf\n";
		return;
	}
	// Normal double: print fixed with one decimal (e.g., 42.0)
	std::cout << std::fixed << std::setprecision(1) << v << "\n";
	std::cout.unsetf(std::ios::floatfield);
}

void	ScalarConverter::convert(const std::string& s)
{
	// Case 1: treat as character (either 'x' or single non-digit like x)
	if (isQuotedChar(s) || isSingleNonDigit(s))
	{
		char c = isQuotedChar(s) ? s[1] : s[0];

		// Convert from char to other types via widening
		double asDouble = static_cast<double>(c);
		printChar(asDouble, true);
		printInt(asDouble, true);
		printFloat(static_cast<float>(asDouble), true, false, s);
		printDouble(asDouble, true, false, s);
		return;
	}

	// Case 2: pseudo literals (nan/±inf with or without 'f')
	if (isPseudoFloat(s) || isPseudoDouble(s))
	{
		// char & int are not representable
		printChar(0.0, false);
		printInt(0.0, false);
		// float/double printed as tokens
		bool isFloatToken = isPseudoFloat(s);
		printFloat(0.0f, true, true, s);
		printDouble(0.0,  true, true, s);
		(void)isFloatToken; // silence unused variable warning
		return;
	}

	// Case 3: decimal integer
	if (isIntLiteral(s))
	{
		// Parse using strtol to detect overflow/underflow/non-digit tails
		char *end = 0;
		long lv = std::strtol(s.c_str(), &end, 10);

		if (*end != '\0' || lv < INT_MIN || lv > INT_MAX)
		{
			// Not a clean in-range int; try parsing as double as a fallback
			char *end2 = 0;
			double dv = std::strtod(s.c_str(), &end2);

			std::cout << "char: impossible\n";
			std::cout << "int: impossible\n";

			if (*end2 == '\0' && std::isfinite(dv))
			{
				// Valid finite double; print float & double variants
				float fv = static_cast<float>(dv);
				printFloat(fv, true, false, s);
				printDouble(dv, true, false, s);
			}
			else
			{
				// Not a valid number at all
				printFloat(0.0f, false, false, s);
				printDouble(0.0,  false, false, s);
			}
			return;
		}

		// Clean, in-range int: fan out to all types
		int iv = static_cast<int>(lv);
		double asDouble = static_cast<double>(iv);
		printChar(asDouble, true);
		printInt(asDouble, true);
		printFloat(static_cast<float>(asDouble), true, false, s);
		printDouble(asDouble, true, false, s);
		return;
	}

	// Case 4: float literal (must end with 'f' and pass the shape test)
	if (isFloatLiteral(s))
	{
		// Pseudos were already allowed by isFloatLiteral; handle them explicitly
		if (isPseudoFloat(s))
		{
			printChar(0.0, false);
			printInt(0.0, false);
			printFloat(0.0f, true, true, s);
			printDouble(0.0,  true, true, s);
			return;
		}

		// Strip trailing 'f' for parsing
		std::string core = s.substr(0, s.size() - 1);
		char *end = 0;
		double dv = std::strtod(core.c_str(), &end);

		if (*end != '\0')
		{
			// Not fully consumed → invalid numeric
			printChar(0.0, false);
			printInt(0.0, false);
			printFloat(0.0f, false, false, s);
			printDouble(0.0,  false, false, s);
			return;
		}

		// Convert to float and print all four
		float fv = static_cast<float>(dv);
		bool finite = std::isfinite(dv);
		printChar(static_cast<double>(fv), finite);
		printInt(static_cast<double>(fv), finite);
		printFloat(fv, finite, false, s);
		printDouble(static_cast<double>(fv), finite, false, s);
		return;
	}

	// Case 5: double literal (decimal or pseudo)
	if (isDoubleLiteral(s))
	{
		if (isPseudoDouble(s))
		{
			printChar(0.0, false);
			printInt(0.0, false);
			printFloat(0.0f, true, true, s);
			printDouble(0.0,  true, true, s);
			return;
		}

		// Parse as double
		char *end = 0;
		double dv = std::strtod(s.c_str(), &end);

		if (*end != '\0')
		{
			// Not fully consumed → invalid numeric
			printChar(0.0, false);
			printInt(0.0, false);
			printFloat(0.0f, false, false, s);
			printDouble(0.0,  false, false, s);
			return;
		}

		// Valid double → print all four
		bool finite = std::isfinite(dv);
		printChar(dv, finite);
		printInt(dv, finite);
		printFloat(static_cast<float>(dv), finite, false, s);
		printDouble(dv, finite, false, s);
		return;
	}

	// No recognized form
	std::cout << "char: impossible\nint: impossible\nfloat: impossible\ndouble: impossible\n";
}
