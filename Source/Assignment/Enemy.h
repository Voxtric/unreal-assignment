// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class ASSIGNMENT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// checks if enemy is still alive
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Attributes")
		bool IsStillAlive;

	// knows if enemy is attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Attributes")
		bool IsAttacking;
	
};
