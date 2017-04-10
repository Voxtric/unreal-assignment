// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GameplayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AGameplayGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	UDataTable* GetItemDB() const
	{
		return ItemDB;
	}

protected:
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* ItemDB;
	
	
};
