// Dop Practice Project.


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/UserWidget/AuraUserWidget.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"

UAuraWidgetController* AAuraHUD::GetWidgetController(FWidgetControllerParams Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController =
			NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->InitWidgetController(Params);
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(
		APlayerController* PC,
		APlayerState* PS,
		UAbilitySystemComponent* ASC,
		UAttributeSet* ABS)
{
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass is null"));
	checkf(OverlayWidgetClass,TEXT("OverlayWidget is null"));
	if (OverlayWidgetClass)
	{
		if (UUserWidget* Widget =
			CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass))
		{
			OverlayWidget = Cast<UAuraUserWidget>(Widget);
			if (OverlayWidget)
			{
				const FWidgetControllerParams Params(PC,PS,ASC,ABS);
				//这里会调用蓝图的逻辑让OnHealthChange 执行了绑定;
				OverlayWidget->SetWidgetController(GetWidgetController(Params));
				//蓝图中进行的绑定，这里进行广播
				OverlayWidgetController->BroadcastInitialValues();
				OverlayWidgetController->BindCallbacksToDependencies();
				OverlayWidget->AddToViewport();
			}
		}
	}
}
