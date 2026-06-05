/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 09:21:01 by alafon            #+#    #+#             */
/*   Updated: 2025/10/17 11:15:50 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <list>
# include <string>
# include <ctime>
# include <stdexcept>
# include <algorithm>

class PmergeMe {
public:
    PmergeMe();
    PmergeMe(const PmergeMe &);
    PmergeMe &operator=(const PmergeMe &);
    ~PmergeMe();

    void run(int argc, char **argv);

private:
    // Core Ford–Johnson sorting
    void fordJohnson(std::vector<int> &v);
    void fordJohnson(std::list<int> &l);

    // Helpers
    void parseInput(int argc, char **argv, std::vector<int> &v, std::list<int> &l);
    std::vector<size_t> jacobsthalSequence(size_t size);
    void insertSorted(std::vector<int> &v, int value);
    void insertSorted(std::list<int> &l, int value);
    void display(const std::string &label, const std::vector<int> &v);

    // Run + timing
    void runVector(std::vector<int> v);
    void runList(std::list<int> l);
};

#endif
