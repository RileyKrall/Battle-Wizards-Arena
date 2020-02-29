// Fill out your copyright notice in the Description page of Project Settings.


#include "WizardPlayerController.h"
#include "Wizard.h"
#include "FFAGM.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealNetwork.h"


AWizardPlayerController::AWizardPlayerController() {

}

void AWizardPlayerController::BeginPlay()
{

}

void AWizardPlayerController::RequestRespawn_Implementation()
{
	//Will want to generalize gamemodes
	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	if (IsValid(GM)) {
		AFFAGM* custGM = Cast<AFFAGM>(GM);
		custGM->RespawnPlayer(this);
	}
}

bool AWizardPlayerController::RequestRespawn_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

//Do this upon game entry
void AWizardPlayerController::ManageSpells_Implementation(const TArray<FName>& KnownSpells, const TArray<TSubclassOf<class ASpellParent>>& MemSpellClasses)
{

	MemorizedSpellClasses = MemSpellClasses;
	KnownSpellList = KnownSpells;

}

bool AWizardPlayerController::ManageSpells_Validate(const TArray<FName>& KnownSpells, const TArray<TSubclassOf<class ASpellParent>>& MemSpellClasses)
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

//Do this upon wizard creation
void AWizardPlayerController::SendMemorizedSpells_Implementation()
{
	AWizard* MyCurrentChar = Cast<AWizard>(this->GetPawn());
	if (MyCurrentChar) {
		//Set Wizard Mem spells
		MyCurrentChar->MemorizedSpellClasses.Empty();
		MyCurrentChar->MemorizedSpellClasses = MemorizedSpellClasses;

		MyCurrentChar->SpellSetup();

		MyCurrentChar->SelectSlot1();
	}

}

bool AWizardPlayerController::SendMemorizedSpells_Validate()
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizardPlayerController::ChangeSpellInSlot_Implementation(int slot, TSubclassOf<class ASpellParent> NewSpellCLass)
{
	while (MemorizedSpellClasses.Num() != 5) {
		MemorizedSpellClasses.Add(NULL);
	}

	MemorizedSpellClasses[slot] = NewSpellCLass;
	SendMemorizedSpells();
}

bool AWizardPlayerController::ChangeSpellInSlot_Validate(int slot, TSubclassOf<class ASpellParent> NewSpellCLass)
{
	if (Role < ROLE_Authority) {
		return false;
	}
	else {
		return true;
	}
}

void AWizardPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWizardPlayerController, MemorizedSpellClasses);
	DOREPLIFETIME(AWizardPlayerController, KnownSpellList);
}

void AWizardPlayerController::AddSpellToKnownSpells(FName SpellName)
{
}

void AWizardPlayerController::AddAllSpellsToKnownList()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("test1")));
	TArray<FName> AllSpells = MasterSpellList->GetRowNames();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("test2")));
	for (auto& name : AllSpells) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("testn")));
		KnownSpellList.Add(name);
	}
}

