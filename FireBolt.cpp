// Fill out your copyright notice in the Description page of Project Settings.

#include "FireBolt.h"
#include "UnrealNetwork.h"
#include "Wizard.h"


// Sets default values
AFireBolt::AFireBolt()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ResourceCost = 20;
	cooldown = 1;
	offCooldown = true;
}

// Called when the game starts or when spawned
void AFireBolt::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFireBolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFireBolt::CastSpell_Implementation(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	UWorld* const World = GetWorld();
	if (World != NULL) {
		AWizard* Caster = Cast<AWizard>(GetOwner());
		Caster->ReduceResource(ResourceCost);
		StartCooldown();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AWizardBattleProjectile* proj = World->SpawnActor<AWizardBattleProjectile>(bolt, spawnLocation, spawnRotation, ActorSpawnParams);
	}
}

bool AFireBolt::CastSpell_Validate(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector)
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AFireBolt::StartCooldown_Implementation()
{
	UWorld* const World = GetWorld();
	offCooldown = false;
	FTimerHandle FireBoltCooldownHandle;
	GetWorldTimerManager().SetTimer(FireBoltCooldownHandle, this, &AFireBolt::SetOffCooldown, cooldown, false);
}

bool AFireBolt::StartCooldown_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}


void AFireBolt::SetOffCooldown_Implementation()
{
	offCooldown = true;
}

bool AFireBolt::SetOffCooldown_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AFireBolt::BeginSpellCast(FRotator spawnRotation, FVector spawnLocation, FVector forwardVector, float AvailableResource)
{
	if (AvailableResource >= ResourceCost && offCooldown) {
		CastSpell(spawnRotation, spawnLocation, forwardVector);
	}

}

void AFireBolt::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFireBolt, offCooldown);

}