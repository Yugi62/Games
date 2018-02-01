#include "GameProject.h"
#include "GunMan.h"

#include "PlayerCharacter.h"
#include "SmallBullet.h"

AGunMan::AGunMan()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleRadius(20.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(30.0f);

	Sight->SetBoxExtent(FVector(1000.0f, 1.0f, 400.0f));

	IdleAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/GunMan_IDle.GunMan_IDLE'")).Get();
	RunAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/GunMan_RUN.GunMan_RUN'")).Get();

	FlipbookComponent->SetFlipbook(IdleAnimation);
	FlipbookComponent->SetRelativeLocation(FVector(-3.3f, 0.0f, 0.0f));
}

void AGunMan::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (OverlapCharacter)
	{
		if (OverlapCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));

			if (OverlapCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X <= 500)
			{
				AddMovementInput(FVector(0.0f, 0.0f, 0.0f), 0.0f);
				FlipbookComponent->SetFlipbook(IdleAnimation);
				SetFire();
			}
			else
			{
				AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 0.5f);
				FlipbookComponent->SetFlipbook(RunAnimation);
				GetWorldTimerManager().ClearTimer(FireHandle);
			}
	
		}
		else
		{
			RootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

			if (RootComponent->GetComponentLocation().X - OverlapCharacter->GetActorLocation().X <= 500)
			{
				AddMovementInput(FVector(0.0f, 0.0f, 0.0f), 0.0f);
				FlipbookComponent->SetFlipbook(IdleAnimation);
				SetFire();
			}
			else
			{
				AddMovementInput(FVector(-1.0f, 0.0f, 0.0f), 0.5f);
				FlipbookComponent->SetFlipbook(RunAnimation);
				GetWorldTimerManager().ClearTimer(FireHandle);
			}

		}
	}

}

void AGunMan::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
			OverlapCharacter = OtherActor;
	}
}

void AGunMan::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		OverlapCharacter = nullptr;
		FlipbookComponent->SetFlipbook(IdleAnimation);
		GetWorldTimerManager().ClearTimer(FireHandle);
	}
}

void AGunMan::SetFire()
{
	if(!GetWorldTimerManager().IsTimerActive(FireHandle))
	GetWorldTimerManager().SetTimer(FireHandle, this, &AGunMan::Fire, 1.0f, true);
}

void AGunMan::Fire()
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = FlipbookComponent->GetSocketLocation("Bullet");
		FRotator SpawnRotation = FlipbookComponent->GetSocketRotation("Bullet");

		ASmallBullet* Projectile = GetWorld()->SpawnActor<ASmallBullet>(SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			FVector	PlayerLocation = OverlapCharacter->GetActorLocation();

			FVector Result = PlayerLocation - SpawnLocation;
			Result.Normalize();

			Projectile->FireDirection(Result);
		}
	}
}


