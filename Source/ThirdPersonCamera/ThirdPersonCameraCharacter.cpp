#include "ThirdPersonCameraCharacter.h"
#include "TPCCharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/HoatCameraSpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacter, Log, All);

AThirdPersonCameraCharacter::AThirdPersonCameraCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTPCCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<UHoatCameraSpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

AHoatCameraModificationVolume* AThirdPersonCameraCharacter::GetCurrentCameraModificationVolume() const
{
	return CurrentCameraModificationVolume;
}

void AThirdPersonCameraCharacter::SetCurrentCameraModificationVolume(AHoatCameraModificationVolume* InCurrentCameraModificationVolume)
{
	CurrentCameraModificationVolume = InCurrentCameraModificationVolume;
}

bool AThirdPersonCameraCharacter::GotMovementInput() const
{
	return !GetPendingMovementInputVector().IsZero();
}

USpringArmComponent* AThirdPersonCameraCharacter::GetCameraBoom() const
{
	return CameraBoom;
}

UCameraComponent* AThirdPersonCameraCharacter::GetFollowCamera() const
{
	return FollowCamera;
}

void AThirdPersonCameraCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AThirdPersonCameraCharacter, bIsSprinting, COND_SimulatedOnly);
}

void AThirdPersonCameraCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonCameraCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonCameraCharacter::Look);

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AThirdPersonCameraCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AThirdPersonCameraCharacter::StopSprinting);

		//Crouching (cannot bind ACharacter::Crounch/UnCrouch directly has they take a parameter)
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AThirdPersonCameraCharacter::RequestCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AThirdPersonCameraCharacter::RequestUnCrouch);

		// Set up input mappings
		check(Controller);
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Clear out existing mapping, and add our mapping
				Subsystem->ClearAllMappings();
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void AThirdPersonCameraCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MoveValue = Value.Get<FVector2D>();
		const FRotator MovementRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Forward/Backward direction
		if (MoveValue.Y != 0.f)
		{
			// Get forward vector
			const FVector Direction = MovementRotation.RotateVector(FVector::ForwardVector);

			AddMovementInput(Direction, MoveValue.Y);
		}

		// Right/Left direction
		if (MoveValue.X != 0.f)
		{
			// Get right vector
			const FVector Direction = MovementRotation.RotateVector(FVector::RightVector);

			AddMovementInput(Direction, MoveValue.X);
		}
	}
}

void AThirdPersonCameraCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookValue = Value.Get<FVector2D>();

		if (LookValue.X != 0.f)
		{
			AddControllerYawInput(LookValue.X);
		}

		if (LookValue.Y != 0.f)
		{
			AddControllerPitchInput(LookValue.Y);
		}
	}
}

bool AThirdPersonCameraCharacter::CanSprint() const
{
	if (UTPCCharacterMovementComponent* TPCCharacterMovement = Cast<UTPCCharacterMovementComponent>(GetCharacterMovement()))
	{
		return !bIsSprinting && TPCCharacterMovement && TPCCharacterMovement->CanEverSprint() && GetRootComponent() && !GetRootComponent()->IsSimulatingPhysics();
	}

	return false;
}

void AThirdPersonCameraCharacter::OnRep_IsSprinting()
{
	if (UTPCCharacterMovementComponent* TPCCharacterMovement = Cast<UTPCCharacterMovementComponent>(GetCharacterMovement()))
	{
		if (bIsSprinting)
		{
			TPCCharacterMovement->bWantsToSprint = true;
		}
		else
		{
			TPCCharacterMovement->bWantsToSprint = false;
		}

		TPCCharacterMovement->bNetworkUpdateReceived = true;
	}
}

void AThirdPersonCameraCharacter::Sprint()
{
	if (UTPCCharacterMovementComponent* TPCCharacterMovement = Cast<UTPCCharacterMovementComponent>(GetCharacterMovement()))
	{
		if (CanSprint())
		{
			TPCCharacterMovement->bWantsToSprint = true;
		}
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		else if (!TPCCharacterMovement->CanEverSprint())
		{
			UE_LOG(LogCharacter, Log, TEXT("%s is trying to sprint, but sprinting is disabled on this character! (check TPCCharacterMovementComponent bCanSprint)"), *GetName());
		}
#endif
	}
}

void AThirdPersonCameraCharacter::StopSprinting()
{
	if (UTPCCharacterMovementComponent* TPCCharacterMovement = Cast<UTPCCharacterMovementComponent>(GetCharacterMovement()))
	{
		TPCCharacterMovement->bWantsToSprint = false;
	}
}

void AThirdPersonCameraCharacter::RequestCrouch()
{
	Crouch();
}

void AThirdPersonCameraCharacter::RequestUnCrouch()
{
	UnCrouch();
}
