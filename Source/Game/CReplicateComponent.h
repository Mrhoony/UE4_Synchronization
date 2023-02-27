#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.h"
#include "CReplicateComponent.generated.h"

USTRUCT()
struct FServerState // Authority -> Simulate
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		FTransform Transform;

	UPROPERTY()
		FVector Velocity;

	UPROPERTY()
		FMoveState LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCReplicateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCReplicateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FMoveState Move);
	
private:
	void ClearAcknowledgeMoves(FMoveState LastMove);
	void UpdateServerState(const FMoveState& Move);

	void AutonomousProxy_OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();

	void SimulateProxyTick(float DeltaTime);

private:
	UPROPERTY(ReplicatedUsing = "OnRep_ServerState")
		FServerState ServerState;
	UFUNCTION()	void OnRep_ServerState();

private:
	TArray<FMoveState> UnacknowledgeMoves;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdate;
	FVector ClientStartLocation;

	UCMovementComponent* MovementComponent;
};
