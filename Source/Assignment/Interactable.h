// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class ASSIGNMENT_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The function that is called when the player interacts with the interactable
	//This is marker as blueprintimplementable so that designers msy implement
	//whatever custom functionality they want in blueprint.
	UFUNCTION(BlueprintImplementableEvent)
	void Interact(APlayerController* Controller);

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	FString Action;

	UFUNCTION(BlueprintCallable, Category = "Pickup")

	FString GetUseText(bool usingGamepad) const
	{
    if (usingGamepad)
    {
      return FString::Printf(TEXT("%s: Press \'X\' to %s"), *Name, *Action);
    }
    else
    {
      return FString::Printf(TEXT("%s: Press \'E\' to %s"), *Name, *Action);
    }
	}
	

};
