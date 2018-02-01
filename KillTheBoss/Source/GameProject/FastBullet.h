#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "FastBullet.generated.h"

UCLASS()
class GAMEPROJECT_API AFastBullet : public AProjectile
{
	GENERATED_BODY()
	
public:	

	AFastBullet();
	virtual void BeginPlay() override;

};
