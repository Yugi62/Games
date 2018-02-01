#include "GameProject.h"
#include "Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetCollisionProfileName(TEXT("Projectile"));

	RootComponent = ProjectileCollision;

	SpriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->SetupAttachment(ProjectileCollision);
	SpriteComponent->SetCollisionProfileName(TEXT("Sprite"));
	SpriteComponent->SetEnableGravity(false);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileCollision);

	ProjectileCollision->SetSimulatePhysics(false);
	ProjectileCollision->GetBodyInstance()->bLockYTranslation = true;
	ProjectileCollision->GetBodyInstance()->bLockXRotation = true;
	ProjectileCollision->GetBodyInstance()->bLockYRotation = true;
	ProjectileCollision->GetBodyInstance()->bLockZRotation = true;

	ProjectileMovementComponent->Velocity = FVector(-1.0f,0.0f,0.0f) * ProjectileMovementComponent->InitialSpeed;
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OverlapStart);
}

void AProjectile::FireDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;

	FRotator FireRotation = ShootDirection.Rotation();
	FireRotation.Yaw = 0.0f;
	FireRotation.Pitch += -90.0f;

	SpriteComponent->SetRelativeRotation(FRotator(FireRotation * (ShootDirection.X>0.0f ? 1.0f:-1.0f )));

	
}

void AProjectile::OverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetActorClass()->IsChildOf(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);

		Character->LaunchCharacter(ProjectileMovementComponent->Velocity * 3.0f,false,false);
		Character->StartInvincibleTimer();

		this->Destroy();
	}
}
