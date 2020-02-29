// Fill out your copyright notice in the Description page of Project Settings.

#include "SpellParent.h"


// Sets default values
ASpellParent::ASpellParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpellParent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpellParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpellParent::BeginSpellCast(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector, float AvailableResource)
{
	CastSpell(spawnRotation, spawnLocation, forwardVector);
}

void ASpellParent::CastSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Cast Called"));
	if (Role < ROLE_Authority) {

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Client Spawn"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Server Spawn"));
	}

}

bool ASpellParent::CastSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void ASpellParent::ReleaseSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{

}

bool ASpellParent::ReleaseSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}