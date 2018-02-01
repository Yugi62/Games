#include "GameProject.h"
#include "KingTeaser.h"

#include "PlayerCharacter.h"
#include "BigBullet.h"
#include "FastBullet.h"
#include "BounceBullet.h"

#define CHASE_DELAY 0.6f
#define LINESHOOT_DELAY 0.2f
#define WAVESHOOT_DELAY 0.1f
#define ROUNDSHOOT_DELAY 0.015f
#define FREEZE_DELAY 6.0f


AKingTeaser::AKingTeaser()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleRadius(120.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(150.0f);

	Sight->DetachFromParent();
	Sight->SetBoxExtent(FVector(1755.0f, 1.0f, 700.0f));
	Sight->SetWorldLocation(FVector(47488.996094f, 0.0f, 2995.554443f));
	Sight->SetCollisionProfileName(TEXT("BossArea"));

	IdleAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/KingTeaser_0006_Idle.KingTeaser_0006_Idle'")).Get();
	RunAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/KingTeaser_0000_Walk.KingTeaser_0000_Walk'")).Get();
	FreezeAnimation = ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook>(TEXT("PaperFlipbook'/Game/Sprite/Enemy/Flipbook/KingTeaser_Layer.KingTeaser_Layer'")).Get();


	FlipbookComponent->SetFlipbook(IdleAnimation);
	FlipbookComponent->SetRelativeLocation(FVector(-2.8f, 0.0f, -30.0f));

	HP = 100.0f;

	DelayCount = 0;
	HitCount = 0;
	DeathCount = 0;

	ActivateBoss = false;
	PatternCount = 0.0f;

	Die_Cue = ConstructorHelpers::FObjectFinderOptional<USoundCue>(TEXT("SoundCue'/Game/Audio/Jump/323093__doty21__robot-scream-5_Cue.323093__doty21__robot-scream-5_Cue'")).Get();
}

void AKingTeaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerCharacter)
	{
		if (PlayerCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
			RootComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
		else
			RootComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

		if (ActivateBoss)
		{
			switch (Pattern)
			{
			case 1:
				Chase();

				if (!GetWorldTimerManager().IsTimerActive(PatternHandle))
					GetWorldTimerManager().SetTimer(PatternHandle, this, &AKingTeaser::Fire, CHASE_DELAY, true);
				break;

			case 2:
				if (!GetWorldTimerManager().IsTimerActive(PatternHandle))
				{
					PlayerLocation = PlayerCharacter->GetActorLocation();
					GetWorldTimerManager().SetTimer(PatternHandle, this, &AKingTeaser::FourLineShoot, LINESHOOT_DELAY, true);
				}
				break;

			case 3:
				if (!GetWorldTimerManager().IsTimerActive(PatternHandle))
					GetWorldTimerManager().SetTimer(PatternHandle, this, &AKingTeaser::WaveShoot, WAVESHOOT_DELAY, true);

				break;

			case 4:
				if (!GetWorldTimerManager().IsTimerActive(PatternHandle))
					GetWorldTimerManager().SetTimer(PatternHandle, this, &AKingTeaser::RoundShoot, ROUNDSHOOT_DELAY, true);
				break;

			case 5:
				if (!GetWorldTimerManager().IsTimerActive(PatternHandle))
					GetWorldTimerManager().SetTimer(PatternHandle, this, &AKingTeaser::Freeze, FREEZE_DELAY, true);
				break;

			default:
				break;

			}
		}
		else if (!ActivateBoss && PatternToFreeze < 3 && Pattern)
		{
			FlipbookComponent->SetFlipbook(IdleAnimation);

			if (!GetWorldTimerManager().IsTimerActive(DelayTimerHandle))
				GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AKingTeaser::DelayTimer, 2.0f, true);
		}
		else if (!ActivateBoss && PatternToFreeze <= 3)
		{
			if (!ActivateBoss)
			{
				PatternCount = 0.0f;
				Pattern = 5;

				FlipbookComponent->SetFlipbook(FreezeAnimation);

				ActivateBoss = true;
			}
		}
	}
}

void AKingTeaser::Chase()
{
	if (PlayerCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X >= 0)
	{
		if (PlayerCharacter->GetActorLocation().X - RootComponent->GetComponentLocation().X <= 100)
		{
			AddMovementInput(FVector(0.0f, 0.0f, 0.0f), 0.0f);
			FlipbookComponent->SetFlipbook(IdleAnimation);
		}
		else
		{
			AddMovementInput(FVector(1.0f, 0.0f, 0.0f), 0.4f);
			FlipbookComponent->SetFlipbook(RunAnimation);
		}

	}
	else
	{
		if (RootComponent->GetComponentLocation().X - PlayerCharacter->GetActorLocation().X <= 100)
		{
			AddMovementInput(FVector(0.0f, 0.0f, 0.0f), 0.0f);
			FlipbookComponent->SetFlipbook(IdleAnimation);
		}
		else
		{
			AddMovementInput(FVector(-1.0f, 0.0f, 0.0f), 0.4f);
			FlipbookComponent->SetFlipbook(RunAnimation);
		}
	}
}

void AKingTeaser::Fire()
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = FlipbookComponent->GetSocketLocation("Bullet");
		FRotator SpawnRotation = FlipbookComponent->GetSocketRotation("Bullet");

		AFastBullet* Projectile = GetWorld()->SpawnActor<AFastBullet>(SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			FVector	PlayerLocation = PlayerCharacter->GetActorLocation();

			FVector Result = PlayerLocation - SpawnLocation;
			Result.Normalize();

			Projectile->FireDirection(Result);
		}
	}

	PatternCount += CHASE_DELAY;

	if (PatternCount >= 4.0f)
	{
		ActivateBoss = false;
		GetWorldTimerManager().ClearTimer(PatternHandle);
	}
}

void AKingTeaser::FourLineShoot()
{
	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = FlipbookComponent->GetSocketLocation("Bullet");
		FRotator SpawnRotation = FlipbookComponent->GetSocketRotation("Bullet");

		ABounceBullet* Projectile = GetWorld()->SpawnActor<ABounceBullet>(SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			FVector	PlayerLocation = PlayerCharacter->GetActorLocation();
			FVector Result = PlayerLocation - SpawnLocation;
			Result.Normalize();

			Projectile->FireDirection(Result);
		}
	}
	PatternCount += LINESHOOT_DELAY;

	if (PatternCount >= 4.0f)
	{
		ActivateBoss = false;
		GetWorldTimerManager().ClearTimer(PatternHandle);
	}
}

void AKingTeaser::WaveShoot()
{
	if (WaveCount >= 360)
		WaveCount = 0;

	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = FlipbookComponent->GetSocketLocation("Bullet");
		FRotator SpawnRotation = FlipbookComponent->GetSocketRotation("Bullet");

		ABigBullet* Projectile = GetWorld()->SpawnActor<ABigBullet>(SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			FVector	PlayerLocation = PlayerCharacter->GetActorLocation();

			FVector Result = (PlayerLocation - SpawnLocation) + (600 * FMath::Sin(10 * WaveCount));
			Result.Y = 0.0f;
			Result.Normalize();

			Projectile->FireDirection(Result);
		}
	}
	WaveCount++;
	PatternCount += WAVESHOOT_DELAY;

	if (PatternCount >= 4.0f)
	{
		ActivateBoss = false;
		GetWorldTimerManager().ClearTimer(PatternHandle);
	}
}

void AKingTeaser::RoundShoot()
{
	if (AngleCount >= 360)
		AngleCount = 0;

	if (GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = FlipbookComponent->GetSocketLocation("Bullet");
		FRotator SpawnRotation = FlipbookComponent->GetSocketRotation("Bullet");

		ABigBullet* Projectile = GetWorld()->SpawnActor<ABigBullet>(SpawnLocation, SpawnRotation, SpawnParams);

		if (Projectile)
		{
			FVector	EndLocation;
			EndLocation.X = SpawnLocation.X + (FMath::Cos(AngleCount) * 100);
			EndLocation.Z = SpawnLocation.Z + (FMath::Sin(AngleCount) * 100);

			EndLocation.Y = 0;

			FVector Result = EndLocation - SpawnLocation;;

			Result.Normalize();

			Projectile->FireDirection(Result);
		}
	}

	AngleCount++;
	PatternCount += ROUNDSHOOT_DELAY;

	if (PatternCount >= 4.0f)
	{
		ActivateBoss = false;
		GetWorldTimerManager().ClearTimer(PatternHandle);
	}
}

void AKingTeaser::Freeze()
{
	PatternToFreeze = 0;

	ActivateBoss = false;
	GetWorldTimerManager().ClearTimer(PatternHandle);
}

void AKingTeaser::OverlapPawn(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

			if (!GetWorldTimerManager().IsTimerActive(HitTimerHandle))
				GetWorldTimerManager().SetTimer(HitTimerHandle, this, &AKingTeaser::HitTimer, 0.1f, true);

			HP -= Weapon->Damage;
			Weapon->GetCapsuleComponent()->SetCollisionProfileName(TEXT("HoldWeapon"));

			AudioComponent->SetSound(Damaged_Cue);
			AudioComponent->Play();

			if (HP <= 0)
			{
				Pattern = NULL;
				GetWorldTimerManager().ClearTimer(PatternHandle);
				GetWorldTimerManager().ClearTimer(DelayTimerHandle);
				FlipbookComponent->SetFlipbook(FreezeAnimation);

				AudioComponent->SetSound(Die_Cue);
				AudioComponent->Play();

				if (PlayerCharacter)
				{
					APlayerCharacter* Character = Cast<APlayerCharacter>(PlayerCharacter);
					Character->ShakeCamera();
				}

				GetCapsuleComponent()->SetCollisionProfileName(TEXT("HoldingWeapon"));

				if (Sight)
					Sight->DestroyComponent();

				if (!GetWorldTimerManager().IsTimerActive(DeathTimerHandle))
					GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AKingTeaser::DeathTimer, 0.1f, true);
			}

			Weapon->Durability--;

			if (Weapon->Durability <= 0)
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

void AKingTeaser::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
		{
			if (!GetWorldTimerManager().IsTimerActive(DelayTimerHandle))
				GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AKingTeaser::DelayTimer, 5.5f, true);

			PlayerCharacter = OtherActor;
		}
	}
}

void AKingTeaser::OverlapStop(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AKingTeaser::DelayTimer()
{
	GetWorldTimerManager().ClearTimer(DelayTimerHandle);

	if (PatternToFreeze < 3)
	{
		if (!ActivateBoss)
		{
			PatternCount = 0.0f;

			FRandomStream Rand;
			Rand.GenerateNewSeed();

			Pattern = Rand.RandRange(1, 4);

			FlipbookComponent->SetFlipbook(IdleAnimation);

			ActivateBoss = true;
		}
	}
	PatternToFreeze++;
}

void AKingTeaser::HitTimer()
{
	HitCount++;

	if (HitCount % 2 == 0)
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
	else
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

	if (HitCount >= 4)
	{
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
		GetWorldTimerManager().ClearTimer(HitTimerHandle);
		HitCount = 0;
	}
}

void AKingTeaser::DeathTimer()
{
	DeathCount++;

	if (DeathCount % 2 == 0)
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 0.6));
	else
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));

	if (DeathCount >= 41)
	{
		FlipbookComponent->SetSpriteColor(FLinearColor(1.0, 1.0, 1.0, 1.0));
		GetWorldTimerManager().ClearTimer(DeathTimerHandle);
		DeathCount = 0;

		this->Destroy();
	}
}