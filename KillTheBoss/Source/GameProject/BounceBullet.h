#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "BounceBullet.generated.h"

UCLASS()
class GAMEPROJECT_API ABounceBullet : public AProjectile
{
	GENERATED_BODY()
	
public:	

	ABounceBullet();
	virtual void BeginPlay() override;
};
