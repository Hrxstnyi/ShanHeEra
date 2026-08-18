#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RelationshipSystem.generated.h"

UENUM(BlueprintType)
enum class ERelationType : uint8
{
    Stranger    UMETA(DisplayName="陌生人"),
    Acquaintance UMETA(DisplayName="相识"),
    Friend      UMETA(DisplayName="朋友"),
    CloseFriend UMETA(DisplayName="挚友"),
    Lover       UMETA(DisplayName="恋人"),
    Spouse      UMETA(DisplayName="夫妻"),
    Family      UMETA(DisplayName="亲人"),
    Master      UMETA(DisplayName="师徒"),
    Sworn       UMETA(DisplayName="结拜"),
    Rival       UMETA(DisplayName="对手"),
    Enemy       UMETA(DisplayName="仇人"),
    Nemesis     UMETA(DisplayName="死敌")
};

USTRUCT(BlueprintType)
struct FRelationData
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid NPCA;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid NPCB;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ERelationType Type = ERelationType::Stranger;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Opinion = 0;     // -100~100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Trust = 0;       // 信任度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Romance = 0;     // 浪漫度
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DaysKnown = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsMarried = false;
};

/**
 * 关系系统 - NPC间关系网、好感度、婚姻、结拜、师徒、世仇
 * CK3式关系驱动玩法
 */
UCLASS()
class SHANHEERA_API URelationshipSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    void InitializeRelations();

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    void ChangeOpinion(FGuid A, FGuid B, int32 Amount);

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    bool ProposeMarriage(FGuid A, FGuid B);

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    bool ProposeSwornBrotherhood(FGuid A, FGuid B);

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    bool ProposeMasterDisciple(FGuid Master, FGuid Disciple);

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    void DeclareEnemy(FGuid A, FGuid B);

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    ERelationType GetRelation(FGuid A, FGuid B) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    int32 GetOpinion(FGuid A, FGuid B) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    TArray<FGuid> GetFamily(FGuid NPC) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    TArray<FGuid> GetFriends(FGuid NPC, int32 MinOpinion = 30) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|关系")
    TArray<FGuid> GetEnemies(FGuid NPC) const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|关系")
    TArray<FRelationData> Relations;

private:
    FRelationData* FindRelation(FGuid A, FGuid B);
    void UpdateRelationType(FRelationData& Rel);
};
