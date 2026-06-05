/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 20:43:06 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ClapTrap.hpp"
#include "includes/ScavTrap.hpp"
#include <iostream>

int	main(void)
{
	std::cout << "🏟️ Welcome to the ScavTrap vs ClapTrap Arena! 🏟️" << std::endl;
	std::cout << "🤖 In one corner: *alafon*, the gate guardian!" << std::endl;
	std::cout << "🤖 In the other: *evaluator*, the veteran brawler!" << std::endl << std::endl;

	std::string	scavTrapName = "alafon";
	std::string	clapTrapName = "evaluator";

	ScavTrap	scavTrap(scavTrapName);
	ClapTrap	clapTrap(clapTrapName);

	// Round 1: ClapTrap strikes first
	std::cout << "⚔️ [Round 1] ClapTrap lunges in!" << std::endl;
	clapTrap.attack(scavTrapName);
	scavTrap.takeDamage(CLAPTRAP_DEF_ATTACK_DMG);
	std::cout << std::endl;

	// Round 2: ScavTrap retaliates
	std::cout << "🛡️ [Round 2] ScavTrap swings back!" << std::endl;
	scavTrap.attack(clapTrapName);
	clapTrap.takeDamage(SCAVTRAP_DEF_ATTACK_DMG);
	std::cout << std::endl;

	// Round 3: ClapTrap scrambles for repairs
	std::cout << "💗 [Round 3] ClapTrap scrapes together repairs!" << std::endl;
	clapTrap.beRepaired(5000);
	std::cout << std::endl;

	// Round 4: ScavTrap goes into gate-guard mode
	std::cout << "🚪 [Round 4] ScavTrap secures the gate!" << std::endl;
	scavTrap.guardGate();
	std::cout << std::endl;

	// Round 5: Epic repair spree
	std::cout << "🔄 [Round 5] ScavTrap enters an endless repair loop!" << std::endl;
	for (int i = 0; i < 5; ++i)
		scavTrap.beRepaired(5000);
	std::cout << std::endl;

	// Round 6: Friendly fire mishap
	std::cout << "😅 [Round 6] ScavTrap misfires on itself!" << std::endl;
	scavTrap.attack(scavTrapName);
	scavTrap.takeDamage(SCAVTRAP_DEF_ATTACK_DMG);
	std::cout << std::endl;

	// Round 7: Gate guard reprise
	std::cout << "🛡️ [Round 7] ScavTrap returns to Gate guarding mode!" << std::endl;
	scavTrap.guardGate();
	std::cout << std::endl;

	// Final Round: The last stand
	std::cout << "💥 [Final Round] ScavTrap takes a crushing blow!" << std::endl;
	scavTrap.takeDamage(5000);
	std::cout << std::endl;

	std::cout << "🏁 The duel concludes! Who will enter next time? 🏁" << std::endl;

	return (0);
}
