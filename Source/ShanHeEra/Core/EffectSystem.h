#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeEffectSystem.generated.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
    Skill       UMETA(DisplayName="技能特效"),
    Weather     UMETA(DisplayName="天气特效"),
    Fire        UMETA(DisplayName="火焰"),
    Water       UMETA(DisplayName="水流"),
    Smoke       UMETA(DisplayName="烟尘"),
    Explosion   UMETA(DisplayName="爆炸"),
    Blood       UMETA(DisplayName="血雾"),
    Magic       UMETA(DisplayName="法术"),
    UI          UMETA(DisplayName="UI特效")
};

USTRUCT(BlueprintType)
struct FEffectData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName EffectID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EEffectType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString NiagaraSystemPath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Scale = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsLooping = false;
};

/**
 * 特效系统 - Niagara粒子、技能特效、天气特效、火焰/水流/烟尘
 */
UCLASS()
class SHANHEERA_API UEffectSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void InitializeEffects();

    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void SpawnEffect(FName EffectID, FVector Location, FRotator Rotation, float Scale = 1.0f);

    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void SpawnWeatherEffect(EEffectType WeatherType, FVector Location);

    UFUNCTION(BlueprintCallable, Category="山河纪元|特效")
    void StopAllEffects();

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效")
    TMap<FName, FEffectData> EffectDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|特效")
    int32 ActiveEffects = 0;
};
