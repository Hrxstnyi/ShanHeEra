#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeLegalSystem.generated.h"

UENUM(BlueprintType)
enum class ECrimeType : uint8
{
    Theft       UMETA(DisplayName="盗窃"),
    Assault     UMETA(DisplayName="伤人"),
    Murder      UMETA(DisplayName="杀人"),
    Fraud       UMETA(DisplayName="诈骗"),
    TaxEvasion  UMETA(DisplayName="逃税"),
    Corruption  UMETA(DisplayName="贪腐"),
    Treason     UMETA(DisplayName="谋反"),
    Smuggling   UMETA(DisplayName="走私")
};

UENUM(BlueprintType)
enum class EPunishmentType : uint8
{
    Fine        UMETA(DisplayName="罚金"),
    Beating     UMETA(DisplayName="笞杖"),
    Imprisonment UMETA(DisplayName="监禁"),
    Exile       UMETA(DisplayName="流放"),
    Death       UMETA(DisplayName="死刑"),
    Amnesty     UMETA(DisplayName="赦免")
};

USTRUCT(BlueprintType)
struct FLawCase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CaseID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ECrimeType Crime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid DefendantID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid PlaintiffID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Severity = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsSolved = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EPunishmentType Punishment;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName CourtCity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysPending = 0;
};

/**
 * 法律司法系统 - 律法、案件审理、监狱、刑罚
 * 清官/贪官玩法的核心
 */
UCLASS()
class SHANHEERA_API ULegalSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|司法")
    void InitializeLaws();

    UFUNCTION(BlueprintCallable, Category="山河纪元|司法")
    FLawCase CreateCase(ECrimeType Crime, FGuid Defendant, FGuid Plaintiff, FName City);

    UFUNCTION(BlueprintCallable, Category="山河纪元|司法")
    EPunishmentType JudgeCase(FName CaseID, bool bIsGuilty, int32 CorruptionLevel);

    UFUNCTION(BlueprintCallable, Category="山河纪元|司法")
    void SimulateCases(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|司法")
    TArray<FLawCase> GetPendingCases(FName City) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|司法")
    TArray<FLawCase> AllCases;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|司法")
    int32 TotalPrisoners = 0;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|司法")
    float JudicialFairness = 70.0f; // 司法公正度
};
