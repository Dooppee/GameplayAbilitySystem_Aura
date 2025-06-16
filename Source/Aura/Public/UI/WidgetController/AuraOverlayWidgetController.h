// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"


class UAuraUserWidget;
struct FGameplayTag;
struct FOnAttributeChangeData;
USTRUCT(BlueprintType)
struct  FUIWidgetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag  MessageTas = FGameplayTag();
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText MessageText = FText();
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget>MessageWidget;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

//自定义的标识符；传参后的数据类型，参数名称
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViewChangeSignature,float,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,Row);

/**
* 1.FOnHealthChangeSignature 
	*这是动态多播委托的类型名，需以F开头（UE命名规范），用于后续声明委托实例（如FOnHealthChangeSignature OnHealthChanged;）
* 2.float
	表示委托回调函数的参数类型（不是返回值类型）。此委托的回调函数需接收一个float参数（如void MyCallback(float NewHealth)），无返回值	
* 3.NewHealth
	是回调函数中参数的变量名（仅用于宏内部生成代码时的标识），实际调用时可自定义参数名（如OnHealthChanged.Broadcast(CurrentHealth)）
*/
UCLASS()
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies(); 
protected:
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnViewChangeSignature OnHealthChange;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnViewChangeSignature OnMaxHealthChange;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnViewChangeSignature OnManaChange;
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnViewChangeSignature OnMaxManaChange;
	UPROPERTY(BlueprintAssignable, Category="GAS|Message")
	FMessageWidgetRowSignature MessageWidgetRowSignature;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetTableRowByTag(UDataTable*,const FGameplayTag&);
};

template <typename T>
T* UAuraOverlayWidgetController::GetTableRowByTag(UDataTable* DataTable,const FGameplayTag& GameplayTag)
{
	return DataTable->FindRow<T>(GameplayTag.GetTagName(),TEXT(""));;
}
