// Copyright Epic Games, Inc. All Rights Reserved.

#include "FLoatingISlandRPGCharacter.h"
#include "FLoatingISlandRPGProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory/InventoryWidget.h"


//////////////////////////////////////////////////////////////////////////
// AFLoatingISlandRPGCharacter

AFLoatingISlandRPGCharacter::AFLoatingISlandRPGCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	GrappleHookComponent = CreateDefaultSubobject<UGrappleComponent>(TEXT("CharactersGrappleHook"));
	InvComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("CharacterInventory"));

	LocalOffSet = FVector(-30.0f, -30.0f, 50.0f);

}

void AFLoatingISlandRPGCharacter::BeginPlay()
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
	PlayerOne = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	InventoryUi = CreateWidget<UInventoryWidget>(PlayerOne, InventoryUiClass);
	InventoryUi->SetInvComp(InvComponent);
}

//////////////////////////////////////////////////////////////////////////// Input

void AFLoatingISlandRPGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFLoatingISlandRPGCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFLoatingISlandRPGCharacter::Look);

		//GrappleHook
		EnhancedInputComponent->BindAction(GrappleAction, ETriggerEvent::Completed, this, &AFLoatingISlandRPGCharacter::FireGrapplingHook);

		//Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &AFLoatingISlandRPGCharacter::ToggleInventory);
	}
}

void AFLoatingISlandRPGCharacter::FireGrapplingHook()
{
	if (GrappleHookComponent->IsInUse())
	{
		GrappleHookComponent->CancelGrappleHook();
	}
	else
	{
		FHitResult Hit; // the variable we store the hit result in
		FVector Start = FirstPersonCameraComponent->GetComponentLocation();
		FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * 1000.0f; //shoot the line way off into the distance
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // ignore outrselves
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Pawn, QueryParams))
		{
			GrappleHookComponent->FireGrappleHook(Hit.Location, LocalOffSet);
		}
		else
		{
			GrappleHookComponent->FireGrappleHook(Hit.TraceEnd, LocalOffSet);
		}

	}
}


void AFLoatingISlandRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFLoatingISlandRPGCharacter::Look(const FInputActionValue& Value)
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

void AFLoatingISlandRPGCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AFLoatingISlandRPGCharacter::GetHasRifle()
{
	return bHasRifle;
}

void AFLoatingISlandRPGCharacter::ToggleInventory()
{
	InvComponent->SetIsDirty(true);
	if (bIsInvOpen == false)
	{
		InventoryUi->AddToViewport();
		PlayerOne->SetInputMode(FInputModeGameAndUI());
		PlayerOne->SetShowMouseCursor(true);
		bIsInvOpen = true;
	}
	else if (bIsInvOpen == true)
	{
		InventoryUi->RemoveFromParent();
		PlayerOne->SetInputMode(FInputModeGameOnly());
		PlayerOne->SetShowMouseCursor(false);
		bIsInvOpen = false;
	}
}
