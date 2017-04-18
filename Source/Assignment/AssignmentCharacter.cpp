// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Assignment.h"
#include "Interactable.h"
#include "GameplayController.h"
#include "AssignmentCharacter.h"
#include "Engine.h"
//#include "GameDataTables.h"
//#include "PaperSpriteComponent.h"
//#include "GameDataTables.h"

//////////////////////////////////////////////////////////////////////////
// AAssignmentCharacter

AAssignmentCharacter::AAssignmentCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	TotalHealth = 100.f;
	AttackRange = 25.f;

	JumpingVelocity = 300.f;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

												// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	IsStillAlive = true;
	IsAttacking = false;
	WeaponIndex = 1;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	OnSetPlayerController(true);
}

//void AAssignmentCharacter::BeginPlay()
//{
//	/**
//	//Ask the datamanager to get all of the tables data at once and store them
//	//AGameDataTables dataHolder
//	for (TActorIterator<AGameDataTables> ActorItr(GetWorld()); ActorItr; ActorItr++)
//	{
//	if (ActorItr)
//	{
//	// print the instance name to screen
//	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
//	//	ActorItr->GetName());
//
//	// Call the fetch to the tables, now we get all the datat stored.
//	// Why? Simply because keep reading everytime from the table itself is
//	//	costly memory wise.
//	// The most safe method is to just read all the data at once and whenever
//	// needed just relate back to the pointer.
//	TablesInstance = *ActorItr;
//	TablesInstance->OnFetchAllTables();
//	}
//	}
//	*/
//}

//////////////////////////////////////////////////////////////////////////
// Input

void AAssignmentCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AAssignmentCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AAssignmentCharacter::StopJumping);

	InputComponent->BindAction("Attack", IE_Pressed, this, &AAssignmentCharacter::OnAttack);
	InputComponent->BindAction("ChangeWeapon", IE_Released, this, &AAssignmentCharacter::OnChangeWeapon);

	InputComponent->BindAxis("MoveForward", this, &AAssignmentCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAssignmentCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AAssignmentCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AAssignmentCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AAssignmentCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AAssignmentCharacter::TouchStopped);
}

void AAssignmentCharacter::Jump()
{

	if (IsControlable && !IsAttacking)
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.f;
	}
}

void AAssignmentCharacter::StopJumping()
{
	if (IsControlable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("StopJumping()"));
		bPressedJump = false;
		JumpKeyHoldTime = 0.f;
	}
}

void AAssignmentCharacter::OnAttack()
{
	if (IsControlable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("OnAttack()"));
		IsAttacking = true;
	}
}

void AAssignmentCharacter::OnPostAttack()
{
	if (IsControlable)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("OnPostAttack()"));
		IsAttacking = false;
	}
}

void AAssignmentCharacter::OnChangeWeapon()
{
	/**
	if (IsControlable)
	{
	if (WeaponIndex < TablesInstance->AllWeaponData.Num())
	{
	WeaponIndex++;
	}
	else
	{
	WeaponIndex = 1;
	}
	}
	*/
}

void AAssignmentCharacter::OnSetPlayerController(bool status)
{
	IsControlable = status;
}

void AAssignmentCharacter::OnChangeHealthByAmount(float usedAmount)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, TEXT("OnChangeHealthByAmount"));
	TotalHealth -= usedAmount;
	FOutputDeviceNull ar;
	this->CallFunctionByNameWithArguments(TEXT("ApplyGetDamageEffect"),
		ar, NULL, true);
}

void AAssignmentCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AAssignmentCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AAssignmentCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAssignmentCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAssignmentCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && IsControlable && !IsAttacking)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAssignmentCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && IsControlable && !IsAttacking)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AAssignmentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForInteractables();

	if (TotalHealth <= 0)
	{
		IsControlable = false;
		IsStillAlive = false;
	}
}

void AAssignmentCharacter::CheckForInteractables()
{
	FHitResult HitResult;

	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = (FollowCamera->GetForwardVector() * 550) + StartTrace;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	AGameplayController* Controller = Cast<AGameplayController>(GetController());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace,
		ECC_Visibility, QueryParams) && Controller)
	{
		// Check if the item we hit was a interactable item
		if (AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor()))
		{
			Controller->CurrentInteractable = Interactable;
			return;
		}
	}

	// If we didnt hit anything or the thing we hit wasnt an interactable, set the
	// currentInteractable to nullptr
	Controller->CurrentInteractable = nullptr;

}