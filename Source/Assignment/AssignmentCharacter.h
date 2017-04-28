// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameDataTables.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "AssignmentCharacter.generated.h"

USTRUCT(BlueprintType)
struct FCraftingInfo : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ComponentID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ProductID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bDestroyItemB;
};

USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class APickup> ItemPickup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	TArray<FCraftingInfo> CraftCombinations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;

	bool operator==(const FInventoryItem& Item) const
	{
		if (ItemID == Item.ItemID)
			return true;
		else return false;
	}

	FInventoryItem()
	{
		Name = FText::FromString("Item");
		Action = FText::FromString("Use");
		Description = FText::FromString("Please enter a description for this item");
		Value = 10;
	}
};


UCLASS(config = Game)
class AAssignmentCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AAssignmentCharacter();

	virtual void Tick(float DeltaTime) override;

	//virtual void BeginPlay() override;

	//void SetUpDataTables();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Base Jump Velocity
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Attributes")
		float JumpingVelocity;

	// checks if player is still alive
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
		bool IsStillAlive;

	// knows if player is attacking
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
		bool IsAttacking;

	// The index of the current weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
		int32 WeaponIndex;

	// To be able to disable the player during cutscenes and menus and the such.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attributes")
		bool IsControlable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game DataTables")
		AGameDataTables* TablesInstance;

	// Returns IsStillAlive
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		bool GetIsStillAlive() const { return IsStillAlive; }

	// Returns IsAttacking
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		bool GetIsAttacking() const { return IsAttacking; }

	// enable or disable inputs
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		void OnSetPlayerController(bool status);

	// The attack effect on health
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		void OnChangeHealthByAmount(float usedAmount);

	// Returens total health
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		float OnGetHealthAmount() const { return TotalHealth; }

	// Returens total health in a value between 0 and 1.
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		float OnGetHealthPercentage() const { return TotalHealth / 100.f; }

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		float OnGetTimePercentage() const { return Time / 600.f; }

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		void OnResetPlayer();

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		void SaveValue(FString valName, float val);

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
		float LoadValue(FString valName);

	// post attack operations
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void OnPostAttack();



protected:

	void CheckForInteractables();

	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void MoveRight(float Value);

	// Called to apply jump action
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void Jump();

	// Called to stop jump and start animations of idle etc.
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void StopJumping();

	// Responsible for attacking
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void OnAttack();

	// Responsible for changing weapons
	UFUNCTION(BlueprintCallable, Category = "Player Actions")
		void OnChangeWeapon();

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	// Player TotalHealth
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
		float TotalHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
		float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
		int Score;

	// AttackRange
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Attributes")
		float AttackRange;

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

