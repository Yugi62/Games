#pragma once

#include "GameFramework/Actor.h"
#include "Item.h"
#include "IronHelmet.generated.h"

UCLASS()
class GAMEPROJECT_API AIronHelmet : public AHelmet
{
	GENERATED_BODY()
	
public:	

	AIronHelmet();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
