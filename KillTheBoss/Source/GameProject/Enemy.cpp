// Fill out your copyright notice in the Description page of Project Settings.

#include "GameProject.h"
#include "Enemy.h"

#include "PlayerCharacter.h"
#include "GameProjectPlayerController.h"

#include "GoldenHelmet.h"
#include "GoldenSuit.h"
#include "Excaliber.h"
#include "IronHelmet.h"
#include "IronSuit.h"
#include "Claymore.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OverlapPawn);

	Sight = CreateDefaultSubobject<UBoxComponent>(TEXT("Sight"));
	Sight->SetCollisionProfileName(TEXT("EnemySight"));
	Sight->SetupAttachment(GetCapsuleComponent());

	Sight->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OverlapStart);
	Sight->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OverlapStop);

	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	FlipbookComponent->SetupAttachment(GetCapsuleComponent());
	FlipbookComponent->SetCollisionProfileName(TEXT("Sprite"));

	UMaterialInterface* Material = ConstructorHelpers::FObjectFinderOptional<UMaterialInterface>(TEXT("MaterialInstanceConstant'/Paper2D/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial'")).Get();
	FlipbookComponent->SetMaterial(0, Material);

	HP = 0;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

	AudioComponent->bAutoActivate = false;

	Damaged_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/170149_timgormly_8-bit-hurt_Cue.170149_timgormly_8-bit-hurt_Cue'")).Get();
}

void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AEnemy::OverlapPawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(AWeapon::StaticClass()))
		{
			AWeapon* Weapon = Cast<AWeapon>(OtherActor);

			if (RootComponent->GetComponentLocation().X - Weapon->GetActorLocation().X >= 0)
			{
				this->LaunchCharacter(FVector(1.0f, 0.0f, 1.0f) * 150, false, false);
			}
			else
			{
				this->LaunchCharacter(FVector(-1.0f, 0.0f, 1.0f) * 150, false, false);
			}

			if(!GetWorldTimerManager().IsTimerActive(TimerHandle))
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemy::SetTimer, 0.1f, true);

			HP -= Weapon->Damage;
			Weapon->GetCapsuleComponent()->SetCollisionProfileName(TEXT("HoldWeapon"));

			AudioComponent->SetSound(Damaged_Cue);
			AudioComponent->Play();


			if(HP <= 0)
			{
				GetCapsuleComponent()->SetCollisionProfileName(TEXT("HoldingWeapon"));
				if(Sight)
				Sight->DestroyComponent();

				if (!GetWorldTimerManager().IsTimerActive(DeathHandle))
					GetWorldTimerManager().SetTimer(DeathHandle, this, &AEnemy::DeathTimer, 0.1f, true);
			}
	
			Weapon->Durability--;
		
			if(Weapon->Durability <= 0)
			Weapon->DestroyWeapon();
		}

		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
		{
			APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);

			if (Character->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
			{
				Character->LaunchCharacter(FVector(25.0f, 0.0f, 5.0f) * 100, false, false);
			}
			else
			{
				Character->LaunchCharacter(FVector(-25.0f, 0.0f, 5.0f) * 100, false, false);
			}

			Character->StartInvincibleTimer();
		}
	}

}

void AEnemy::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AEnemy::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AEnemy::DropItem()
{
	FRandomStream Rand;
	Rand.GenerateNewSeed();

	if (Rand.RandRange(0, 10000) < Rare_Drop * 100)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = this->GetActorLocation();
		FRotator SpawnRotation = this->GetActorRotation();

		int32 Integer = Rand.RandRange(0, 2);

		if (Integer == 0)
		{
			AGoldenSuit* PickuptoSpawn = GetWorld()->SpawnActor<AGoldenSuit>(SpawnLocation, SpawnRotation, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);
		}
		else if (Integer == 1)
		{
			AGoldenHelmet* PickuptoSpawn = GetWorld()->SpawnActor<AGoldenHelmet>(SpawnLocation, SpawnRotation, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);
		}
		else if (Integer == 2)
		{
			AExcaliber* PickuptoSpawn = GetWorld()->SpawnActor<AExcaliber>(SpawnLocation, SpawnRotation, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);
		}

	}
	else if (Rand.RandRange(0, 10000) < Armor_Drop * 100)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = this->GetActorLocation();
		FRotator SpawnRotation = this->GetActorRotation();

		int32 Integer = Rand.RandRange(0, 3);

		if (Integer == 0)
		{
			AIronHelmet* PickuptoSpawn = GetWorld()->SpawnActor<AIronHelmet>(SpawnLocation, SpawnRotation, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);
		}
		else if (Integer == 1)
		{
			AIronSuit* PickuptoSpawn = GetWorld()->SpawnActor<AIronSuit>(SpawnLocation, SpawnRotation, SpawnParams);
			PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);
		}
	}
	else if (Rand.RandRange(0, 10000) < Weapon_Drop * 100)
	{
		FActorSpawnParameters SpawnParams;
		FVector SpawnLocation = this->GetActorLocation();
		FRotator SpawnRotation = this->GetActorRotation();

		AClaymore* PickuptoSpawn = GetWorld()->SpawnActor<AClaymore>(SpawnLocation, SpawnRotation, SpawnParams);
		PickuptoSpawn->GetCapsuleComponent()->SetSimulatePhysics(true);
	}
}

void AEnemy::SetTimer()
{
	EndTimer++;
	
	if (EndTimer % 2 == 0)
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
	else
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

	if (EndTimer >= 4)
	{
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
		GetWorldTimerManager().ClearTimer(TimerHandle);
		EndTimer = 0;
	}
}

void AEnemy::DeathTimer()
{
	DeathCount++;

	if (DeathCount % 2 == 0)
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
	else
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

	if (DeathCount >= 4)
	{
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
		GetWorldTimerManager().ClearTimer(DeathHandle);
		DeathCount = 0;

		this->Destroy();
		DropItem();
	}
}