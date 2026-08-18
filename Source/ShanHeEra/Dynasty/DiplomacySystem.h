#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHeDiplomacySystem.generated.h"

UENUM(BlueprintType)
enum class EDiplomacyRelation : uint8
{
    Hostile     UMETA(DisplayName="敌对"),
    Cold        UMETA(DisplayName="冷淡"),
    Neutral     UMETA(DisplayName="中立"),
    Friendly    UMETA(DisplayName="友好"),
    Allied      UMETA(DisplayName="同盟"),
    Vassal      UMETA(DisplayName="藩属"),
    Unified     UMETA(DisplayName="一统")
};

UENUM(BlueprintType)
enum class EDipActionType : uint8
{
    SendGift        UMETA(DisplayName="遣使送礼"),
    ProposeAlliance UMETA(DisplayName="提议结盟"),
    DeclareWar      UMETA(DisplayName="宣战"),
    DemandVassal    UMETA(DisplayName="要求臣服"),
    TradeAgreement  UMETA(DisplayName="贸易协定"),
    Marriage        UMETA(DisplayName="联姻"),
    PeaceOffer      UMETA(DisplayName="求和"),
    ClaimTerritory  UMETA(DisplayName="领土要求")
};

USTRUCT(BlueprintType)
struct FDiplomacyRelationData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FactionA;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FactionB;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EDiplomacyRelation Relation = EDiplomacyRelation::Neutral;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Opinion = 0; // -100~100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Trust = 50;   // 信任度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bAtWar = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bHasTrade = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WarDays = 0;
};

USTRUCT(BlueprintType)
struct FTributaryState
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName VassalFaction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SuzerainFaction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 AnnualTribute = 0; // 年贡
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Autonomy = 50;    // 自治度
};

/**
 * 外交系统 - 国家/势力间关系管理
 * 支持：关系度、同盟、宣战、藩属、朝贡、联姻、贸易协定
 */
UCLASS()
class SHANHEERA_API UDiplomacySystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    void InitializeDiplomacy();

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    void Simulate(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    bool PerformAction(EDipActionType Action, FName From, FName To, int32 Parameter = 0);

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    EDiplomacyRelation GetRelation(FName A, FName B) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    int32 GetOpinion(FName A, FName B) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    void ChangeOpinion(FName A, FName B, int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    TArray<FName> GetAllies(FName Faction) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|外交")
    TArray<FName> GetEnemies(FName Faction) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|外交")
    TArray<FDiplomacyRelationData> Relations;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|外交")
    TArray<FTributaryState> Tributaries;

private:
    void UpdateRelations(int32 Days);
    void CheckWarDecay();
    void AI_DiplomacyDecisions();
};
