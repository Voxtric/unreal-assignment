// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "AssignmentCharacter.h"
#include "GameplayController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AGameplayController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Utils")
	void AddItemToInventoryByID(FName ID);

	// The interactable that the player is currentlty looking at.
	// This will be set to nullptr if the player is not looking at an
	// interactable item
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class AInteractable* CurrentInteractable;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TArray<FInventoryItem> Inventory;


protected:
	void Interact();

	virtual void SetupInputComponent() override;

};
