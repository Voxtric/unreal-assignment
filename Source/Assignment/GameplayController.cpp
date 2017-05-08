// Fill out your copyright notice in the Description page of Project Settings.

#include "Assignment.h"
#include "Interactable.h"
#include "GameplayGameMode.h"
#include "GameplayController.h"
#include "AssignmentCharacter.h"
//#include "Engine.h"



void AGameplayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, 
		this, &AGameplayController::Interact);

}

void AGameplayController::SetMousePosition(float LocationX, float LocationY)
{
  const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
  int intX = (int)(ViewportSize.X / 100.0f * LocationX);
  int intY = (int)(ViewportSize.Y / 100.0f * LocationY);
  FViewport* v = CastChecked<ULocalPlayer>(this->Player)->ViewportClient->Viewport;
  v->SetMouse(intX, intY);
}

int32 AGameplayController::AddScore(TArray<int32>& scoreValues, TArray<FString>& scorePlayers, int32 score, FString playerName)
{
  int32 index = -1;
  if (score > scoreValues[9])
  {
    scoreValues[9] = score;
    scorePlayers[9] = playerName;
    index = 9;
    for (int i = 8; i >= 0; --i)
    {
      if (score > scoreValues[i])
      {
        scoreValues[i + 1] = scoreValues[i];
        scoreValues[i] = score;
        scorePlayers[i + 1] = scorePlayers[i];
        scorePlayers[i] = playerName;
        index = i;
      }
      else
      {
        break;
      }
    }
  }
  return index;
}

void AGameplayController::SaveScores(TArray<int32> scoreValues, TArray<FString> scorePlayers)
{
  FString fileName = FString("scoreValues");
  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
  IFileHandle* fileHandle = platformFile.OpenWrite(*fileName);
  if (fileHandle)
  {
    uint8* byteArray = reinterpret_cast<uint8*>(FMemory::Malloc(sizeof(int32)));
    for (unsigned int i = 0; i < 10; ++i)
    {
      int32 var = scoreValues[i];
      int32* intPointer = reinterpret_cast<int32*>(byteArray);
      *intPointer = var;
      fileHandle->Write(byteArray, sizeof(float));
    }
    delete fileHandle;
    FMemory::Free(byteArray);
  }

  fileName = FString("scorePlayers");
  platformFile = FPlatformFileManager::Get().GetPlatformFile();
  fileHandle = platformFile.OpenWrite(*fileName);
  if (fileHandle)
  {
    uint8* byteArray = reinterpret_cast<uint8*>(FMemory::Malloc(sizeof(TCHAR)));
    for (unsigned int i = 0; i < 10; ++i)
    {
      FString player = scorePlayers[i];
      for (unsigned int j = 0; j < 20; ++j)
      {
        TCHAR var = ' ';
        if (player.IsValidIndex(j))
        {
          var = (*player)[j];
        }
        TCHAR* charPointer = reinterpret_cast<TCHAR*>(byteArray);
        *charPointer = var;
        fileHandle->Write(byteArray, sizeof(TCHAR));
      }
    }
    delete fileHandle;
    FMemory::Free(byteArray);
  }
}

TArray<int32> AGameplayController::LoadScoreValues()
{
  TArray<int32> intTArray;
  intTArray.Init(0, 10);

  FString fileName = FString("scoreValues");
  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
  IFileHandle* fileHandle = platformFile.OpenRead(*fileName);
  if (fileHandle)
  {
    for (unsigned int i = 0; i < 10; ++i)
    {
      int32 var = 0;
      int32* intPointer = &var;
      uint8* byteBuffer = reinterpret_cast<uint8*>(intPointer);
      fileHandle->Read(byteBuffer, sizeof(int32));
      intTArray[i] = var;
    }
    delete fileHandle;
  }

  return intTArray;
}

TArray<FString> AGameplayController::LoadScorePlayers()
{
  TArray<FString> stringTArray;
  stringTArray.Init("", 10);

  FString fileName = FString("scorePlayers");
  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
  IFileHandle* fileHandle = platformFile.OpenRead(*fileName);
  if (fileHandle)
  {
    for (unsigned int i = 0; i < 10; ++i)
    {
      FString string = FString("");
      for (unsigned int j = 0; j < 20; ++j)
      {
        TCHAR var = ' ';
        TCHAR* charPointer = &var;
        uint8* byteBuffer = reinterpret_cast<uint8*>(charPointer);
        fileHandle->Read(byteBuffer, sizeof(TCHAR));
        string.AppendChar(var);
      }
      stringTArray[i] = string.TrimTrailing();
    }
    delete fileHandle;
  }

  return stringTArray;
}

void AGameplayController::SaveInventory()
{
  if (Inventory.Num() > 0)
  {
    FString fileName = FString("inventory");
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    IFileHandle* fileHandle = platformFile.OpenWrite(*fileName);
    if (fileHandle)
    {
      uint8* byteArray = reinterpret_cast<uint8*>(FMemory::Malloc(sizeof(TCHAR)));
      for (int i = 0; i < Inventory.Num(); ++i)
      {
        FString id = Inventory[i].ItemID.ToString();
        TCHAR var = (*id)[0];
        TCHAR* charPointer = reinterpret_cast<TCHAR*>(byteArray);
        *charPointer = var;
        fileHandle->Write(byteArray, sizeof(TCHAR));
      }
      delete fileHandle;
      FMemory::Free(byteArray);
    }
  }
}

void AGameplayController::LoadInventory()
{
  FString fileName = FString("inventory");
  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
  IFileHandle* fileHandle = platformFile.OpenRead(*fileName);
  if (fileHandle)
  {    
    bool eof = false;
    while (!eof)
    {
      TCHAR var = ' ';
      TCHAR* charPointer = &var;
      uint8* byteBuffer = reinterpret_cast<uint8*>(charPointer);
      eof = !fileHandle->Read(byteBuffer, sizeof(TCHAR));
      if (!eof)
      {
        FString id = FString("");
        id.AppendChar(var);
        AddItemToInventoryByID(FName(*id));        
      }
    }
    delete fileHandle;
    platformFile.DeleteFile(*fileName);
  }
}

void AGameplayController::Interact()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Interact"));
	if (CurrentInteractable)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("CurrentInteracble is valid!"));
		CurrentInteractable->Interact(this);
	}

}

void AGameplayController::AddItemToInventoryByID(FName ID)
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("AddItemToInventoryByID"));
	AGameplayGameMode* GameMode = Cast<AGameplayGameMode>(GetWorld()->
		GetAuthGameMode());

	UDataTable* ItemTable = GameMode->GetItemDB();

	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");

	if (ItemToAdd)
	{
		Inventory.Add(*ItemToAdd);
	}
}