#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/CinematicDirector.h"
#include "Core/ShanHeStructs.h"
#include "ShanHeCGManager.generated.h"
class UMediaPlayer;
class UMediaTexture;
class UAudioComponent;
USTRUCT(BlueprintType)
struct FCGSequence
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CGID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString MediaPath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float Duration = 30.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bCanSkip = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECinematicSequenceID CinematicSequenceID = ECinematicSequenceID::None;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FSubtitleEntry> Subtitles;
};
UCLASS()
class SHANHEERA_API UCGManager : public UObject
{
    GENERATED_BODY()
public:
    UCGManager();
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void InitializeCG();
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void PlayCG(FName CGID);
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void PlayCGWithCinematic(FName CGID);
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void StopCG();
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    void SkipCG();
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    bool IsPlaying() const { return bIsPlaying; }
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    float GetPlaybackTime() const;
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG")
    FCinematicSequence GetCinematicForCG(FName CGID);
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG|字幕")
    void ShowSubtitle(const FSubtitleEntry& Subtitle);
    UFUNCTION(BlueprintCallable, Category="山河纪元|CG|字幕")
    void HideSubtitle();
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    TMap<FName, FCGSequence> CGLibrary;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    FName CurrentCGID;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    bool bIsPlaying = false;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    bool bSubtitleVisible = false;
    UPROPERTY(BlueprintReadOnly, Category="山河纪元|CG")
    FText CurrentSubtitle;
private:
    float PlaybackTime = 0.0f;
    int32 CurrentSubtitleIndex = -1;
    void UpdateSubtitles(float DeltaTime);
};
