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

  UFUNCTION(BlueprintCallable, Category = "Game|Player")
    void SetMousePosition(float LocationX, float LocationY);

  UFUNCTION(BlueprintCallable, Category = "Game")
    int32 AddScore(UPARAM(ref) TArray<int32>& scoreValues, UPARAM(ref) TArray<FString>& scorePlayers, int32 score, FString playerName);

  UFUNCTION(BlueprintCallable, Category = "Game")
    void SaveScores(TArray<int32> scoreValues, TArray<FString> scorePlayers);

  UFUNCTION(BlueprintCallable, Category = "Game")
    TArray<int32> LoadScoreValues();

  UFUNCTION(BlueprintCallable, Category = "Game")
    TArray<FString> LoadScorePlayers();

  UFUNCTION(BlueprintCallable, Category = "Game")
    void SaveInventory();

  UFUNCTION(BlueprintCallable, Category = "Game")
    void LoadInventory();

	


protected:
	void Interact();

	virtual void SetupInputComponent() override;

};
