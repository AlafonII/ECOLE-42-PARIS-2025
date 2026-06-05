/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MutantStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:08:41 by alex              #+#    #+#             */
/*   Updated: 2025/09/25 11:37:33 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP

#include <stack>

template <typename T, typename Container = std::deque<T> >
class MutantStack : public std::stack<T, Container> {
public:
	MutantStack() : std::stack<T, Container>() {}
	MutantStack(MutantStack const& other) : std::stack<T, Container>(other) {}
	MutantStack& operator=(MutantStack const& other) {
		if (this != &other) {
			std::stack<T, Container>::operator=(other);
		}
		return *this;
	}
	~MutantStack() {}

	// iterator typedefs
	typedef typename Container::iterator iterator;
	typedef typename Container::const_iterator const_iterator;

	iterator begin() { return this->c.begin(); }
	iterator end()   { return this->c.end(); }
	const_iterator begin() const { return this->c.begin(); }
	const_iterator end()   const { return this->c.end(); }
};

#endif

