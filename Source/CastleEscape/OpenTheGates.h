// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenTheGates.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CASTLEESCAPE_API UOpenTheGates : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenTheGates();

public:
	void OpenGate(float DeltaTime);
	void CloseGate(float DeltaTime);
	float GetActorMass();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* Door = NULL;

	FVector InitialPosition;
	FVector CurrentPosition;

	UPROPERTY(EditAnywhere)
		float LiftSpeed = 2.f;

	float CurrentLift;
	UPROPERTY(EditAnywhere)
		float TargetOpenLift = 400.f;

	float TargetCloseLift;
	UPROPERTY(EditAnywhere)
		class ATriggerVolume* StatueTrigger = NULL;
	bool Yes = true;
};
