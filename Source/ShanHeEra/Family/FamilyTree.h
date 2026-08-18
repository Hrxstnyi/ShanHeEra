#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/ShanHeStructs.h"
#include "Core/ShanHeEnums.h"
#include "ShanHeFamilyTree.generated.h"

UENUM(BlueprintType)
enum class EInheritanceType : uint8
{
    Primogeniture   UMETA(DisplayName="长子继承"),
    EqualDivision   UMETA(DisplayName="诸子均分"),
    Ultimogeniture  UMETA(DisplayName="幼子继承"),
    Meritocracy     UMETA(DisplayName="选贤继承")
};

USTRUCT(BlueprintType)
struct FFamilyMember
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid MemberID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FPersonName Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Generation = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Age = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsAlive = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsMale = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid FatherID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid MotherID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FGuid SpouseID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FGuid> ChildrenIDs;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FCharacterStats Stats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<ECharacterTrait> Traits;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESocialClass SocialClass = ESocialClass::Farmer;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Occupation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Residence;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 BirthYear = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 DeathYear = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText LifeSummary; // 生平简介
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsPlayerCharacter = false;
};

/**
 * 家族树 - 管理整个家族的成员关系和世代
 * 支持多代传承，记录每个成员的生平
 */
UCLASS()
class SHANHEERA_API UFamilyTree : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    FGuid AddMember(const FPersonName& Name, int32 Generation, FGuid FatherID, FGuid MotherID, bool bIsMale);

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    void RemoveMember(FGuid MemberID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    FFamilyMember GetMember(FGuid MemberID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    TArray<FFamilyMember> GetChildren(FGuid ParentID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    TArray<FFamilyMember> GetGeneration(int32 Generation) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    int32 GetMemberCount() const { return Members.Num(); }

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    int32 GetAliveCount() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|家族树")
    int32 GetHighestGeneration() const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|家族树")
    TArray<FFamilyMember> Members;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|家族树")
    FName FamilyName;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|家族树")
    int32 FoundingYear = 0;
};
