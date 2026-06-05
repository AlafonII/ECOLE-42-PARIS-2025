/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:08:41 by alex              #+#    #+#             */
/*   Updated: 2025/09/10 14:20:40 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ITER_HPP
#define ITER_HPP

#include <cstddef>

template <typename T, typename F>
void iter(T* array, std::size_t length, F func) {
	for (std::size_t i = 0; i < length; ++i) {
		func(array[i]);
	}
}

template <typename T, typename F>
void iter(T const* array, std::size_t length, F func) {
	for (std::size_t i = 0; i < length; ++i) {
		func(array[i]);
	}
}

#endif
