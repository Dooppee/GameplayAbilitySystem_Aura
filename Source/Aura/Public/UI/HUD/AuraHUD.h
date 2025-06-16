// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"
class UAuraOverlayWidgetController;
class UAbilitySystemComponent;
class UAuraWidgetController;
class UAuraUserWidget;
//逻辑整理
/*
 * 1.HUD中用来创建并且可视化的UserWidget，为了可以管理蓝图中的UserWidget,使用UserWidgetClass
 * 2.有了UserWidget，就需要有对应的WidgetController，用来处理UserWidget的逻辑，
 * 为了可以管理蓝图中的WidgetController,使用WidgetControllerClass
 * 3.在WidgetController中，需要有一个FWidgetControllerParams，用来传递给WidgetController
 * 4.用结构体包装WidgetController的参数，这样可以方便地传递给WidgetController
 * 5.初始化WidgetController，并给UserWidget设置WidgetController;
 * 6.开始思考什么时候初始化InitOverlay?初始话成功的条件是什么？
 * 初始化成功的条件是：
 * 				1.有PlayerController
 * 				2.有PlayerState
 * 				3.有AbilitySystemComponent
 * 				4.有AttributeSet
 *因此要找一个有这些东西的Actor，比如AuraCharacter;
*/
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UFUNCTION(BlueprintCallable)
	UAuraWidgetController* GetWidgetController(FWidgetControllerParams Params);
	UFUNCTION(BlueprintCallable)
	void InitOverlay(
		APlayerController* PC,
		APlayerState* PS,
		UAbilitySystemComponent* ASC,
		UAttributeSet* ABS);
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraOverlayWidgetController> OverlayWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAuraOverlayWidgetController> OverlayWidgetController;
};
