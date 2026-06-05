/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:20:43 by alafon            #+#    #+#             */
/*   Updated: 2025/10/17 11:34:32 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/RPN.hpp"

#include <cctype>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

RPN::RPN() {}
RPN::RPN(const RPN &other) { (void)other; }
RPN &RPN::operator=(const RPN &other) {
    (void)other;
    return *this;
}
RPN::~RPN() {}

// Parses and evaluates a reverse Polish notation expression.
int RPN::evaluate(const std::string &expression) const {
    if (expression.empty())
        throw std::runtime_error("Error");

    std::stack<int> values;
    std::istringstream tokens(expression);
    std::string token;

    while (tokens >> token) {
        if (token.size() == 1 && isOperator(token[0])) {
            if (values.size() < 2)
                throw std::runtime_error("Error");
            int rhs = values.top();
            values.pop();
            int lhs = values.top();
            values.pop();
            values.push(applyOperator(lhs, rhs, token[0]));
        } else {
            for (std::string::size_type i = 0; i < token.size(); ++i) {
                if (!std::isdigit(static_cast<unsigned char>(token[i])))
                    throw std::runtime_error("Error");
            }
            int value = std::atoi(token.c_str());
            values.push(value);
        }
    }

    if (values.size() != 1)
        throw std::runtime_error("Error");
    return values.top();
}

// Checks whether a token represents a supported arithmetic operator.
bool RPN::isOperator(char token) {
    return token == '+' || token == '-' || token == '*' || token == '/';
}

// Applies the requested operator to the top two operands from the stack.
int RPN::applyOperator(int lhs, int rhs, char op) {
    switch (op) {
    case '+':
        return lhs + rhs;
    case '-':
        return lhs - rhs;
    case '*':
        return lhs * rhs;
    case '/':
        if (rhs == 0)
            throw std::runtime_error("Error");
        return lhs / rhs;
    default:
        throw std::runtime_error("Error");
    }
}
