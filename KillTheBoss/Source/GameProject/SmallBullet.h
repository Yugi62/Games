#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "SmallBullet.generated.h"

UCLASS()
class GAMEPROJECT_API ASmallBullet : public AProjectile
{
	GENERATED_BODY()
	
public:	

	ASmallBullet();
	virtual void BeginPlay() override;
};
