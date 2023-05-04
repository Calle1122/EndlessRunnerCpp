// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunner/Public/RunnerGameModeBase.h"

#include "Obstacle.h"
#include "RunGameHUD.h"
#include "RunHitBox.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ARunnerGameModeBase::SaveGame()
{
	FLeaderboardItem NewItem;
	NewItem.Name1 = Player1Name;
	NewItem.Name2 = Player2Name;
	NewItem.Score = Score;
	SaveGameInstance->LeaderboardItems.Add(NewItem);

	ScoreboardItems.Add(NewItem);
	ScoreboardItems.Sort();

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Highscore"), 0);
}

void ARunnerGameModeBase::LoadGame()
{
	if(UGameplayStatics::DoesSaveGameExist("Highscore", 0))
	{
		SaveGameInstance = Cast<USaveFileHandler>(UGameplayStatics::LoadGameFromSlot("Highscore", 0));
	}
	else
	{
		SaveGameInstance = Cast<USaveFileHandler>(UGameplayStatics::CreateSaveGameObject(USaveFileHandler::StaticClass()));
	}

	for (FLeaderboardItem item : SaveGameInstance->LeaderboardItems)
	{
		ScoreboardItems.Add(item);
	}

	ScoreboardItems.Sort();
}

void ARunnerGameModeBase::BeginPlay()
{
	Player2 = Cast<AEndlessRunnerCharacter>(UGameplayStatics::CreatePlayer(GetWorld(), 1, true));
	
	Player1 = Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	Player2 = Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetCharacter());
	
	Player1Mesh = Player1->GetMesh();
	Player2Mesh = Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1))->GetMesh();

	Player2Mesh->SetMaterial(0, PinkBaseMat1);
	Player2Mesh->SetMaterial(1, PinkBaseMat2);
	Player2Mesh->SetMaterial(2, PinkBaseMat3);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->SetActorLocation(FVector(0,-250,100));
	UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetCharacter()->SetActorLocation(FVector(0,250,100));

	Player1Lane = 2;
	Player2Lane = 2;

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->Tags.Add(FName("p1"));
	UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetCharacter()->Tags.Add(FName("p2"));
	
	MainMenuWidget = CreateWidget<UMainMenuHUD>(GetWorld()->GetFirstPlayerController(), MainMenuInterface);
	MainMenuWidget->AddToViewport(9999);

	MainMenuWidget->InitializeMainMenu();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	
	GameOver = true;
}

void ARunnerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GameOver)
	{
		Player1->SetActorLocation(FVector(0,-250,100));
		Player2->SetActorLocation(FVector(0,250,100));

		return;
	}

	FVector Player1Loc = Player1->GetActorLocation();
	Player1->SetActorLocation(FVector(Player1Loc.X, Player1Loc.Y, FMath::Max(90, Player1Loc.Z)));
	
	FVector Player2Loc = Player2->GetActorLocation();
	Player2->SetActorLocation(FVector(Player2Loc.X, Player2Loc.Y, FMath::Max(90, Player2Loc.Z)));
	
	Score+=DeltaSeconds*ScoreMultiplier;
	if(IsValid(RunWidget))
	{
		RunWidget->ScoreTxt->SetText(FText::FromString(FString::FromInt((int)Score)));
	}

	MultiplierAddTimer+=DeltaSeconds/5;
	if(MultiplierAddTimer>=1.f)
	{
		MultiplierAddTimer=0.f;
		ChangeMultiplier(ScoreMultiplier+.1f);
	}

	ProjectileChanceFactor += DeltaSeconds/5;
	if(ProjectileChanceFactor>100.f)
	{
		ProjectileChanceFactor = 100.f;
	}
	ProjectilePercentChance = (int)ProjectileChanceFactor;
}

ARunnerGameModeBase::ARunnerGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void ARunnerGameModeBase::CreateInitialTiles()
{
	for(int i=0; i<NumberOfInitialTiles; i++)
	{
		AddTile();
	}
}

void ARunnerGameModeBase::AddTile()
{
	UWorld* World = GetWorld();

	if(World)
	{
		ARunTile* Tile;
		
		if(!NextTileArrow)
		{
			Tile = GetWorld()->SpawnActor<ARunTile>(BaseTileClass, FVector(-50,-250,0), FRotator(0,0,0));

			//Duplicate to right lane
			FVector TilePos = Tile->GetActorLocation();
			GetWorld()->SpawnActor<ARunTile>(BaseTileClass, FVector(TilePos.X, FMath::Abs(TilePos.Y), TilePos.Z), FRotator(0, 0, 0));
		}
		else
		{
			int32 index = FMath::RandRange(0, EasyTiles.Num() - 1);
			Tile = GetWorld()->SpawnActor<ARunTile>(EasyTiles[index], NextTileArrow->GetComponentTransform());

			//Duplicate to right lane
			FVector TilePos = Tile->GetActorLocation();
			GetWorld()->SpawnActor<ARunTile>(EasyTiles[index], FVector(TilePos.X, FMath::Abs(TilePos.Y), TilePos.Z), FRotator(0, 0, 0));
		}
		if(Tile)
		{
			NextTileArrow = Tile->GetAttachArrow();
		}
	}
}

void ARunnerGameModeBase::ReduceHealth(int PlayerIndex)
{
	if(PlayerIndex == 0)
	{
		if(P1IFrameMode==false)
		{
			Player1Health--;
			SetPlayerInvincible(0);

			GetWorldTimerManager().ClearTimer(P1IFrameHandle);
			GetWorldTimerManager().SetTimer(P1IFrameHandle, this, &ARunnerGameModeBase::P1EnableDamageTaking, IFrameTime, false);
		
			switch (Player1Health)
			{
			case 2:
				RunWidget->RemoveHealth(RunWidget->P1Health3Img);
				break;
			case 1:
				RunWidget->RemoveHealth(RunWidget->P1Health2Img);
				break;
			case 0:
				RunWidget->RemoveHealth(RunWidget->P1Health1Img);
				break;
			}
	
			if(Player1Health<=0)
			{
				P1Dead=true;

				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("visibility to false for P1"));
				Player1Mesh->SetVisibility(false);
				Player1->SetActorEnableCollision(false);

				GetWorldTimerManager().ClearTimer(RespawnHandleP1);
				GetWorldTimerManager().SetTimer(RespawnHandleP1, this, &ARunnerGameModeBase::RespawnPlayer1, RespawnTime, false);
				TryEndRun();
			}
		}
	}

	else
	{
		if(P2IFrameMode==false)
		{
			Player2Health--;
			SetPlayerInvincible(1);

			GetWorldTimerManager().ClearTimer(P2IFrameHandle);
			GetWorldTimerManager().SetTimer(P2IFrameHandle, this, &ARunnerGameModeBase::P2EnableDamageTaking, IFrameTime, false);
		
			switch (Player2Health)
			{
			case 2:
				RunWidget->RemoveHealth(RunWidget->P2Health3Img);
				break;
			case 1:
				RunWidget->RemoveHealth(RunWidget->P2Health2Img);
				break;
			case 0:
				RunWidget->RemoveHealth(RunWidget->P2Health1Img);
				break;
			}
	
			if(Player2Health<=0)
			{
				P2Dead=true;

				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("visibility to false for P2"));
				Player2Mesh->SetVisibility(false);
				Player2->SetActorEnableCollision(false);
				
				GetWorldTimerManager().ClearTimer(RespawnHandleP2);
				GetWorldTimerManager().SetTimer(RespawnHandleP2, this, &ARunnerGameModeBase::RespawnPlayer2, RespawnTime, false);
				TryEndRun();
			}
		}
	}
}

void ARunnerGameModeBase::TryEndRun()
{
	if(P1Dead&&P2Dead)
	{
		GameOver=true;

		RunWidget->RemoveFromParent();
	
		EndScreenWidget = CreateWidget<UEndScreenHUD>(GetWorld()->GetFirstPlayerController(), EndScreenInterface);
		EndScreenWidget->AddToViewport(9999);

		EndScreenWidget->InitializeEndScreen();
		EndScreenWidget->RegisterScore();

		LoadGame();
		SaveGame();
	
		EndScreenWidget->DisplayHighscores(ScoreboardItems);
	}
}

void ARunnerGameModeBase::P1EnableDamageTaking()
{
	P1IFrameMode=false;

	Player1Mesh->SetMaterial(0, BaseMat1);
	Player1Mesh->SetMaterial(1, BaseMat2);
	Player1Mesh->SetMaterial(2, BaseMat3);
}

void ARunnerGameModeBase::P2EnableDamageTaking()
{
	P2IFrameMode=false;

	Player2Mesh->SetMaterial(0, PinkBaseMat1);
	Player2Mesh->SetMaterial(1, PinkBaseMat2);
	Player2Mesh->SetMaterial(2, PinkBaseMat3);
}

void ARunnerGameModeBase::SetPlayerInvincible(int PlayerIndex)
{
	if(PlayerIndex == 0)
	{
		P1IFrameMode=true;

		Player1Mesh->SetMaterial(0, IMaterial);
		Player1Mesh->SetMaterial(1, IMaterial);
		Player1Mesh->SetMaterial(2, IMaterial);
	}

	else
	{
		P2IFrameMode=true;

		Player2Mesh->SetMaterial(0, IMaterial);
		Player2Mesh->SetMaterial(1, IMaterial);
		Player2Mesh->SetMaterial(2, IMaterial);
	}
}

void ARunnerGameModeBase::RespawnPlayer1()
{
	Player1Health = 3;
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("visibility to true for P1"));
	Player1Mesh->SetVisibility(true);
	
	Player1->SetActorLocation(FVector(0,-250,100), false);
	Player1Lane = 2;

	P1Dead = false;
	
	SetPlayerInvincible(0);

	GetWorldTimerManager().ClearTimer(P1IFrameHandle);
	GetWorldTimerManager().SetTimer(P1IFrameHandle, this, &ARunnerGameModeBase::P1EnableDamageTaking, IFrameTime, false);
	
	RunWidget->RespawnHealthImages(0);
	Player1->SetActorEnableCollision(true);
}

void ARunnerGameModeBase::RespawnPlayer2()
{
	Player2Health = 3;
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("visibility to true for P2"));
	Player2Mesh->SetVisibility(true);
	
	Player2->SetActorLocation(FVector(0,250,100), false);
	Player2Lane = 2;

	P2Dead = false;
	
	SetPlayerInvincible(1);	

	GetWorldTimerManager().ClearTimer(P2IFrameHandle);
	GetWorldTimerManager().SetTimer(P2IFrameHandle, this, &ARunnerGameModeBase::P2EnableDamageTaking, IFrameTime, false);
	
	RunWidget->RespawnHealthImages(1);
	Player2->SetActorEnableCollision(true);
}

void ARunnerGameModeBase::ChangeMultiplier(float NewMultiplier)
{
	NewMultiplier*=10;
	NewMultiplier = FMath::CeilToInt(NewMultiplier);
	NewMultiplier/=10;
	
	ScoreMultiplier = NewMultiplier;
	RunWidget->UpdateMultiplierTxt(NewMultiplier);
}

void ARunnerGameModeBase::OnTileDestroy()
{
	if(canGenerateTile)
	{
		AddTile();	
		canGenerateTile = false;
	}
	else
	{
		canGenerateTile = true;
	}
}

void ARunnerGameModeBase::P1Dodge()
{
	if(FMath::RandRange(1, 4) > 3)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObstacle::StaticClass(), FoundActors);
		
		int IndexPicker = FMath::RandRange(0, FoundActors.Num()-1);

		Cast<AObstacle>(FoundActors[IndexPicker])->LuckyDestroy();
		
		RunWidget->PlayLuckyAnimation(0);
	}
}

void ARunnerGameModeBase::P2Dodge()
{
	if(FMath::RandRange(1, 4) > 3)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObstacle::StaticClass(), FoundActors);
		
		int IndexPicker = FMath::RandRange(0, FoundActors.Num()-1);

		Cast<AObstacle>(FoundActors[IndexPicker])->LuckyDestroy();
		
		RunWidget->PlayLuckyAnimation(1);
	}
}

void ARunnerGameModeBase::OnGameStart()
{
	CreateInitialTiles();
	
	MainMenuWidget->RemoveFromParent();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	
	RunWidget = CreateWidget<URunGameHUD>(GetWorld()->GetFirstPlayerController(), UserInterface);
	RunWidget->AddToViewport(9999);
	
	GameOver = false;
}



