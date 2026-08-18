#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "ShanHePlayerState.generated.h"

UCLASS()
class SHANHEERA_API AShanHePlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    FPersonName PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    FCharacterStats Stats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    ESocialClass SocialClass = ESocialClass::Farmer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    ERegionType BirthRegion = ERegionType::NorthPlain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    int32 Age = 16;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    FResourceStock Wealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    TArray<FName> LearnedSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|玩家")
    TArray<FName> Titles;

    UFUNCTION(BlueprintCallable, Category="山河纪元|玩家")
    void AddReputation(int32 Amount) { Stats.Reputation += Amount; }
};
