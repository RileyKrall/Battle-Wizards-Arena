// Fill out your copyright notice in the Description page of Project Settings.

#include "Wizard.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "SpellDamageInstance.h"
#include "FFAGM.h"
#include "WizardPlayerController.h"
#include "VObject.h"
#include "VGrid.h"
#include "UnrealNetwork.h"
#include "engine.h"

// Sets default values
AWizard::AWizard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	//Mesh1P->SetupAttachment(GetCapsuleComponent());
	this->GetMesh()->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	this->GetMesh()->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(this->GetMesh(), "helmet");
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 30.f, 150.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	SpellSource = FVector(0.f, 0.f, 150.f);
	//SpellSource = FirstPersonCameraComponent->GetComponentLocation() - GetActorLocation();//+ FVector(100.0f, 0.0f, 0.0f);
	Health = 100.0f;
	Resource = 100.0f;

	CanBlink = true;
	BlinkCooldown = 5;


}

// Called when the game starts or when spawned
void AWizard::BeginPlay()
{
	Super::BeginPlay();

	BaseResistance = NewObject<UResistanceVector>();
	BaseResistance->setResistance(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

	UWorld* const World = GetWorld();

	AWizardPlayerController* PC = Cast<AWizardPlayerController>(World->GetFirstPlayerController());
	if (IsValid(PC)) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("valid PC")));
		this->VoxMan = PC->VoxMan;
		if (!IsValid(VoxMan)) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Invalid voxMan")));
		}
	}
}

// Called every frame
void AWizard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bVoxelEdit) {
		DestroyVoxel();
	}
}

// Called to bind functionality to input
void AWizard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Select Memorized Spells
	PlayerInputComponent->BindAction("Select1", IE_Pressed, this, &AWizard::SelectSlot1);
	PlayerInputComponent->BindAction("Select2", IE_Pressed, this, &AWizard::SelectSlot2);
	PlayerInputComponent->BindAction("Select3", IE_Pressed, this, &AWizard::SelectSlot3);
	PlayerInputComponent->BindAction("Select4", IE_Pressed, this, &AWizard::SelectSlot4);
	PlayerInputComponent->BindAction("Select5", IE_Pressed, this, &AWizard::SelectSlot5);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AWizard::toggleTypeEdit);
	PlayerInputComponent->BindAction("EditMap", IE_Pressed, this, &AWizard::toggleEdit);
	PlayerInputComponent->BindAction("EditMap", IE_Released, this, &AWizard::toggleEdit);

	// Cast Prepared Spell
	PlayerInputComponent->BindAction("Cast", IE_Pressed, this, &AWizard::CastBegin);
	PlayerInputComponent->BindAction("Cast", IE_Released, this, &AWizard::CastRelease);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AWizard::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWizard::MoveRight);
	PlayerInputComponent->BindAction("BlinkForward", IE_Released, this, &AWizard::blinkForward);
	PlayerInputComponent->BindAction("BlinkRight", IE_Released, this, &AWizard::blinkRight);
	PlayerInputComponent->BindAction("BlinkBack", IE_Released, this, &AWizard::blinkBack);
	PlayerInputComponent->BindAction("BlinkLeft", IE_Released, this, &AWizard::blinkLeft);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWizard::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWizard::LookUpAtRate);


}


void AWizard::MoveForward(float value)
{
	if (value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AWizard::MoveRight(float value)
{
	if (value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AWizard::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWizard::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//Do this at game start or on spell change
void AWizard::SpellSetup_Implementation()
{
	MemorizedSpells.Empty();
	
	//Set Array of spell classes to size of 5, if not full;
	if (MemorizedSpellClasses.Num() <= 5) {
		while (MemorizedSpellClasses.Num() != 5)
		{
			MemorizedSpellClasses.Add(NULL);
		}
	}

	//Maybe attatch to player in future
	const FRotator SpawnRotation = GetControlRotation();
	const FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	for (int32 i = 0; i < 5; i++) {
		if (MemorizedSpellClasses[i] != NULL) {
			MemorizedSpells.Add(GetWorld()->SpawnActor<ASpellParent>(MemorizedSpellClasses[i], SpawnLocation, SpawnRotation, ActorSpawnParams));
			MemorizedSpells[i]->SetOwner(this);
		}
		else {
			MemorizedSpells.Add(NULL);
		}
	}

}

bool AWizard::SpellSetup_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::SelectSlot1_Implementation()
{
	if (MemorizedSpells.Num() >= 1) {
		Spell = MemorizedSpells[0];
		CurrentSpellSlot = 0;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Selected slot 1")));
	}


	if (Spell == NULL) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("NULL")));
	}
}

bool AWizard::SelectSlot1_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::SelectSlot2_Implementation()
{
	if (MemorizedSpells.Num() >= 2) {
		Spell = MemorizedSpells[1];
		CurrentSpellSlot = 1;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Selected slot 2")));
	}
}

bool AWizard::SelectSlot2_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::SelectSlot3_Implementation()
{
	if (MemorizedSpells.Num() >= 3) {
		Spell = MemorizedSpells[2];
		CurrentSpellSlot = 2;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Selected slot 3")));
	}
}

bool AWizard::SelectSlot3_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::SelectSlot4_Implementation()
{
	if (MemorizedSpells.Num() >= 4) {
		Spell = MemorizedSpells[3];
		CurrentSpellSlot = 3;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Selected slot 4")));
	}
}

bool AWizard::SelectSlot4_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::SelectSlot5_Implementation()
{
	if (MemorizedSpells.Num() >= 5) {
		Spell = MemorizedSpells[4];
		CurrentSpellSlot = 4;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Selected slot 5")));
	}
}

bool AWizard::SelectSlot5_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::CastBegin_Implementation()
{

	if (Spell != NULL) {
		const FRotator SpawnRotation = GetControlRotation();
		const FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));
		const FVector ForwardVector = SpawnRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));
		Spell->BeginSpellCast(SpawnRotation, SpawnLocation, ForwardVector, GetResource());
	}

}

bool AWizard::CastBegin_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::CastRelease_Implementation()
{

	if (Spell != NULL) {
		const FRotator SpawnRotation = GetControlRotation();
		const FVector SpawnLocation = FirstPersonCameraComponent->GetComponentLocation() + SpawnRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));
		const FVector ForwardVector = SpawnRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));
		Spell->ReleaseSpell(SpawnRotation, SpawnLocation, ForwardVector);
	}


}

bool AWizard::CastRelease_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::Damage_Implementation(USpellDamageInstance* Damage)
{

	float temp = Damage->getPhysical() * (1 - BaseResistance->getPhysical());
	temp += Damage->getMystic() * (1 - BaseResistance->getMystic());
	temp += Damage->getFire() * (1 - BaseResistance->getFire());
	temp += Damage->getElectric() * (1 - BaseResistance->getElectric());
	temp += Damage->getCorrosive() * (1 - BaseResistance->getCorrosive());
	temp += Damage->getMental() * (1 - BaseResistance->getMental());
	temp += Damage->getFrost() * (1 - BaseResistance->getFrost());

	
	Health = Health - temp;

	if (Health <= 0) {
		Health = 0;
		PlayerDeath();
	}
	else if(Health > 100) {

		Health = 100;
	}


	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health: %f"), Health));
}

bool AWizard::Damage_Validate(USpellDamageInstance* Damage)
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::Heal_Implementation(float heal)
{

	Health = Health + heal;

	if (Health <= 0) {
		Health = 0;		
	}
	else if (Health > 100) {

		Health = 100;
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Health: %f"), Health));

}

bool AWizard::Heal_Validate(float heal)
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::ReduceResource_Implementation(float cost)
{
	Resource = Resource - cost;
	if (Resource < 0) {
		Resource = 0;
	}
	else if (Resource > 100) {

		Resource = 100;
	}
}

bool AWizard::ReduceResource_Validate(float cost)
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizard::PlayerDeath()
{
	if (Role == ROLE_Authority) {
		KillPlayer();
		Cast<AWizardPlayerController>(GetController())->RequestRespawn();
	}
}

void AWizard::KillPlayer_Implementation()
{
	GetCapsuleComponent()->DestroyComponent();
	this->GetCharacterMovement()->DisableMovement();
	this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	this->GetMesh()->SetAllBodiesSimulatePhysics(true);
	//timer then destroy actor
	
}

bool AWizard::KillPlayer_Validate()
{
	return true;
}

void AWizard::blinkLeft_Implementation()
{
	const FRotator BlinkRotation = GetControlRotation();
	const FVector Location = GetActorLocation() + BlinkRotation.RotateVector(FVector(0.0f, -100.0f, 0.0f));
	const FVector forwardVector = BlinkRotation.RotateVector(FVector(0.0f, 1.0f, 0.0f));

	UWorld* const World = GetWorld();
	if (World != NULL && BlinkCooldown) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		FVector End = (forwardVector*-1000.f) + Location;
		DrawDebugLine(World, Location, End, FColor::Red, false, 1, 0, 1);
		if (World->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams)) {
			FVector hitLocation = Hit.Location;
			TeleportTo(hitLocation, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
		else {
			FVector rotate = GetControlRotation().RotateVector(FVector(0.0f, -1000.0f, 0.0f));
			FVector location = GetActorLocation() + rotate;
			TeleportTo(location, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
	}
}

bool AWizard::blinkLeft_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizard::blinkRight_Implementation()
{
	const FRotator BlinkRotation = GetControlRotation();
	const FVector Location = GetActorLocation() + BlinkRotation.RotateVector(FVector(0.0f, 100.0f, 0.0f));
	const FVector forwardVector = BlinkRotation.RotateVector(FVector(0.0f, 1.0f, 0.0f));

	UWorld* const World = GetWorld();
	if (World != NULL && BlinkCooldown) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		FVector End = (forwardVector*1000.f) + Location;
		DrawDebugLine(World, Location, End, FColor::Red, false, 1, 0, 1);
		if (World->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams)) {
			FVector hitLocation = Hit.Location;
			TeleportTo(hitLocation, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
		else {
			FVector rotate = GetControlRotation().RotateVector(FVector(0.0f, 1000.0f, 0.0f));
			FVector location = GetActorLocation() + rotate;
			TeleportTo(location, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
	}
}

bool AWizard::blinkRight_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizard::blinkForward_Implementation()
{
	const FRotator BlinkRotation = GetControlRotation();
	const FVector Location = FirstPersonCameraComponent->GetComponentLocation() + BlinkRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));
	const FVector forwardVector = BlinkRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));

	UWorld* const World = GetWorld();
	if (World != NULL && BlinkCooldown) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		FVector End = (forwardVector*1000.f) + Location;
		DrawDebugLine(World, Location, End, FColor::Red, false, 1, 0, 1);
		if (World->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams)) {
			FVector hitLocation = Hit.Location;
			TeleportTo(hitLocation, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
		else {
			FVector rotate = GetControlRotation().RotateVector(FVector(1000.0f, 0.0f, 0.0f));
			FVector location = GetActorLocation() + rotate;
			TeleportTo(location, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
	}

}

bool AWizard::blinkForward_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizard::blinkBack_Implementation()
{
	const FRotator BlinkRotation = GetControlRotation();
	const FVector Location = FirstPersonCameraComponent->GetComponentLocation() + BlinkRotation.RotateVector(FVector(-100.0f, 0.0f, 0.0f));
	const FVector forwardVector = BlinkRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));

	UWorld* const World = GetWorld();
	if (World != NULL && BlinkCooldown) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		FVector End = (forwardVector*-1000.f) + Location;
		DrawDebugLine(World, Location, End, FColor::Red, false, 1, 0, 1);
		if (World->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams)) {
			FVector hitLocation = Hit.Location;
			TeleportTo(hitLocation, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
		else {
			FVector rotate = GetControlRotation().RotateVector(FVector(-1000.0f, 0.0f, 0.0f));
			FVector location = GetActorLocation() + rotate;
			TeleportTo(location, GetActorRotation());
			CanBlink = false;
			StartCooldown();
		}
	}
}

bool AWizard::blinkBack_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizard::toggleEdit()
{
	bVoxelEdit = !bVoxelEdit;
}

void AWizard::toggleTypeEdit()
{
	bVoxelTypeEdit = !bVoxelTypeEdit;
}

void AWizard::InteractWith_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Attempting to interact")));
	const FRotator LookRotation = GetControlRotation();
	const FVector Location = FirstPersonCameraComponent->GetComponentLocation() + LookRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));
	const FVector forwardVector = LookRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));

	UWorld* const World = GetWorld();
	if (World != NULL) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		FVector End = (forwardVector*1000.f) + Location;
		DrawDebugLine(World, Location, End, FColor::Red, false, 1, 0, 1);
		if (World->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams)) {
			if (Hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass())) {

				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple , FString::Printf(TEXT("Interacted With")));
			}
			
		}
	}
}

bool AWizard::InteractWith_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizard::DestroyVoxel()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("basic call: %d"), Role));
	const FRotator LookRotation = GetControlRotation();
	const FVector Location = FirstPersonCameraComponent->GetComponentLocation() + LookRotation.RotateVector(FVector(100.0f, 0.0f, 0.0f));
	const FVector forwardVector = LookRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));

	UWorld* const World = GetWorld();
	if (World != NULL) {

		FHitResult Hit;
		FCollisionQueryParams CollisionParams;

		FVector End = (forwardVector*1000.f) + Location;
		//DrawDebugLine(World, Location, End, FColor::Red, false, 1, 0, 1);

		if (World->LineTraceSingleByChannel(Hit, Location, End, ECC_Visibility, CollisionParams)) {

			if (Hit.GetActor()->IsA(AVObject::StaticClass())) {

				AVObject* hitChunk = Cast<AVObject>(Hit.GetActor());

				// Get Point of impact
				FVector HitPoint = hitChunk->GetVoxelSurfacePoint(Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.ImpactPoint.Z, bVoxelTypeEdit);

				// Get VGrid to edit
				int gridID = hitChunk->getVGridID();

				if (Role == ROLE_Authority) {
					EditVoxel(HitPoint.X, HitPoint.Y, HitPoint.Z, gridID, bVoxelTypeEdit);
				}
				else {
					CallVoxelChangeToServer(HitPoint.X, HitPoint.Y, HitPoint.Z, gridID, bVoxelTypeEdit);
				}
				// Send to VGrid to handle edits
				//grid->EditRadiusInGrid(HitPoint.X, HitPoint.Y, HitPoint.Z, 10, false);
				//grid->EditPointInGrid(HitPoint.X, HitPoint.Y, HitPoint.Z, add);
				//grid->EditWholeVoxelAndSurroundingInGrid(HitPoint.X, HitPoint.Y, HitPoint.Z, add);
			}

		}
	}
}




void AWizard::SetSpellDataTable(UDataTable * input)
{
	MasterSpellList = input;
}

/*
void AWizard::AddSpellToKnownSpells(FName SpellName)
{
	GetSpellFromDataTable(SpellName);
}

void AWizard::AddAllSpellsToKnownList()
{
	TArray<FName> AllSpells = MasterSpellList->GetRowNames();

	for (auto& name : AllSpells) {
		KnownSpellList.Add(name);
	}
}

*/
void AWizard::AddResource(float input)
{
	ReduceResource(-input);
}

void AWizard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWizard, Health);
	DOREPLIFETIME(AWizard, Resource);
	DOREPLIFETIME(AWizard, Spell);
	DOREPLIFETIME(AWizard, SpellSource);
	DOREPLIFETIME(AWizard, BaseResistance);
	DOREPLIFETIME(AWizard, CanBlink);
	DOREPLIFETIME(AWizard, CurrentSpellSlot);
	DOREPLIFETIME(AWizard, MemorizedSpells);
	DOREPLIFETIME(AWizard, MemorizedSpellClasses);
}

ASpellParent * AWizard::GetSpell()
{
	return Spell;
}

float AWizard::GetHealth()
{
	return Health;
}

float AWizard::GetResource()
{
	return Resource;
}

void AWizard::StartCooldown_Implementation()
{
	UWorld* const World = GetWorld();
	
	FTimerHandle BlinkCooldownHandle;
	GetWorldTimerManager().SetTimer(BlinkCooldownHandle, this, &AWizard::SetOffCooldown, BlinkCooldown, false);
}

bool AWizard::StartCooldown_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}


void AWizard::SetOffCooldown_Implementation()
{
	CanBlink = true;
}

bool AWizard::SetOffCooldown_Validate()
{
	if (Role == ROLE_Authority) {
		return true;
	}
	else {
		return false;
	}
}

void AWizard::CallVoxelChangeToServer_Implementation(int x, int y, int z, int gridID, bool change)
{
	EditVoxel(x, y, z, gridID, change);
}

bool AWizard::CallVoxelChangeToServer_Validate(int x, int y, int z, int gridID, bool change)
{
	return true;
}

void AWizard::EditVoxel_Implementation(int x, int y, int z, int gridID, bool change)
{
	AVGrid* grid = VoxMan->GetGrid(gridID);
	grid->EditPointInGrid(x, y, z, change);
}

bool AWizard::EditVoxel_Validate(int x, int y, int z, int gridID, bool change)
{
	return true;
}
