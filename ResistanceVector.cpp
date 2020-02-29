// Fill out your copyright notice in the Description page of Project Settings.

#include "ResistanceVector.h"

UResistanceVector::UResistanceVector() {
	PhysicalRes = 0;

	MysticRes = 0;

	FireRes = 0;

	ElectricRes = 0;

	CorrosiveRes = 0;

	MentalRes = 0;

	FrostRes = 0;

	TimeActive = 0;

	TimeRemaining = 0;
}

void UResistanceVector::setResistance(float Physical, float Mystic, float Fire, float Electric, float Corrosive, float Mental, float Frost, float TotalTimeActive)
{

	PhysicalRes = Physical;

	MysticRes = Mystic;

	FireRes = Fire;

	ElectricRes = Electric;

	CorrosiveRes = Corrosive;

	MentalRes = Mental;

	FrostRes = Frost;

	TimeActive = TotalTimeActive;

	TimeRemaining = TotalTimeActive;
}

float UResistanceVector::getPhysical()
{
	return PhysicalRes;
}

float UResistanceVector::getMystic()
{
	return MysticRes;
}

float UResistanceVector::getFire()
{
	return FireRes;
}

float UResistanceVector::getElectric()
{
	return ElectricRes;
}

float UResistanceVector::getCorrosive()
{
	return CorrosiveRes;
}

float UResistanceVector::getMental()
{
	return MentalRes;
}

float UResistanceVector::getFrost()
{
	return FrostRes;
}

void UResistanceVector::setPhysical(float New)
{
	PhysicalRes = New;
}

void UResistanceVector::setMystic(float New)
{
	MysticRes = New;
}

void UResistanceVector::setFire(float New)
{
	FireRes = New;
}

void UResistanceVector::setElectric(float New)
{
	ElectricRes = New;
}

void UResistanceVector::setCorrosive(float New)
{
	CorrosiveRes = New;
}

void UResistanceVector::setMental(float New)
{
	MentalRes = New;
}

void UResistanceVector::setFrost(float New)
{
	FrostRes = New;
}

void UResistanceVector::resetResistances()
{
	PhysicalRes = 0;
	MysticRes = 0;
	FireRes = 0;
	ElectricRes = 0;
	CorrosiveRes = 0;
	MentalRes = 0;
	FrostRes = 0;
}

void UResistanceVector::AddToVector(UResistanceVector * other)
{
	PhysicalRes += other->getPhysical();
	MysticRes += other->getMystic();
	FireRes += other->getFire();
	ElectricRes += other->getElectric();
	CorrosiveRes += other->getCorrosive();
	MentalRes += other->getMental();
	FrostRes += other->getFrost();
}

void UResistanceVector::RemoveFromVector(UResistanceVector * other)
{
	PhysicalRes -= other->getPhysical();
	MysticRes -= other->getMystic();
	FireRes -= other->getFire();
	ElectricRes -= other->getElectric();
	CorrosiveRes -= other->getCorrosive();
	MentalRes -= other->getMental();
	FrostRes += other->getFrost();
}


