#include "Core/PhotoAlbumSystem.h"
#include "Core/ShanHeLog.h"

void UPhotoAlbumSystem::TakePhoto(EPhotoFilter Filter, FText Caption)
{
    if (Photos.Num() >= MaxPhotos)
    {
        UE_LOG(LogShanHe, Warning, TEXT("相册已满，无法拍照"));
        return;
    }

    FPhotoEntry Photo;
    Photo.PhotoID = FName(*FString::Printf(TEXT("Photo_%d"), FDateTime::Now().GetTicks()));
    Photo.Filter = Filter;
    Photo.Caption = Caption;
    Photo.FilePath = FString::Printf(TEXT("Saved/Photos/%s.png"), *Photo.PhotoID.ToString());
    Photos.Add(Photo);

    UE_LOG(LogShanHe, Log, TEXT("拍照成功！滤镜：%d，题字：%s，总计：%d张"),
        (int32)Filter, *Caption.ToString(), Photos.Num());
}

void UPhotoAlbumSystem::DeletePhoto(FName PhotoID)
{
    for (int32 i = 0; i < Photos.Num(); i++)
    {
        if (Photos[i].PhotoID == PhotoID)
        {
            Photos.RemoveAt(i);
            UE_LOG(LogShanHe, Log, TEXT("删除照片：%s"), *PhotoID.ToString());
            return;
        }
    }
}

void UPhotoAlbumSystem::ToggleFavorite(FName PhotoID)
{
    for (FPhotoEntry& P : Photos)
    {
        if (P.PhotoID == PhotoID)
        {
            P.bIsFavorite = !P.bIsFavorite;
            UE_LOG(LogShanHe, Log, TEXT("收藏状态：%s -> %s"), *PhotoID.ToString(),
                P.bIsFavorite ? TEXT("已收藏") : TEXT("取消收藏"));
            return;
        }
    }
}

void UPhotoAlbumSystem::AddCaption(FName PhotoID, FText Caption)
{
    for (FPhotoEntry& P : Photos)
    {
        if (P.PhotoID == PhotoID)
        {
            P.Caption = Caption;
            UE_LOG(LogShanHe, Log, TEXT("题字更新：%s"), *Caption.ToString());
            return;
        }
    }
}

TArray<FPhotoEntry> UPhotoAlbumSystem::GetPhotosByLocation(FName LocationID) const
{
    TArray<FPhotoEntry> Result;
    for (const FPhotoEntry& P : Photos)
        if (P.LocationID == LocationID) Result.Add(P);
    return Result;
}

TArray<FPhotoEntry> UPhotoAlbumSystem::GetFavoritePhotos() const
{
    TArray<FPhotoEntry> Result;
    for (const FPhotoEntry& P : Photos)
        if (P.bIsFavorite) Result.Add(P);
    return Result;
}

void UPhotoAlbumSystem::ExportPhoto(FName PhotoID, const FString& ExportPath)
{
    UE_LOG(LogShanHe, Log, TEXT("导出照片：%s -> %s"), *PhotoID.ToString(), *ExportPath);
}
