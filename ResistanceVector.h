// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimedObject.h"
#include "UObject/NoExportTypes.h"
#include "ResistanceVector.generated.h"

/**
 * 
 */
UCLASS()
class WIZARDBATTLE_API UResistanceVector : public UObject
{

	GENERATED_BODY()

public:

	UResistanceVector();

	void setResistance(float Physical, float Mystic, float Fire, float Electric, float Corrosive, float Mental, float Frost, float TotalTimeActive);

private:

	UPROPERTY()
	float PhysicalRes;

	UPROPERTY()
	float MysticRes;

	UPROPERTY()
	float FireRes;

	UPROPERTY()
	float ElectricRes;

	UPROPERTY()
	float CorrosiveRes;

	UPROPERTY()
	float MentalRes;

	UPROPERTY()
	float FrostRes;

	UPROPERTY()
	float TimeActive;

	UPROPERTY()
	float TimeRemaining;

	UPROPERTY()
	bool NeedsTimeReduction;


public:

	float getPhysical();

	float getMystic();

	float getFire();

	float getElectric();

	float getCorrosive();

	float getMental();

	float getFrost();

	void setPhysical(float New);

	void setMystic(float New);

	void setFire(float New);

	void setElectric(float New);

	void setCorrosive(float New);

	void setMental(float New);

	void setFrost(float New);
	
	void resetResistances();

	void AddToVector(UResistanceVector* other);
	
	void RemoveFromVector(UResistanceVector* other);
};
