// Fill out your copyright notice in the Description page of Project Settings.

#include "MinorHeal.h"
#include "UnrealNetwork.h"
#include "Wizard.h"


// Sets default values
AMinorHeal::AMinorHeal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ResourceCost = 20;
}

// Called when the game starts or when spawned
void AMinorHeal::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMinorHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinorHeal::CastSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	if (GetOwner() == NULL) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Owner Null"));
	}
	if ((Target != NULL) && (Target != this) && (Target != GetOwner()) && (Target->IsA(AWizard::StaticClass()))) {
		Cast<AWizard>(Target)->Heal(10.0f);
	}
}

bool AMinorHeal::CastSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

AActor* AMinorHeal::LineTraceActor(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector) 
{
	UWorld* const World = GetWorld();
	if (World != NULL) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;



		FVector End = (forwardVector*3000.f) + spawnLocation;
		DrawDebugLine(World, spawnLocation, End, FColor::Red, false, 1, 0, 1);
		if (World->LineTraceSingleByChannel(Hit, spawnLocation, End, ECC_Visibility, CollisionParams)) {
			return Hit.GetActor();
		}
	}
	return NULL;
}


void AMinorHeal::BeginSpellCast(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector, float AvailableResource)
{
	if (AvailableResource >= ResourceCost) {
		Cast<AWizard>(GetOwner())->ReduceResource(ResourceCost);
		Target = LineTraceActor(spawnRotation, spawnLocation, forwardVector);
		CastSpell(spawnRotation, spawnLocation, forwardVector);
	}
}

void AMinorHeal::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMinorHeal, Target);
}