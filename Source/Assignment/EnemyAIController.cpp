// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "EnemyAIController.h"
#include "Enemy.h"
#include "AssignmentCharacter.h"
/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


AEnemyAIController::AEnemyAIController(/*const class FPostConstructInitializeProperties& PCIP*/)
/*: Super(PCIP)*/
{
	// create blackboard and behaviour components in the constructor
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));

	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	bWantsPlayerState = true;
}



void AEnemyAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemy* _tempEnemy = Cast<AEnemy>(InPawn);

	// start behavior
	if (_tempEnemy && _tempEnemy->EnemyBehaviorTree)
	{

		BlackboardComp->InitializeBlackboard(*_tempEnemy->EnemyBehaviorTree->BlackboardAsset);

		// Get the enemy blackboard ID, and store it to access that blackboard key later.
		EnemyKeyID = BlackboardComp->GetKeyID("Enemy");
		EnemyPositionKeyID = BlackboardComp->GetKeyID("EnemyPosition");
		BehaviorComp->StartTree(*_tempEnemy->EnemyBehaviorTree);
		UE_LOG(LogClass, Log, TEXT("===========>>>> Got the enemy and the BT have been started"));
	}
}

void AEnemyAIController::BeginInactiveState()
{
	Super::BeginInactiveState();

	AGameState* GameState = GetWorld()->GameState;

	const float MinRespawnDelay = /*(GameState && GameState->GameModeClass) ? GetDefault<AGameMode>(GameState->GameModeClass)->MinRespawnDelay :*/ 1.0f;

	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemyAIController::Respawn, MinRespawnDelay);
}

void AEnemyAIController::Respawn()
{
	//	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}




void AEnemyAIController::SetEnemy(class APawn* InPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		BlackboardComp->SetValueAsVector(FName("N", EnemyPositionKeyID), InPawn->GetActorLocation());
		SetFocus(InPawn);
	}
}

class AAssignmentCharacter* AEnemyAIController::GetEnemy() const
{
	if (BlackboardComp)
	{
		return Cast<AAssignmentCharacter>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}

	return NULL;
}


void AEnemyAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	// Look toward focus
	FVector TheCenter = GetFocalPoint();
	if (!TheCenter.IsZero() && GetPawn())
	{
		FVector Direction = TheCenter - GetPawn()->GetActorLocation();
		FRotator TheNewRotation = Direction.Rotation();

		TheNewRotation.Yaw = FRotator::ClampAxis(TheNewRotation.Yaw);

		SetControlRotation(TheNewRotation);

		APawn* const _tempPawn = GetPawn();
		if (_tempPawn && bUpdatePawn)
		{
			_tempPawn->FaceRotation(TheNewRotation, DeltaTime);
		}

	}
}


bool AEnemyAIController::PawnCanBeSeen(APawn * target)
{
	if (target == NULL || GetPawn() == NULL)
	{
		return false;
	}
	FVector difference = target->GetActorLocation() - GetPawn()->GetActorLocation();
	float angle = FVector::DotProduct(difference, GetPawn()->GetActorRotation().Vector());

	if (LineOfSightTo(target, GetPawn()->GetActorLocation()) && angle >0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void AEnemyAIController::OnSearchForEnemy()
{
	APawn* _tempPawn = GetPawn();
	if (_tempPawn == NULL)
	{
		return;
	}

	const FVector _tempLocation = _tempPawn->GetActorLocation();
	float BestDistSq = MAX_FLT;
	AAssignmentCharacter* PlayerPawn = NULL;

	//foreach all pawns in world
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		if (PawnCanBeSeen(*It))
		{
			AAssignmentCharacter* TestPawn = Cast<AAssignmentCharacter>(*It);

			AEnemy* const _testEnemy = Cast<AEnemy>(TestPawn);

			if (_testEnemy)
			{
				//it is just another enemy, not player
			}
			else
			{
				if (TestPawn && TestPawn->GetIsStillAlive())
				{
					UE_LOG(LogClass, Log, TEXT(" ===================>>>>>  ENEMY SEEN %s "), *GetNameSafe(*It));
					const float _distanceSq = (TestPawn->GetActorLocation() - _tempLocation).SizeSquared();
					if (_distanceSq < BestDistSq)
					{
						BestDistSq = _distanceSq;
						PlayerPawn = TestPawn;
					}
				}
			}

		}
	}

	if (PlayerPawn)
	{
		// We saw someone, so set him as target.
		SetEnemy(PlayerPawn);
		UE_LOG(LogClass, Log, TEXT(" ===================>>>>>  Set Target"));

	}

}