#include "GameProject.h"
#include "SpawnItem.h"

#include "Excaliber.h"
#include "GoldenHelmet.h"
#include "GoldenSuit.h"
#include "IronHelmet.h"
#include "IronSuit.h"
#include "ShortSword.h"
#include "Claymore.h"

ASpawnItem::ASpawnItem()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;

	SpawnGoldenHelmet = false;
	SpawnGoldenSuit = false;
	SpawnIronHelmet = false;
	SpawnIronSuit = false;
	SpawnClaymore = false;
	SpawnExcaliber = false;
}

void ASpawnItem::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FVector SpawnLocation = BoxComponent->GetComponentLocation();
	FRotator SpawnRotation = BoxComponent->GetComponentRotation();

	if (SpawnGoldenHelmet)
	{
		AGoldenHelmet* Item = GetWorld()->SpawnActor<AGoldenHelmet>(SpawnLocation, SpawnRotation, SpawnParams);
		Item->GetCapsuleComponent()->SetSimulatePhysics(true);
	}

	if (SpawnGoldenSuit)
	{
		AGoldenSuit* Item = GetWorld()->SpawnActor<AGoldenSuit>(SpawnLocation, SpawnRotation, SpawnParams);
		Item->GetCapsuleComponent()->SetSimulatePhysics(true);
	}

	if (SpawnIronHelmet)
	{
		AIronHelmet* Item = GetWorld()->SpawnActor<AIronHelmet>(SpawnLocation, SpawnRotation, SpawnParams);
		Item->GetCapsuleComponent()->SetSimulatePhysics(true);
	}

	if (SpawnIronSuit)
	{
		AIronSuit* Item = GetWorld()->SpawnActor<AIronSuit>(SpawnLocation, SpawnRotation, SpawnParams);
		Item->GetCapsuleComponent()->SetSimulatePhysics(true);
	}

	if (SpawnClaymore)
	{
		AClaymore* Item = GetWorld()->SpawnActor<AClaymore>(SpawnLocation, SpawnRotation, SpawnParams);
		Item->GetCapsuleComponent()->SetSimulatePhysics(true);
	}

	if (SpawnExcaliber)
	{
		AExcaliber* Item = GetWorld()->SpawnActor<AExcaliber>(SpawnLocation, SpawnRotation, SpawnParams);
		Item->GetCapsuleComponent()->SetSimulatePhysics(true);
	}
}
