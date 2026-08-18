#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeCGManager.generated.h"

class UMediaPlayer;
class UMediaSource;

USTRUCT(BlueprintType)
struct FCGSequence
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CGID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName MusicTrack;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsSkippable = true;
};

UCLASS()
class SHANHEERA_API UCGManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void PlayCG(FName CGID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void SkipCG();

    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    bool IsPlaying() const { return bIsPlaying; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    TArray<FCGSequence> CGLibrary;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    FCGSequence CurrentCG;

private:
    bool bIsPlaying = false;
};
