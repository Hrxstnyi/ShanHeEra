#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LiteratureSystem.generated.h"

UENUM(BlueprintType)
enum class EArtType : uint8
{
    Poetry      UMETA(DisplayName="诗词"),
    Painting    UMETA(DisplayName="绘画"),
    Calligraphy UMETA(DisplayName="书法"),
    Appreciation UMETA(DisplayName="鉴赏"),
    Collection  UMETA(DisplayName="收藏")
};

USTRUCT(BlueprintType)
struct FArtwork
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ArtworkID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EArtType Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Author;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Quality = 50; // 品质 0-100
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Value = 100; // 价值
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Content; // 诗词内容/画作描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Dynasty; // 朝代
};

USTRUCT(BlueprintType)
struct FLiteraryGathering
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName GatheringID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Location;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FText> Participants;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EArtType Theme;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 ReputationReward = 20;
};

/**
 * 诗词书画系统 - 文人线深度
 * 作诗/作画/书法/鉴赏/收藏，文人雅集
 */
UCLASS()
class SHANHEERA_API ULiteratureSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    void InitializeLiterature();

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    FArtwork CreatePoetry(int32 Intelligence, int32 LiteratureSkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    FArtwork CreatePainting(int32 Intelligence, int32 ArtSkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    FArtwork CreateCalligraphy(int32 Intelligence, int32 CalligraphySkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    int32 AppraiseArtwork(FArtwork Artwork, int32 AppraisalSkill);

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    bool CollectArtwork(FArtwork Artwork);

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    bool HostGathering(FLiteraryGathering Gathering);

    UFUNCTION(BlueprintCallable, Category="山河纪元|文艺")
    TArray<FArtwork> GetCollection() const;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|文艺")
    TArray<FArtwork> Collection; // 个人收藏

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|文艺")
    TMap<FName, FArtwork> MasterpieceDatabase; // 名作数据库

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|文艺")
    int32 LiteraryReputation = 0; // 文名
};
