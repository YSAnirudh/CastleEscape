// Fill out your copyright notice in the Description page of Project Settings.


#include "GetDown.h"
#define OUT

// Sets default values for this component's properties
UGetDown::UGetDown()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGetDown::BeginPlay()
{
	Super::BeginPlay();
	Platform = GetOwner();
	InitialPosition = Platform->GetActorLocation();
	CurrentPosition = InitialPosition;

	if (!PlatformTrigger) {
		UE_LOG(LogTemp, Error, TEXT("Platform Trigger Not Set"));
	}
}


// Called every frame
void UGetDown::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (ifOverlap())
		GetDown = true;

	if (GetDown) {
		GoDown(DeltaTime);
	}
}

bool UGetDown::ifOverlap() {
	TArray<AActor*> OverlappingActors;

	PlatformTrigger->GetOverlappingActors(OUT OverlappingActors);

	if (OverlappingActors.Num() == 0) {
		return false;
	}

	return true;
}

void UGetDown::GoDown(float DeltaTime) {
	CurrentPosition.Z = FMath::Lerp(CurrentPosition.Z, InitialPosition.Z - GetDownHeight, GetDownSpeed * DeltaTime);

	Platform->SetActorLocation(CurrentPosition);
}
