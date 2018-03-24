// (c) 2018 Joachim Hofer

#include "OpenableDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenableDoor::UOpenableDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenableDoor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UOpenableDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto CurrentTime = GetWorld()->GetTimeSeconds();
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OnOpen.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}
}

float UOpenableDoor::GetTotalMassOfActorsOnPlate() const
{
	TSet<AActor*> OverlappingActors;
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate not initialized!"))
		return 0.f;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	auto TotalMass = 0.f;
	for (const auto* actor : OverlappingActors) {
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Error, TEXT("Mass: %f"), TotalMass)
	return TotalMass;
}
