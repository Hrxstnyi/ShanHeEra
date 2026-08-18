#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ExaminationSystem.generated.h"

UENUM(BlueprintType)
enum class EExamLevel : uint8
{
    County      UMETA(DisplayName="县试 (童生)"),
    Prefecture  UMETA(DisplayName="府试"),
    College     UMETA(DisplayName="院试 (秀才)"),
    Provincial  UMETA(DisplayName="乡试 (举人)"),
    Metropolitan UMETA(DisplayName="会试 (贡士)"),
    Palace      UMETA(DisplayName="殿试 (进士)")
};

UENUM(BlueprintType)
enum class EExamRank : uint8
{
    Failed      UMETA(DisplayName="落第"),
    Passed      UMETA(DisplayName="通过"),
    Excellent   UMETA(DisplayName="优等"),
    TopScorer   UMETA(DisplayName="案首/会元/状元")
};

USTRUCT(BlueprintType)
struct FExamAttempt
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EExamLevel Level;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Year = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EExamRank Rank = EExamRank::Failed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Score = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText EssayTopic;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Feedback;
};

USTRUCT(BlueprintType)
struct FExamRequirement
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EExamLevel Level;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredLearning = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredReputation = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RegistrationFee = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Location;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 HeldEveryYears = 3; // 乡试/会试每三年一次
};

/**
 * 科举系统 - 太吾绘卷式人生上升通道
 * 完整流程：县试->府试->院试(秀才)->乡试(举人)->会试(贡士)->殿试(进士)
 * 每次考试需要：学习积累 + 临场发挥 + 运气 + 人脉关系
 * 落第不是终点，可以成为教师、名士、或转向其他道路
 */
UCLASS()
class SHANHEERA_API UExaminationSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|科举")
    void InitializeExams();

    UFUNCTION(BlueprintCallable, Category="山河纪元|科举")
    bool CanTakeExam(EExamLevel Level, int32 PlayerLearning, int32 PlayerReputation, int32 PlayerGold) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|科举")
    EExamRank TakeExam(EExamLevel Level, int32 PlayerLearning, int32 PlayerTalent,
                       int32 PlayerLuck, bool bHasRecommendation);

    UFUNCTION(BlueprintCallable, Category="山河纪元|科举")
    FText GetExamTitle(EExamLevel Level) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|科举")
    FText GetRankTitle(EExamRank Rank, EExamLevel Level) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|科举")
    int32 CalculateScore(int32 Learning, int32 Talent, int32 Luck, bool bHasRecommendation) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|科举")
    TMap<EExamLevel, FExamRequirement> ExamRequirements;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|科举")
    TArray<FExamAttempt> PlayerExamHistory;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|科举")
    EExamLevel PlayerHighestLevel = EExamLevel::County;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|科举")
    int32 TotalAttempts = 0;

private:
    EExamRank DetermineRank(int32 Score, EExamLevel Level) const;
    FText GenerateFeedback(EExamRank Rank, EExamLevel Level) const;
};
