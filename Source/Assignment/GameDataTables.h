// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "GameDataTables.generated.h"

USTRUCT(Blueprintable)
struct FWeaponStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString DisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 CooldownTime;

	//default constructor
	FWeaponStruct()
	{
		Icon = "None";
		DisplayName = "None";
		Damage = 0;
		CooldownTime = 0;
	}
};

USTRUCT(Blueprintable)
struct FMissionStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Kill;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 Collect;

	//default constructor
	FMissionStruct()
	{
		Kill = 0;
		Collect = 0;
	}
};

UCLASS()
class ASSIGNMENT_API AGameDataTables : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameDataTables();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//I used editanywhere, so I'll be able to assign it in the details panel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game DataTables")
		UDataTable* WeaponsTable;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game DataTables")
	TArray<FWeaponStruct*> AllWeaponsData;

	UFUNCTION(BlueprintCallable, Category = "Game DataTables")
		void OnFetchAllTables();

};