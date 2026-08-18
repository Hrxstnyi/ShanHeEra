#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpatialAudioSystem.generated.h"

UENUM(BlueprintType)
enum class EAmbientType : uint8
{
    Market      UMETA(DisplayName="集市"),
    Battlefield UMETA(DisplayName="战场"),
    Forest      UMETA(DisplayName="山林"),
    Village     UMETA(DisplayName="村庄"),
    Palace      UMETA(DisplayName="宫殿"),
    Temple      UMETA(DisplayName="寺庙"),
    River       UMETA(DisplayName="河边"),
    Desert      UMETA(DisplayName="沙漠"),
    Night       UMETA(DisplayName="夜晚")
};

USTRUCT(BlueprintType)
struct FShanHeAmbientSound
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EAmbientType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString SoundCuePath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float BaseVolume = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FString> RandomSounds; // 随机音效
};

/**
 * 3D音效系统 - 空间音效、环境音（集市/战场/山林）、脚步声
 */
UCLASS()
class SHANHEERA_API USpatialAudioSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|音效")
    void InitializeAudio();

    UFUNCTION(BlueprintCallable, Category="山河纪元|音效")
    void PlayAmbient(EAmbientType Type);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音效")
    void StopAmbient();

    UFUNCTION(BlueprintCallable, Category="山河纪元|音效")
    void PlayFootstep(FVector Location, float Volume = 0.3f);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音效")
    void PlaySpatialSound(FName SoundID, FVector Location, float Volume = 1.0f);

    UFUNCTION(BlueprintCallable, Category="山河纪元|音效")
    void SetReverbPreset(FName Preset); // 洞穴/大厅/户外

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音效")
    TMap<EAmbientType, FShanHeAmbientSound> AmbientDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音效")
    EAmbientType CurrentAmbient = EAmbientType::Village;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|音效")
    bool bIsAmbientPlaying = false;
};
