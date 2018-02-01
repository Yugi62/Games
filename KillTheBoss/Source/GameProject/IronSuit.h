#pragma once

#include "GameFramework/Actor.h"
#include "Item.h"
#include "IronSuit.generated.h"

UCLASS()
class GAMEPROJECT_API AIronSuit : public ASuit
{
	GENERATED_BODY()
	
public:	

	AIronSuit();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	
	
};
