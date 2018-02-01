#pragma once

#include "Blueprint/UserWidget.h"
#include "ItemSelectWidget.generated.h"

UCLASS()
class GAMEPROJECT_API UItemSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void Show(int32 InventoryLocation);
};
