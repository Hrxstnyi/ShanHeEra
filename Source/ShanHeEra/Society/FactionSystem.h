#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FactionSystem.generated.h"

UENUM(BlueprintType)
enum class EFactionType : uint8
{
    Gang        UMETA(DisplayName="帮派"),
    MerchantGuild UMETA(DisplayName="商会"),
    RebelArmy   UMETA(DisplayName="义军"),
    Sect        UMETA(DisplayName="教派"),
    Mercenary   UMETA(DisplayName="佣兵团"),
    Clan        UMETA(DisplayName="宗族")
};

USTRUCT(BlueprintType)
struct FPlayerFaction
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName FactionID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText FactionName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EFactionType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid LeaderID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FGuid> Members;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Headquarters;  // 总部城市
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Reputation = 0; // 势力声望
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Treasury = 0;   // 金库
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Morale = 50;    // 士气
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Influence = 0;  // 影响力
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TMap<FName, int32> Holdings; // 控制的产业
};

/**
 * 玩家自建势力系统 - 招募NPC、建立帮派/商会/义军，势力发展壮大
 */
UCLASS()
class SHANHEERA_API UFactionSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    bool CreateFaction(FText Name, EFactionType Type, FName HQCity);

    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    bool RecruitMember(FGuid NPCID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    void ExpelMember(FGuid NPCID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    void CollectTreasury(int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    void ExpandToCity(FName CityID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    void SimulateFaction(int32 Days);

    UFUNCTION(BlueprintCallable, Category="山河纪元|势力")
    FPlayerFaction GetPlayerFaction() const { return PlayerFaction; }

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|势力")
    FPlayerFaction PlayerFaction;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|势力")
    bool bHasFaction = false;
};
