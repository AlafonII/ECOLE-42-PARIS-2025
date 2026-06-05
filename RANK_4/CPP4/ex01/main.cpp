/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alafon <alafon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/27 09:43:25 by alafon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Cat.hpp"
#include "includes/Dog.hpp"
#include "includes/Animal.hpp"
#include "includes/Brain.hpp"

static void	print_header(const std::string test_name)
{
	std::cout << "\n===== " << test_name << " =====\n";
}

static void	test_polymorphic_sound(void)
{
	print_header("Polymorphic Sound Test");
	const Animal	*meta = new Animal();
	const Animal	*dog = new Dog();
	const Animal	*cat = new Cat();

	std::cout << dog->getType() << " makes sound: ";
	dog->makeSound();
	std::cout << cat->getType() << " makes sound: ";
	cat->makeSound();
	std::cout << "Animal makes sound: ";
	meta->makeSound();

	delete meta;
	delete dog;
	delete cat;
}

static void	test_array_creation_deletion(void)
{
	print_header("Array Creation/Deletion Test");
	const int	size = 4;
	Animal	*animals[size];

	for (int i = 0; i < size; ++i)
	{
		if (i < size / 2)
			animals[i] = new Dog();
		else
			animals[i] = new Cat();
	}
	for (int i = 0; i < size; ++i)
		delete animals[i];
}

static void	test_deep_copy_cat(void)
{
	print_header("Deep Copy Cat Test");
	Cat		original;
	original.getBrain()->setIdea("Chase the red dot", 0);
	Cat		copy = original;

	std::cout << "Original Cat idea[0]: "
		<< original.getBrain()->getIdea(0) << "\n";
	std::cout << "Copy Cat idea[0]: "
		<< copy.getBrain()->getIdea(0) << "\n";

	copy.getBrain()->setIdea("Sleep on the keyboard", 0);
	std::cout << "After modifying copy:\n";
	std::cout << "Original Cat idea[0]: "
		<< original.getBrain()->getIdea(0) << "\n";
	std::cout << "Copy Cat idea[0]: "
		<< copy.getBrain()->getIdea(0) << "\n";
}

static void	test_deep_copy_dog(void)
{
	print_header("Deep Copy Dog Test");
	Dog		original;
	original.getBrain()->setIdea("Fetch the ball", 1);
	Dog		copy(original);

	std::cout << "Original Dog idea[1]: "
		<< original.getBrain()->getIdea(1) << "\n";
	std::cout << "Copy Dog idea[1]: "
		<< copy.getBrain()->getIdea(1) << "\n";

	copy.getBrain()->setIdea("Dig a hole", 1);
	std::cout << "After modifying copy:\n";
	std::cout << "Original Dog idea[1]: "
		<< original.getBrain()->getIdea(1) << "\n";
	std::cout << "Copy Dog idea[1]: "
		<< copy.getBrain()->getIdea(1) << "\n";
}

static void	test_brain_ideas(void)
{
	print_header("Brain Ideas Bounds Test");
	Brain	brain;
	brain.setIdea("Idea zero", 0);
	brain.setIdea("Idea ninety-nine", 99);

	std::cout << "Idea[0]: "
		<< brain.getIdea(0) << "\n";
	std::cout << "Idea[99]: "
		<< brain.getIdea(99) << "\n";
	std::cout << "Idea[100](wrap): "
		<< brain.getIdea(100) << "\n";
	std::cout << "Idea[-1](adjust to 0): "
		<< brain.getIdea(-1) << "\n";
}

int	main(void)
{
	test_polymorphic_sound();
	test_array_creation_deletion();
	test_deep_copy_cat();
	test_deep_copy_dog();
	test_brain_ideas();
	return (0);
}
