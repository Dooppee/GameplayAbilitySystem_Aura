// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"
DECLARE_MULTICAST_DELEGATE_OneParam(FGameEffectTagDelegate, const FGameplayTagContainer& /*AssetTags*/);
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityEffectSet();
	FGameEffectTagDelegate GameEffectTagDelegate;
protected:
	void GameplayEffect(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
};
