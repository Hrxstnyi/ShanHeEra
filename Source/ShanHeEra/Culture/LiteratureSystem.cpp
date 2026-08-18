#include "Culture/LiteratureSystem.h"
#include "Core/ShanHeLog.h"

void ULiteratureSystem::InitializeLiterature()
{
    struct FArtDef { FName ID; EArtType Type; const TCHAR* Title; const TCHAR* Author; int32 Quality; int32 Value; };
    FArtDef Arts[] = {
        {TEXT("poem_libai_01"), EArtType::Poetry, TEXT("将进酒"), TEXT("李白"), 100, 100000},
        {TEXT("poem_dumu_01"), EArtType::Poetry, TEXT("清明"), TEXT("杜牧"), 95, 50000},
        {TEXT("painting_qingming"), EArtType::Painting, TEXT("清明上河图"), TEXT("张择端"), 100, 1000000},
        {TEXT("painting_fuchun"), EArtType::Painting, TEXT("富春山居图"), TEXT("黄公望"), 100, 800000},
        {TEXT("calligraphy_lanting"), EArtType::Calligraphy, TEXT("兰亭集序"), TEXT("王羲之"), 100, 2000000},
        {TEXT("calligraphy_jishen"), EArtType::Calligraphy, TEXT("祭侄文稿"), TEXT("颜真卿"), 98, 500000},
    };

    for (const auto& A : Arts)
    {
        FArtwork Art;
        Art.ArtworkID = A.ID; Art.Type = A.Type;
        Art.Title = FText::FromString(UTF8_TO_TCHAR(A.Title));
        Art.Author = FText::FromString(UTF8_TO_TCHAR(A.Author));
        Art.Quality = A.Quality; Art.Value = A.Value;
        MasterpieceDatabase.Add(A.ID, Art);
    }

    UE_LOG(LogShanHe, Log, TEXT("文艺系统初始化 - %d 件名作"), MasterpieceDatabase.Num());
}

FArtwork ULiteratureSystem::CreatePoetry(int32 Intelligence, int32 LiteratureSkill)
{
    FArtwork Poem;
    Poem.Type = EArtType::Poetry;
    Poem.Quality = FMath::Clamp(Intelligence * 3 + LiteratureSkill * 2 + FMath::RandRange(-10, 10), 0, 100);
    Poem.Value = Poem.Quality * 100;
    Poem.Author = FText::FromString(TEXT("玩家"));
    LiteraryReputation += Poem.Quality / 10;
    UE_LOG(LogShanHe, Log, TEXT("创作诗词，品质%d，价值%d"), Poem.Quality, Poem.Value);
    return Poem;
}

FArtwork ULiteratureSystem::CreatePainting(int32 Intelligence, int32 ArtSkill)
{
    FArtwork Painting;
    Painting.Type = EArtType::Painting;
    Painting.Quality = FMath::Clamp(Intelligence * 2 + ArtSkill * 3 + FMath::RandRange(-10, 10), 0, 100);
    Painting.Value = Painting.Quality * 200;
    UE_LOG(LogShanHe, Log, TEXT("创作绘画，品质%d，价值%d"), Painting.Quality, Painting.Value);
    return Painting;
}

FArtwork ULiteratureSystem::CreateCalligraphy(int32 Intelligence, int32 CalligraphySkill)
{
    FArtwork Calligraphy;
    Calligraphy.Type = EArtType::Calligraphy;
    Calligraphy.Quality = FMath::Clamp(Intelligence * 2 + CalligraphySkill * 3 + FMath::RandRange(-10, 10), 0, 100);
    Calligraphy.Value = Calligraphy.Quality * 150;
    UE_LOG(LogShanHe, Log, TEXT("创作书法，品质%d，价值%d"), Calligraphy.Quality, Calligraphy.Value);
    return Calligraphy;
}

int32 ULiteratureSystem::AppraiseArtwork(FArtwork Artwork, int32 AppraisalSkill)
{
    int32 Estimate = Artwork.Quality + AppraisalSkill + FMath::RandRange(-20, 20);
    Estimate = FMath::Clamp(Estimate, 0, 100);
    UE_LOG(LogShanHe, Log, TEXT("鉴赏作品：估计品质%d（实际%d）"), Estimate, Artwork.Quality);
    return Estimate;
}

bool ULiteratureSystem::CollectArtwork(FArtwork Artwork)
{
    Collection.Add(Artwork);
    LiteraryReputation += Artwork.Quality / 5;
    UE_LOG(LogShanHe, Log, TEXT("收藏作品：%s"), *Artwork.Title.ToString());
    return true;
}

bool ULiteratureSystem::HostGathering(FLiteraryGathering Gathering)
{
    LiteraryReputation += Gathering.ReputationReward;
    UE_LOG(LogShanHe, Log, TEXT("举办文人雅集：%s，文名+%d"), *Gathering.Name.ToString(), Gathering.ReputationReward);
    return true;
}

TArray<FArtwork> ULiteratureSystem::GetCollection() const
{
    return Collection;
}
