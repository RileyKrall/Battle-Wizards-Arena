// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WizardBattleProjectile.h"
#include "SpellDamageInstance.h"
#include "engine.h"
#include "SpellParent.generated.h"


UCLASS()
class WIZARDBATTLE_API ASpellParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool offCooldown;

	float cooldown;

	float duration;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void BeginSpellCast(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector, float AvailableResource);

	UFUNCTION(Server, WithValidation, unreliable)
	void CastSpell(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);
	virtual void CastSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);
	virtual bool CastSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);

	UFUNCTION(Server, WithValidation, unreliable)
	void ReleaseSpell(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);
	virtual void ReleaseSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);
	virtual bool ReleaseSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector);

	UPROPERTY()
	float ResourceCost = 0;

	
};
