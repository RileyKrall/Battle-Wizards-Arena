// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "WizardBattleProjectile.h"
#include "Wizard.h"
#include "VObject.h"
#include "VGrid.h"
#include "WizardPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SpellDamageInstance.h"
#include "Components/SphereComponent.h"

AWizardBattleProjectile::AWizardBattleProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AWizardBattleProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AWizardBattleProjectile::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherActor->IsA(AWizard::StaticClass()))) {
		Damage = NewObject<USpellDamageInstance>();
		Damage->setDamage(50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		Cast<AWizard>(OtherActor)->Damage(Damage);
		Destroy();
	}
	else if ((OtherActor != NULL) && (OtherActor != this) && (OtherActor->IsA(AVObject::StaticClass()))) {
		bool add = false;
		AVObject* hitChunk = Cast<AVObject>(Hit.GetActor());

		// Get Point of impact
		FVector HitPoint = hitChunk->GetVoxelSurfacePoint(Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z, add);

		int gridID = hitChunk->getVGridID();

		//AVGrid* grid = VoxMan->GetGrid(gridID);
		//grid->EditPointInGrid(HitPoint.X, HitPoint.Y, HitPoint.Z, add);

		DestroyVoxel(HitPoint.X, HitPoint.Y, HitPoint.Z, gridID, add);
	}

	//if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	//{
	//	OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	//
	//	Destroy();
	//}
}

bool AWizardBattleProjectile::OnHit_Validate(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizardBattleProjectile::DestroyVoxel_Implementation(int x, int y, int z, int gridID, bool change)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("test1 Role: %d"), Role));
	AWizardPlayerController* PC = Cast<AWizardPlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PC)) {
		VoxMan = PC->VoxMan;
		if (IsValid(VoxMan)) {
			AVGrid* grid = VoxMan->GetGrid(gridID);
			grid->EditPointInGrid(x, y, z, change);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Invalid VoxMan")));
		}
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Invalid PC")));
	}
	

	

}

bool AWizardBattleProjectile::DestroyVoxel_Validate(int x, int y, int z, int gridID, bool change)
{
	return true;
}
