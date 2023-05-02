// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRunner/Public/RunnerGameModeBase.h"
#include "RunGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ARunnerGameModeBase::BeginPlay()
{
	CreateInitialTiles();
	
	RunWidget = CreateWidget<URunGameHUD>(GetWorld()->GetFirstPlayerController(), UserInterface);
	RunWidget->AddToViewport(9999);

	//Player2 = GetWorld()->SpawnActor<AEndlessRunnerCharacter>(Player2Class, FVector(0, 250, 100), FRotator(0, 0, 0));
	Player2 = Cast<AEndlessRunnerCharacter>(UGameplayStatics::CreatePlayer(GetWorld(), 1, true));
	
	AEndlessRunnerCharacter* Player1 = Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Player1Mesh = Player1->GetMesh();
	Player2Mesh = Cast<AEndlessRunnerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 1))->GetMesh();

	Player2Mesh->SetMaterial(0, PinkBaseMat1);
	Player2Mesh->SetMaterial(1, PinkBaseMat2);
	Player2Mesh->SetMaterial(2, PinkBaseMat3);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->SetActorLocation(FVector(0,-250,100));
	UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetCharacter()->SetActorLocation(FVector(0,250,100));

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter()->Tags.Add(FName("p1"));
	UGameplayStatics::GetPlayerController(GetWorld(), 1)->GetCharacter()->Tags.Add(FName("p2"));
}

void ARunnerGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	Score+=DeltaSeconds*ScoreMultiplier;
	RunWidget->ScoreTxt->SetText(FText::FromString(FString::FromInt((int)Score)));

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
			P1IFrameMode=true;

			Player1Mesh->SetMaterial(0, IMaterial);
			Player1Mesh->SetMaterial(1, IMaterial);
			Player1Mesh->SetMaterial(2, IMaterial);
		
			GetWorldTimerManager().SetTimer(IFrameHandle, this, &ARunnerGameModeBase::P1EnableDamageTaking, IFrameTime, false);
		
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
				EndRun();
			}
		}
	}

	else
	{
		if(P1IFrameMode==false)
		{
			Player2Health--;
			P2IFrameMode=true;

			Player2Mesh->SetMaterial(0, IMaterial);
			Player2Mesh->SetMaterial(1, IMaterial);
			Player2Mesh->SetMaterial(2, IMaterial);
		
			GetWorldTimerManager().SetTimer(IFrameHandle, this, &ARunnerGameModeBase::P2EnableDamageTaking, IFrameTime, false);
		
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
				EndRun();
			}
		}
	}
}

void ARunnerGameModeBase::EndRun()
{
	RunWidget->RemoveFromParent();
	
	EndScreenWidget = CreateWidget<UEndScreenHUD>(GetWorld()->GetFirstPlayerController(), EndScreenInterface);
	EndScreenWidget->AddToViewport(9999);

	EndScreenWidget->InitializeEndScreen();
	EndScreenWidget->RegisterScore();
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



