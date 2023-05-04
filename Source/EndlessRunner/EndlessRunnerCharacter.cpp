// Copyright Epic Games, Inc. All Rights Reserved.

#include "EndlessRunnerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RunnerGameModeBase.h"


//////////////////////////////////////////////////////////////////////////
// AEndlessRunnerCharacter

AEndlessRunnerCharacter::AEndlessRunnerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	RunGameMode =  Cast<ARunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AEndlessRunnerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AEndlessRunnerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		if(shouldBindInput)
		{
			//Jumping
            		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::Jump);
            		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEndlessRunnerCharacter::StopJumping);
            
            		//Moving
            		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::Move);
            
            		//Looking
            		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEndlessRunnerCharacter::Look);
            
            		//PLAYER 2
            		EnhancedInputComponent->BindAction(ArrowMoveAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::P2Move);
			
            		EnhancedInputComponent->BindAction(ArrowJumpAction, ETriggerEvent::Started, this, &AEndlessRunnerCharacter::P2Jump);
					EnhancedInputComponent->BindAction(ArrowJumpAction, ETriggerEvent::Completed, this, &AEndlessRunnerCharacter::P2StopJumping);
		}
		
	}

}

void AEndlessRunnerCharacter::P2Move(const FInputActionValue& Value)
{
	if(RunGameMode->P2Dead)
	{
		return;
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	if(Controller != nullptr)
	{
		if(MovementVector.X > 0)
		{
			if(RunGameMode->Player2Lane > 2){return;}
			RunGameMode->Player2Lane++;
			RunGameMode->Player2->AddActorLocalOffset(FVector(0,135.f,0));
		}
		else
		{
			if(RunGameMode->Player2Lane < 2){return;}
			RunGameMode->Player2Lane--;
			RunGameMode->Player2->AddActorLocalOffset(FVector(0,-135.f,0));
		}
	}
}

void AEndlessRunnerCharacter::P2Jump()
{
	if(RunGameMode->P2Dead)
	{
		return;
	}
	
	RunGameMode->Player2->Super::Jump();
}

void AEndlessRunnerCharacter::P2StopJumping()
{
	if(RunGameMode->P2Dead)
	{
		return;
	}
	
	RunGameMode->Player2->Super::StopJumping();
}

void AEndlessRunnerCharacter::Jump()
{
	if(RunGameMode->P1Dead)
	{
		return;
	}
	
	Super::Jump();
}

void AEndlessRunnerCharacter::StopJumping()
{
	if(RunGameMode->P1Dead)
	{
		return;
	}
	
	Super::StopJumping();
}

void AEndlessRunnerCharacter::Move(const FInputActionValue& Value)
{
	if(RunGameMode->P1Dead)
	{
		return;
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if(MovementVector.X > 0)
	{
		if(RunGameMode->Player1Lane > 2){return;}
		RunGameMode->Player1Lane++;
		RunGameMode->Player1->AddActorLocalOffset(FVector(0,135.f,0));
	}
	else
	{
		if(RunGameMode->Player1Lane < 2){return;}
		RunGameMode->Player1Lane--;
		RunGameMode->Player1->AddActorLocalOffset(FVector(0,-135.f,0));
	}
}

void AEndlessRunnerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}




