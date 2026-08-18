#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "ShanHeCharacter.generated.h"

UCLASS()
class SHANHEERA_API AShanHeCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AShanHeCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色")
    FPersonName CharName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色")
    FCharacterStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色")
    ESocialClass SocialClass = ESocialClass::Farmer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色")
    TArray<ECharacterTrait> Traits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色")
    int32 Age = 30;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|角色")
    bool bIsAlive = true;

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色")
    bool HasTrait(ECharacterTrait Trait) const { return Traits.Contains(Trait); }

    UFUNCTION(BlueprintCallable, Category="山河纪元|角色")
    FString GetFullName() const { return CharName.Surname + CharName.GivenName; }
};
