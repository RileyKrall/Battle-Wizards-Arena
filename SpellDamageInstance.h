// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellDamageInstance.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API USpellDamageInstance : public UObject
{
	GENERATED_BODY()
	
public:

	USpellDamageInstance();

	void setDamage(float Physical, float Mystic, float Fire, float Electric, float Corrosive, float Mental, float Frost);

private:

	UPROPERTY()
	float PhysicalDamage;

	UPROPERTY()
	float MysticDamage;

	UPROPERTY()
	float FireDamage;

	UPROPERTY()
	float ElectricDamage;

	UPROPERTY()
	float CorrosiveDamage;

	UPROPERTY()
	float MentalDamage;

	UPROPERTY()
	float FrostDamage;

public:

	float getPhysical();

	float getMystic();

	float getFire();

	float getElectric();

	float getCorrosive();

	float getMental();

	float getFrost();
	
	
	
};
