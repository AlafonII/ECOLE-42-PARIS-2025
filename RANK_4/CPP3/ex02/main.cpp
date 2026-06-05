/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 22:21:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ClapTrap.hpp"
#include "includes/ScavTrap.hpp"
#include "includes/FragTrap.hpp"
#include <iostream>

int	main(void)
{
	std::cout << "🏟️ Welcome to the Chaos Arena! 🏟️" << std::endl;
	std::cout << "🤖 Fighters: *alafon*, *evaluator*, *evil_evaluator*" << std::endl << std::endl;

	std::string	fragTrapName = "alafon";
	std::string	scavTrapName = "evaluator";
	std::string	clapTrapName = "evil_evaluator";

	FragTrap	fragTrap(fragTrapName);
	ScavTrap	scavTrap(scavTrapName);
	ClapTrap	clapTrap(clapTrapName);

	// Round 1: FragTrap asks for high fives
	std::cout << "🙌 [Round 1] FragTrap shows off its high-five skills!" << std::endl;
	fragTrap.highFivesGuys();
	std::cout << std::endl;

	// Round 2: ClapTrap vs ScavTrap
	std::cout << "⚔️ [Round 2] ClapTrap lunges at ScavTrap!" << std::endl;
	clapTrap.attack(scavTrapName);
	scavTrap.takeDamage(CLAPTRAP_DEF_ATTACK_DMG);
	std::cout << std::endl;

	// Round 3: ScavTrap counter-attacks FragTrap
	std::cout << "🦾 [Round 3] ScavTrap crushes FragTrap!" << std::endl;
	scavTrap.attack(fragTrapName);
	fragTrap.takeDamage(SCAVTRAP_DEF_ATTACK_DMG);
	std::cout << std::endl;

	// Round 4: FragTrap scrambles repairs
	std::cout << "💗 [Round 4] FragTrap scrambles to repair itself!" << std::endl;
	fragTrap.beRepaired(5);
	std::cout << std::endl;

	// Round 5: Gate guard defense
	std::cout << "🚪 [Round 5] ScavTrap locks down the gate!" << std::endl;
	scavTrap.guardGate();
	std::cout << std::endl;

	// Round 6: FragTrap onslaught
	std::cout << "🔥 [Round 6] FragTrap unleashes a hundred attacks!" << std::endl;
	for (int i = 0; i < 5; ++i)
	{
		fragTrap.attack(scavTrapName);
		scavTrap.takeDamage(FRAGTRAP_DEF_ATTACK_DMG);
	}
	std::cout << std::endl;

	// Round 7: ScavTrap healing moment
	std::cout << "🔄 [Round 7] ScavTrap sneaks in some repairs!" << std::endl;
	scavTrap.beRepaired(20);
	std::cout << std::endl;

	// Round 8: FragTrap encore high-five
	std::cout << "🙌 [Round 8] FragTrap begs for another high-five!" << std::endl;
	fragTrap.highFivesGuys();
	std::cout << std::endl;

	// Final Round: FragTrap knocked out
	std::cout << "💥 [Final Round] FragTrap takes a massive hit!" << std::endl;
	fragTrap.takeDamage(5000);
	std::cout << std::endl;

	// Epilogue
	std::cout << "🫡 [Epilogue] Even in defeat, FragTrap offers one last salute..." << std::endl;
	fragTrap.highFivesGuys();
	std::cout << std::endl;

	std::cout << "🏁 The free-for-all concludes! Who will rise next time? 🏁" << std::endl;

	return (0);
}
