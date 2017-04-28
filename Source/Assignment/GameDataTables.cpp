// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "GameDataTables.h"


// Sets default values
AGameDataTables::AGameDataTables()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameDataTables::BeginPlay()
{
	Super::BeginPlay();

	//In case I want to call it at the beginning, but the most safe way is to call it from the outside, porbably from the player class
	//OnFetchAllTables();
}

// Called every frame
void AGameDataTables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Get all the datat from the tables, store them inside a public structs, and print to screen
void AGameDataTables::OnFetchAllTables()
{
	//Any will be okay, not neccessarly to be this cell name
	static const FString ContextString(TEXT("Name"));

	//Get all the row names and stor them temporary here, the point is to define the amount of rows, the best way yet!
	TArray<FName> weaponsTableRowsNames = WeaponsTable->GetRowNames();

	//usually we used 0 as the start index, but a table have it' first row indexed as 1, other wise it will crash
	for (int32 i = 1; i < weaponsTableRowsNames.Num() + 1; i++)
	{
		FString IndexString = FString::FromInt((int32)i);
		FName IndexName = FName(*IndexString);

		FWeaponStruct* aStructRow = WeaponsTable->FindRow<FWeaponStruct>(IndexName, ContextString, true);
		AllWeaponsData.Add(aStructRow);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(AllWeaponsData.Num()));

	//Just a print to screen to check if I got all the values correctly!
	for (int32 c = 0; c < AllWeaponsData.Num(); c++)
	{
		FString message = TEXT(" Number: ") + FString::FromInt(c) + TEXT(" Name: ") + AllWeaponsData[c]->DisplayName + TEXT(" Icon: ") + AllWeaponsData[c]->Icon + TEXT(" Damage: ") + FString::FromInt(AllWeaponsData[c]->Damage) + TEXT(" Cooldown: ") + FString::FromInt(AllWeaponsData[c]->CooldownTime);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, message);
	}


	//Get all the row names and stor them temporary here, the point is to define the amount of rows, the best way yet!
	TArray<FName> missionsTableRowsNames = MissionsTable->GetRowNames();

	//usually we used 0 as the start index, but a table have it' first row indexed as 1, other wise it will crash
	for (int32 e = 1; e < missionsTableRowsNames.Num() + 1; e++)
	{
		FString IndexString = FString::FromInt((int32)e);
		FName IndexName = FName(*IndexString);

		FMissionStruct* aStructRow = MissionsTable->FindRow<FMissionStruct>(IndexName, ContextString, true);
		AllMissionsData.Add(aStructRow);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(AllMissionsData.Num()));

	//Just a print to screen to check if I got all the values correctly!
	for (int32 x = 0; x < AllMissionsData.Num(); x++)
	{
		FString message = TEXT(" Number: ") + FString::FromInt(x) + TEXT(" Kills: ") + FString::FromInt(AllMissionsData[x]->Kill) + TEXT(" Collects: ") + FString::FromInt(AllMissionsData[x]->Collect);
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, message);
	}
}

