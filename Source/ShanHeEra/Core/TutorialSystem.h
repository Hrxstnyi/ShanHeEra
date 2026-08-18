#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TutorialSystem.generated.h"

USTRUCT(BlueprintType)
struct FTutorialStep
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName StepID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Hint;           // 操作提示
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName TargetWidget;   // 高亮的UI元素
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsCompleted = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsMandatory = true;
};

USTRUCT(BlueprintType)
struct FTutorialChapter
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ChapterID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ChapterName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FTutorialStep> Steps;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsUnlocked = false;
};

/**
 * 新手引导系统 - 教程关卡、操作提示、系统解锁引导
 */
UCLASS()
class SHANHEERA_API UTutorialSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|引导")
    void InitializeTutorials();

    UFUNCTION(BlueprintCallable, Category="山河纪元|引导")
    void StartTutorial(FName ChapterID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|引导")
    void CompleteStep(FName StepID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|引导")
    void SkipTutorial();

    UFUNCTION(BlueprintCallable, Category="山河纪元|引导")
    FTutorialStep GetCurrentStep() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|引导")
    bool IsTutorialActive() const { return bIsActive; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|引导")
    TArray<FTutorialChapter> Chapters;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|引导")
    int32 CurrentChapterIndex = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|引导")
    int32 CurrentStepIndex = 0;

private:
    bool bIsActive = false;
};
