#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DreamCar.generated.h"

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

UCLASS()
class GAME_API ADreamCar : public APawn
{
	GENERATED_BODY()

public:
	ADreamCar();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocation(float Value);
	void UpdateRotation(float Value, float InSteering);

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FMoveState Move);

	void SimulateMove(const FMoveState& LastMove);
	FMoveState CreateMove(float DeltaTime);
	void ClearAcknowledgeMoves(FMoveState Move);

private:
	UPROPERTY(EditAnywhere)
		float Mass = 1000;

	UPROPERTY(EditAnywhere)
		float MaxForce = 10000;

	UPROPERTY(EditAnywhere)
		float TurningRadius = 10;

	UPROPERTY(EditAnywhere)
		float MaxDegreePerSecond = 90;
	
	UPROPERTY(EditAnywhere)
		float DragCoefficient = 16.f;
	// 왜 16?
	//AirResistance = (invdir *) Speed ^ 2 * DragCoefficient
	//-> DragCoefficient = AirResistance / Speed ^ 2

	UPROPERTY(EditAnywhere)
		float RollingCoefficient = 0.015f;

private:
	UPROPERTY(ReplicatedUsing = "OnRep_ServerState")
		FServerState ServerState;
	UFUNCTION()	void OnRep_ServerState();

private:
	FVector Velocity;
	
	float Throttle;
	float Steering;

	TArray<FMoveState> UnacknowledgeMoves;

	//UPROPERTY(ReplicatedUsing = "OnRep_ReplicatedTransform") // 패킷을 타는 변수
	//	FTransform ReplicatedTransform;
};
