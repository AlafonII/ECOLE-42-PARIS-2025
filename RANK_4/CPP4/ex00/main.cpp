/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 13:11:35 by alafon            #+#    #+#             */
/*   Updated: 2025/06/26 18:39:56 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/WrongCat.hpp"
#include "includes/WrongAnimal.hpp"
#include "includes/Cat.hpp"
#include "includes/Dog.hpp"
#include "includes/Animal.hpp"

static void	print_header(const std::string test_name)
{
	std::cout << "\n===== " << test_name << " =====\n";
}

static void	testBasic(void)
{
	print_header("Basic Test");
	const Animal* meta = new Animal();
	const Animal* j    = new Dog();
	const Animal* i    = new Cat();

	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;
	i->makeSound();  // Cat sound
	j->makeSound();  // Dog sound
	meta->makeSound(); // Animal sound

	delete meta;
	delete j;
	delete i;
}

static void	testPolymorphism(void)
{
	print_header("Polymorphism Test");
	const Animal* base = new Animal();
	const Animal* dog  = new Dog();
	const Animal* cat  = new Cat();

	std::cout << "[dog->getType()] " << dog->getType() << std::endl;
	std::cout << "[cat->getType()] " << cat->getType() << std::endl;
	cat->makeSound();  // Cat sound
	dog->makeSound();  // Dog sound
	base->makeSound(); // Animal sound

	delete base;
	delete dog;
	delete cat;
}

static void	testWrongPolymorphism(void)
{
	print_header("Wrong Polymorphism Test");
	const WrongAnimal* base = new WrongAnimal();
	const WrongAnimal* wc   = new WrongCat();

	std::cout << "[wc->getType()] " << wc->getType() << std::endl;
	wc->makeSound();    // WrongCat sound?
	base->makeSound();  // WrongAnimal sound

	delete base;
	delete wc;
}

static void	testArrayOfAnimals(void)
{
	print_header("Array of Animals Test");
	const Animal* zoo[6];

	for (int i = 0; i < 3; i++)
		zoo[i] = new Dog();
	for (int i = 3; i < 6; i++)
		zoo[i] = new Cat();

	for (int i = 0; i < 6; i++)
		delete zoo[i];
}

static void	testCopy(void)
{
	print_header("Copy Constructors Test");
	Dog origDog;
	Dog copyDog = origDog;
	Cat origCat;
	Cat copyCat;

	copyCat = origCat;

	std::cout << "Original and copy make sounds:" << std::endl;
	origDog.makeSound();
	copyDog.makeSound();
	origCat.makeSound();
	copyCat.makeSound();
}

int	main(void)
{
	testBasic();
	testPolymorphism();
	testWrongPolymorphism();
	testArrayOfAnimals();
	testCopy();
	return (0);
}