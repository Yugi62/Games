#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "BigBullet.generated.h"

UCLASS()
class GAMEPROJECT_API ABigBullet : public AProjectile
{
	GENERATED_BODY()
	
public:	

	ABigBullet();
	virtual void BeginPlay() override;
	
};
