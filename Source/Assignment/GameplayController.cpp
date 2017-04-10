// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "Interactable.h"
#include "GameplayGameMode.h"
#include "GameplayController.h"
#include "AssignmentCharacter.h"
//#include "Engine.h"



void AGameplayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, 
		this, &AGameplayController::Interact);

}

void AGameplayController::Interact()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Interact"));
	if (CurrentInteractable)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("CurrentInteracble is valid!"));
		CurrentInteractable->Interact(this);
	}

}

void AGameplayController::AddItemToInventoryByID(FName ID)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("AddItemToInventoryByID"));
	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(GetWorld()->
		GetAuthGameMode());

	UDataTable* ItemTable = GameMode->GetItemDB();

	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	if (ItemToAdd)
	{
		Inventory.Add(*ItemToAdd);
	}
}