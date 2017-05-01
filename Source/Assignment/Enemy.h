// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class ASSIGNMENT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Triggers, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* bodySphereTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Triggers, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* leftHandTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Triggers, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* rightHandTrigger;

public:
	//The constructor
	AEnemy();

	//Override the PostInitializeComponents()
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// The attack effect on health
	UFUNCTION(BlueprintCallable, Category = "Enemy Attributes")
		void OnChangeHealthByAmount(float usedAmount);

	//The health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		float TotalHealth;

	//The range for the enemy attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		float AttackRange;

	//The power of the enemy attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		float AttackDamage;

	//Check if the enemy is dead or alive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		bool IsDead;

	//Check if the enemy is dead or alive
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Behavior")
		bool IsAttacking;


	//The sensing component used to see or hear the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy AI")
		class UPawnSensingComponent* PawnSensor;

	//The used BT with that enemy
	UPROPERTY(EditAnywhere, Category = "Enemy AI")
		class UBehaviorTree* EnemyBehaviorTree;

	//Perform attack
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
		void OnPerformAttack();

	//Perform attack
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
		void OnPreAttack();

	//Perform attack done
	UFUNCTION(BlueprintCallable, Category = "Enemy AI")
		void OnPostAttack();

	//Hear the player's noise using the sensing component
	UFUNCTION()
		void OnHearNoise(APawn *OtherActor, const FVector &Location, float Volume);

	//See the player's by sight using the sensing component
	UFUNCTION()
		void OnSeePawn(APawn *OtherPawn);

	UFUNCTION()
		void OnHandTriggerOverlap(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//float AccumulatedFiretime;

	//bool IsFirstPerson(); //virtual bool IsFirstPerson() const override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;
	bool Attacking;

	/** Returns sphere trigger subobject **/
	FORCEINLINE class USphereComponent* GetBodySphereTrigger() const { return bodySphereTrigger; }
};
