#include "TPCCharacterMovementComponent.h"
#include "ThirdPersonCameraCharacter.h"

UTPCCharacterMovementComponent::UTPCCharacterMovementComponent()
{
	// Configure character movement
	bOrientRotationToMovement = true; // Character moves in the direction of input...
	RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	NavAgentProps.bCanCrouch = true; // Allow the Character to crouch by default

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	JumpZVelocity = 550.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 500.f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
}

float UTPCCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking || MovementMode == MOVE_NavWalking)
	{
		if (IsSprinting())
		{
			return IsCrouching() ? MaxWalkSpeedCrouchedSprinting : MaxWalkSpeedSprinting;
		}
	}

	return Super::GetMaxSpeed();
}

void UTPCCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);

	// Proxies get replicated sprint state.
	if (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)
	{
		if (AThirdPersonCameraCharacter* TPCCharacter = Cast<AThirdPersonCameraCharacter>(CharacterOwner))
		{
			// Check for a change in sprint state. Players toggle sprint by changing bWantsToSprint.
			const bool bIsSprinting = TPCCharacter->bIsSprinting;
			if (bIsSprinting && !bWantsToSprint)
			{
				TPCCharacter->bIsSprinting = false;
			}
			else if (!bIsSprinting && bWantsToSprint)
			{
				TPCCharacter->bIsSprinting = true;
			}
		}
	}
}

void UTPCCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

FNetworkPredictionData_Client* UTPCCharacterMovementComponent::GetPredictionData_Client() const
{
	if (!ClientPredictionData)
	{
		if (UTPCCharacterMovementComponent* TempComponent = const_cast<UTPCCharacterMovementComponent*>(this))
		{
			TempComponent->ClientPredictionData = new FTPCNetworkPredictionData_Client_Character(*this);
		}
	}

	return ClientPredictionData;
}

bool UTPCCharacterMovementComponent::CanEverSprint() const
{
	return bCanSprint;
}

bool UTPCCharacterMovementComponent::IsSprinting() const
{
	if (AThirdPersonCameraCharacter* TPCCharacter = Cast<AThirdPersonCameraCharacter>(CharacterOwner))
	{
		return TPCCharacter->bIsSprinting;
	}

	return false;
}

void FTPCSavedMove_Character::Clear()
{
	Super::Clear();
	bSavedWantsToSprint = false;
}

bool FTPCSavedMove_Character::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	if (bSavedWantsToSprint != ((FTPCSavedMove_Character*)&NewMove)->bSavedWantsToSprint)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void FTPCSavedMove_Character::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (C)
	{
		if (UTPCCharacterMovementComponent* TPCCharacterMovement = Cast<UTPCCharacterMovementComponent>(C->GetCharacterMovement()))
		{
			bSavedWantsToSprint = TPCCharacterMovement->bWantsToSprint;
		}
	}
}

void FTPCSavedMove_Character::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	if (C)
	{
		if (UTPCCharacterMovementComponent* TPCCharacterMovement = Cast<UTPCCharacterMovementComponent>(C->GetCharacterMovement()))
		{
			TPCCharacterMovement->bWantsToSprint = bSavedWantsToSprint;
		}
	}
}

uint8 FTPCSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}

	return Result;
}

FTPCNetworkPredictionData_Client_Character::FTPCNetworkPredictionData_Client_Character(const UCharacterMovementComponent& CharacterMovementComponent)
	: Super(CharacterMovementComponent)
{
}

FSavedMovePtr FTPCNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FTPCSavedMove_Character());
}
