// Dop Practice Project.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public AAuraCharacterBase,public IEnemyInterface
{
	GENERATED_BODY()
public:
	AEnemyCharacter();
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
protected:
	virtual void BeginPlay() override;
};
