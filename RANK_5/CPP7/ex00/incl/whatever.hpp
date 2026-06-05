/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whatever.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:08:41 by alex              #+#    #+#             */
/*   Updated: 2025/09/11 09:54:42 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHATEVER_HPP
#define WHATEVER_HPP

template <typename T>
void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template <typename T>
T const& min(T const& a, T const& b) {
	if (b <= a) {
		return b;
	}
	return a;
}

template <typename T>
T const& max(T const& a, T const& b) {
	if (b >= a) {
		return b;
	}
	return a;
}

#endif
