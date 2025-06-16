// Dop Practice Project.


#include "UI/WidgetController/AuraWidgetController.h"


void UAuraWidgetController::InitWidgetController(FWidgetControllerParams InParames)
{
	PlayerController = InParames.PlayerController;
	PlayerState = InParames.PlayerState;
	AbilitySystemComponent = InParames.AbilitySystemComponent;
	AttributeSet = InParames.AttributeSet;
}
