#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPCCharacterMovementComponent.generated.h"

/**
 * Custom movement component that supports sprinting without client jittering
 */
UCLASS()
class THIRDPERSONCAMERA_API UTPCCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UTPCCharacterMovementComponent();

	/** Returns maximum speed of component in current movement mode. */
	virtual float GetMaxSpeed() const override;

	/** Update the character state in PerformMovement right before doing the actual position change */
	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	/** Unpack compressed flags from a saved move and set state accordingly. See FSavedMove_Character. */
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	/** Get prediction data for a client game. Should not be used if not running as a client. Allocates the data on demand and can be overridden to allocate a custom override if desired. Result must be a FNetworkPredictionData_Client_Character. */
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	/** Returns true if component can sprint */
	FORCEINLINE bool CanEverSprint() const;

	/** Returns true if currently sprinting */ 
	UFUNCTION(BlueprintCallable, Category = "AI|Components|NavMovement")
	bool IsSprinting() const;

	/** If true, try to sprint (or keep sprinting) on next update. If false, try to stop sprinting on next update. */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Character Movement (General Settings)")
	uint8 bWantsToSprint{ 0 };

private:
	/** If true, this component is capable of sprinting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = NavMovement, meta = (AllowPrivateAccess = "true"))
	bool bCanSprint{ true };

	/** The maximum ground speed when sprinting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Walking", meta =
		(AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float MaxWalkSpeedSprinting{ 800.f };

	/** The maximum ground speed when crouched and sprinting. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Movement: Walking", meta =
		(AllowPrivateAccess = "true", ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float MaxWalkSpeedCrouchedSprinting{ 400.f };
};

class FTPCSavedMove_Character : public FSavedMove_Character
{
public:
	typedef FSavedMove_Character Super;

	/** Clear saved move properties, so it can be re-used. */
	virtual void Clear() override;

	/** Returns true if this move can be combined with NewMove for replication without changing any behavior */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

	/** Called to set up this saved move (when initially created) to make a predictive correction. */
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData);

	/** Called before ClientUpdatePosition uses this SavedMove to make a predictive correction	 */
	virtual void PrepMoveFor(ACharacter* C) override;

	/** Returns a byte containing encoded special movement information (jumping, crouching, etc.)	 */
	virtual uint8 GetCompressedFlags() const override;

	uint8 bSavedWantsToSprint{ 0 };
};

class FTPCNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	FTPCNetworkPredictionData_Client_Character(const UCharacterMovementComponent& CharacterMovementComponent);

	/** Allocate a new saved move. Subclasses should override this if they want to use a custom move class. */
	virtual FSavedMovePtr AllocateNewMove() override;
};
