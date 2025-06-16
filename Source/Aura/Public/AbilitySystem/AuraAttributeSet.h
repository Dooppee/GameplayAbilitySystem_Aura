// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};
//ATTRIBUTE_ACCESSORS是我在子类中定义的一个总的宏
//下面四个是父类中的宏，通过定义的总类宏去调用父类宏实现新的宏定义
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
// 当调用Get And Set 的时候
	/*// 开发者调用
	UAuraAttributeSet->GetHealth(); 
	UAuraAttributeSet->SetHealth(100.f);
	// 实际展开代码
	UAuraAttributeSet->Health.GetCurrentValue();
	{
	 UAuraAttributeSet::SetHealth(100.f)
	 → 2. AbilitySystemComponent->SetNumericAttributeBase(HealthAttribute, 100.f)
	 → 3. 修改 Health.BaseValue
	 → 4. 重新计算 Health.CurrentValue（考虑Buff/Debuff）
	 → 5. 标记属性为Dirty
	 → 6. 服务器同步到客户端（如有）
	 → 7. 客户端触发 OnRep_Health
	 → 8. GAMEPLAYATTRIBUTE_REPNOTIFY 广播事件
	}

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	//FGameplayAttributeData 注册后的 数据改变前
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	//GE执行后
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	//用到了UE中的网络同步，当服务器中的Health数值发生变化时，通知客户端并发生回调，OnRep_Health，并传入旧的数值
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Health,Category="Vital Attribute")
	//GAS集成结构体FGameplayAttributeData相比普通float支持预测和修饰
	//FGameplayAttributeData 其实内部有两个float类型的数据：Base / Current
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health)
	//通过GAS中的宏定义实现数据访问器的定义（函数定义）
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_MaxHealth,Category="Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth)
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Mana,Category="Vital Attribute")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana)
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_MaxMana,Category="Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana)
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	UPROPERTY()
	FEffectProperties Properties;
};

