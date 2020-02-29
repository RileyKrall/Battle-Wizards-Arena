// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellDamageInstance.h"

USpellDamageInstance::USpellDamageInstance() {

	PhysicalDamage = 0;

	MysticDamage = 0;

	FireDamage = 0;

	ElectricDamage = 0;

	CorrosiveDamage = 0;

	MentalDamage = 0;

	FrostDamage = 0;
}

void USpellDamageInstance::setDamage(float Physical, float Mystic, float Fire, float Electric, float Corrosive, float Mental, float Frost)
{

	PhysicalDamage = Physical;

	MysticDamage = Mystic;

	FireDamage = Fire;

	ElectricDamage = Electric;

	CorrosiveDamage = Corrosive;

	MentalDamage = Mental;

	FrostDamage = Frost;
}

float USpellDamageInstance::getPhysical()
{
	return PhysicalDamage;
}

float USpellDamageInstance::getMystic()
{
	return MysticDamage;
}

float USpellDamageInstance::getFire()
{
	return FireDamage;
}

float USpellDamageInstance::getElectric()
{
	return ElectricDamage;
}

float USpellDamageInstance::getCorrosive()
{
	return CorrosiveDamage;
}

float USpellDamageInstance::getMental()
{
	return MentalDamage;
}

float USpellDamageInstance::getFrost()
{
	return FrostDamage;
}