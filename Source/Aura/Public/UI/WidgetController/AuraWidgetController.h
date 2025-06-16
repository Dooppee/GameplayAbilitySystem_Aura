// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

USTRUCT(Blueprintable)
struct FWidgetControllerParams
{
	GENERATED_BODY()
	FWidgetControllerParams(){};
	FWidgetControllerParams(APlayerController* PC ,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* ABS):
		PlayerController(PC),PlayerState(PS),AbilitySystemComponent(ASC),AttributeSet(ABS){};
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState = nullptr;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};
/**
 * 
 */
UCLASS(Blueprintable)
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void InitWidgetController(FWidgetControllerParams InParames);
protected:
	//WidgetController进行单项依赖
	//Widget依赖WidgetController
	//WidgetController以来Mode
	//这就是MVC模式 MVC模式是一种软件设计模式，它将应用程序分为三个主要部分：模型、视图和控制器。
	UPROPERTY(BlueprintReadOnly,category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly,category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	UPROPERTY(BlueprintReadOnly,category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly,category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
