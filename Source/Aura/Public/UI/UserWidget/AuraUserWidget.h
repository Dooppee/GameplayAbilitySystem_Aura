// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 1.记住MVP的设计模式 模型 视图 控制器;
 * 2.所有要预留WidgetController的插槽，用来单项管理UserWidget
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UObject> WidgetController;
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
