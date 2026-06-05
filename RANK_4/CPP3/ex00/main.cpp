/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/25 23:42:38 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/ClapTrap.hpp"
#include <iostream>

int	main(void)
{
	std::cout << "🏟️ Welcome to the ClapTrap Arena! 🏟️" << std::endl;
	std::cout << "Introducing today’s contenders:" << std::endl;
	std::cout << "*alafon* — the relentless rookie" << std::endl;
	std::cout << "*evaluator*  — the seasoned veteran" << std::endl << std::endl;

	std::string	trapName0 = "alafon";
	std::string	trapName1 = "evaluator";

	ClapTrap	trap0(trapName0);
	ClapTrap	trap1(trapName1);

	std::cout << "🔔 Let the battle commence! 🔔" << std::endl << std::endl;

	// Round 1: Rookie tests repair protocols
	std::cout << "[Round 1] *alafon* attempts self-repair under no damage..." << std::endl;
	trap0.beRepaired(0);
	std::cout << std::endl;

	// Round 2: alafon unleashes a flurry of attacks
	std::cout << "[Round 2] *alafon* launches a barrage of attacks!" << std::endl;
	for (int i = 0; i < 9; ++i)
	{
		trap0.attack(trapName1);
		trap1.takeDamage(CLAPTRAP_DEF_ATTACK_DMG);
	}
	std::cout << std::endl;

	// Round 3: Critical strike
	std::cout << "[Round 3] *alafon* goes for the finishing blow!" << std::endl;
	trap0.attack(trapName1);
	trap1.takeDamage(5);
	std::cout << std::endl;

	// Round 4: Veteran’s comeback
	std::cout << "[Round 4] *evaluator* repairs and retaliates!" << std::endl;
	trap1.beRepaired(4);
	trap1.attack(trapName0);
	trap0.takeDamage(5);
	std::cout << std::endl;

	// Round 5: Overcharge repairs
	std::cout << "[Round 5] Both fighters push their systems to the limit..." << std::endl;
	trap0.beRepaired(5000);
	trap1.beRepaired(5000);
	std::cout << std::endl;

	// Round 6: Friendly fire mishap
	std::cout << "[Round 6] *evaluator* misfires on itself!" << std::endl;
	trap1.attack(trapName1);
	trap1.takeDamage(CLAPTRAP_DEF_ATTACK_DMG);
	std::cout << std::endl;

	// Final Round: Last gasp
	std::cout << "[Final Round] The arena falls silent..." << std::endl;
	trap1.beRepaired(5000);
	trap1.takeDamage(1);
	std::cout << std::endl;

	std::cout << "🏁 And that’s a wrap! Who will rise next time?" << std::endl;

	return (0);
}
