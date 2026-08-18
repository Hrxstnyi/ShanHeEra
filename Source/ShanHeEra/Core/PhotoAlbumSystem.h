#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShanHePhotoAlbumSystem.generated.h"

UENUM(BlueprintType)
enum class EPhotoFilter : uint8
{
    None        UMETA(DisplayName="原图"),
    Ink         UMETA(DisplayName="水墨"),
    Sepia       UMETA(DisplayName="复古"),
    Vivid       UMETA(DisplayName="鲜艳"),
    Bamboo      UMETA(DisplayName="竹韵"),
    Silk        UMETA(DisplayName="绢帛")
};

USTRUCT(BlueprintType)
struct FPhotoEntry
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PhotoID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString FilePath;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Caption;       // 题字
    UPROPERTY(EditAnywhere, BlueprintReadWrite) EPhotoFilter Filter;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName LocationID;    // 拍摄地点
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Year = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Month = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 Day = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsFavorite = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FName> TaggedNPCs;
};

/**
 * 拍照相册系统 - 游戏内拍照、滤镜、相册收藏、分享
 * 国风滤镜（水墨/绢帛/竹韵）
 */
UCLASS()
class SHANHEERA_API UPhotoAlbumSystem : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    void TakePhoto(EPhotoFilter Filter, FText Caption);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    void DeletePhoto(FName PhotoID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    void ToggleFavorite(FName PhotoID);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    void AddCaption(FName PhotoID, FText Caption);

    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    TArray<FPhotoEntry> GetPhotosByLocation(FName LocationID) const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    TArray<FPhotoEntry> GetFavoritePhotos() const;

    UFUNCTION(BlueprintCallable, Category="山河纪元|相册")
    void ExportPhoto(FName PhotoID, const FString& ExportPath);

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相册")
    TArray<FPhotoEntry> Photos;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相册")
    int32 MaxPhotos = 200;

    UPROPERTY(BlueprintReadOnly, Category="山河纪元|相册")
    bool bIsCameraMode = false;
};
