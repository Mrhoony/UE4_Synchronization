#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

USTRUCT()
struct FMoveState // Autonomous -> Authority
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		float Throttle;

	UPROPERTY()
		float Steering;

	UPROPERTY()
		float DeltaTime;

	UPROPERTY()
		float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SimulateMove(const FMoveState& Move);

	FORCEINLINE FVector GetVelocity() { return Velocity; }
	FORCEINLINE void SetVelocity(FVector Val) { Velocity = Val; }

	FORCEINLINE void SetSteering(float Val) { Steering = Val; }
	FORCEINLINE void SetThrottle(float Val) { Throttle = Val; }

	FORCEINLINE FMoveState GetLastMove() { return LastMove; }

private:
	FVector GetAirResistance();
	FVector GetRollingResistance();

	FMoveState CreateMove(float DeltaTime);
	void UpdateLocation(float DeltaTime);
	void UpdateRotation(float DeltaTime, float InSteering);

private:
	UPROPERTY(EditAnywhere)	float Mass = 1000;
	UPROPERTY(EditAnywhere)	float MaxForce = 10000;
	UPROPERTY(EditAnywhere)	float TurningRadius = 10;
	UPROPERTY(EditAnywhere)	float DragCoefficient = 16.f;
	// 왜 16?
	//AirResistance = (invdir *) Speed ^ 2 * DragCoefficient
	//-> DragCoefficient = AirResistance / Speed ^ 2
	UPROPERTY(EditAnywhere)	float RollingCoefficient = 0.015f;

private:
	FVector Velocity;

	float Throttle;
	float Steering;

	FMoveState LastMove;

	//UPROPERTY(ReplicatedUsing = "OnRep_ReplicatedTransform") // 패킷을 타는 변수
	//	FTransform ReplicatedTransform;
};
