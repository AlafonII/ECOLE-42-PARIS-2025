/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 14:08:41 by alex              #+#    #+#             */
/*   Updated: 2025/09/11 10:13:17 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>
#include <stdexcept>

template <typename T>
class Array {
public:
	Array() : _data(0), _size(0) {}

	Array(unsigned int n) : _data(0), _size(n) {
		if (n > 0) {
			_data = new T[n]();
		}
	}

	Array(Array const& other) : _data(0), _size(other._size) {
		if (_size > 0) {
			_data = new T[_size];
			for (unsigned int i = 0; i < _size; ++i) {
				_data[i] = other._data[i];
			}
		}
	}

	Array& operator=(Array const& other) {
		if (this != &other) {
			T* newBlock = 0;
			if (other._size > 0) {
				newBlock = new T[other._size];
				for (unsigned int i = 0; i < other._size; ++i) {
					newBlock[i] = other._data[i];
				}
			}
			delete [] _data;
			_data = newBlock;
			_size = other._size;
		}
		return *this;
	}

	~Array() {
		delete [] _data;
	}

	T& operator[](unsigned int idx) {
		if (idx >= _size) {
			throw std::out_of_range("Array: index out of bounds");
		}
		return _data[idx];
	}

	T const& operator[](unsigned int idx) const {
		if (idx >= _size) {
			throw std::out_of_range("Array: index out of bounds");
		}
		return _data[idx];
	}

	unsigned int size() const {
		return _size;
	}

private:
	T* _data;
	unsigned int _size;
};

#endif
