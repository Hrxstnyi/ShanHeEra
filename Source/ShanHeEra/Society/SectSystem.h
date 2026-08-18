#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeSectSystem.generated.h"

UENUM(BlueprintType)
enum class ESectRank : uint8
{
    Disciple    UMETA(DisplayName="普通弟子"),
    Elder       UMETA(DisplayName="长老"),
    Deputy      UMETA(DisplayName="副掌门"),
    Leader      UMETA(DisplayName="掌门")
};

USTRUCT(BlueprintType)
struct FSect
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SectID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SectName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MemberCount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Influence = 0; // 势力值
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Wealth = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESectRank PlayerRank = ESectRank::Disciple;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> AlliedSects;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> EnemySects;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName BaseLocation;
};

USTRUCT(BlueprintType)
struct FSectMission
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName MissionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredRank = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RewardInfluence = 10;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RewardWealth = 50;
};

/**
 * 帮派宗门系统 - 加入/创建帮派，门派任务，门派战争，掌门争夺
 */
UCLASS()
class SHANHEERA_API USectSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    void InitializeSects();

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    bool JoinSect(FName SectID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    bool CreateSect(FText SectName, FName BaseLocation);

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    bool CompleteMission(FName MissionID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    bool ChallengeLeadership(); // 争夺掌门

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    void DeclareWar(FName EnemySectID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    void ProposeAlliance(FName AlliedSectID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    ESectRank GetPlayerRank(FName SectID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|帮派")
    TArray<FSectMission> GetAvailableMissions(FName SectID) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|帮派")
    TMap<FName, FSect> SectDatabase;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|帮派")
    FName CurrentSect = NAME_None;
};
