// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/TriggerVolume.h"
#include "OpenTheGates.h"

// Sets default values for this component's properties
UOpenTheGates::UOpenTheGates()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenTheGates::BeginPlay()
{
	Super::BeginPlay();

	Door = GetOwner();
	
	InitialPosition = Door->GetActorLocation();
	CurrentPosition = InitialPosition;
	TargetCloseLift = InitialPosition.Z;
	if (!StatueTrigger) {
		UE_LOG(LogTemp, Error, TEXT("Trigger Volume Not Set."));
	}
}


// Called every frame
void UOpenTheGates::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetActorMass() >= 40.f) {
		OpenGate(DeltaTime);
	}
	else {
		CloseGate(DeltaTime);
	}
}

void UOpenTheGates::OpenGate(float DeltaTime) {
	CurrentPosition.Z = FMath::Lerp(CurrentPosition.Z, InitialPosition.Z + TargetOpenLift, LiftSpeed * DeltaTime);

	Door->SetActorLocation(CurrentPosition);
}

void UOpenTheGates::CloseGate(float DeltaTime) {
	CurrentPosition.Z = FMath::Lerp(CurrentPosition.Z, InitialPosition.Z, 5 * LiftSpeed * DeltaTime);

	Door->SetActorLocation(CurrentPosition);
}

float UOpenTheGates::GetActorMass() {
	float Mass = 0.f;
	UE_LOG(LogTemp, Warning, TEXT("Mass = %d"), Mass);
	TArray<AActor*> OverlappingActors;

	StatueTrigger->GetOverlappingActors(OUT OverlappingActors);

	for (AActor *Actor : OverlappingActors) {
		Mass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return Mass;
}