#include "Dynasty/ExaminationSystem.h"
#include "Core/ShanHeLog.h"

void UExaminationSystem::InitializeExams()
{
    ExamRequirements.Empty();

    // 县试：入门级，成为童生
    FExamRequirement County;
    County.Level = EExamLevel::County;
    County.RequiredLearning = 10;
    County.RequiredReputation = 0;
    County.RegistrationFee = 100; // 铜钱
    County.Location = TEXT("LocalCounty");
    County.HeldEveryYears = 1;
    ExamRequirements.Add(EExamLevel::County, County);

    // 府试
    FExamRequirement Prefecture;
    Prefecture.Level = EExamLevel::Prefecture;
    Prefecture.RequiredLearning = 25;
    Prefecture.RequiredReputation = 5;
    Prefecture.RegistrationFee = 300;
    Prefecture.Location = TEXT("PrefectureCity");
    Prefecture.HeldEveryYears = 1;
    ExamRequirements.Add(EExamLevel::Prefecture, Prefecture);

    // 院试：通过后成为秀才（最低功名）
    FExamRequirement College;
    College.Level = EExamLevel::College;
    College.RequiredLearning = 50;
    College.RequiredReputation = 15;
    College.RegistrationFee = 500;
    College.Location = TEXT("PrefectureCity");
    College.HeldEveryYears = 1;
    ExamRequirements.Add(EExamLevel::College, College);

    // 乡试：每三年一次，通过后成为举人（可以做官了）
    FExamRequirement Provincial;
    Provincial.Level = EExamLevel::Provincial;
    Provincial.RequiredLearning = 80;
    Provincial.RequiredReputation = 30;
    Provincial.RegistrationFee = 1000;
    Provincial.Location = TEXT("ProvincialCapital");
    Provincial.HeldEveryYears = 3;
    ExamRequirements.Add(EExamLevel::Provincial, Provincial);

    // 会试：每三年一次，在京城
    FExamRequirement Metropolitan;
    Metropolitan.Level = EExamLevel::Metropolitan;
    Metropolitan.RequiredLearning = 95;
    Metropolitan.RequiredReputation = 50;
    Metropolitan.RegistrationFee = 2000;
    Metropolitan.Location = TEXT("Capital");
    Metropolitan.HeldEveryYears = 3;
    ExamRequirements.Add(EExamLevel::Metropolitan, Metropolitan);

    // 殿试：皇帝亲试，通过后成为进士
    FExamRequirement Palace;
    Palace.Level = EExamLevel::Palace;
    Palace.RequiredLearning = 100;
    Palace.RequiredReputation = 80;
    Palace.RegistrationFee = 0; // 殿试免费
    Palace.Location = TEXT("ImperialPalace");
    Palace.HeldEveryYears = 3;
    ExamRequirements.Add(EExamLevel::Palace, Palace);

    UE_LOG(LogShanHe, Log, TEXT("科举系统初始化 - 六级考试体系就绪"));
}

bool UExaminationSystem::CanTakeExam(EExamLevel Level, int32 PlayerLearning, int32 PlayerReputation, int32 PlayerGold) const
{
    if (const FExamRequirement* Req = ExamRequirements.Find(Level))
    {
        return PlayerLearning >= Req->RequiredLearning &&
               PlayerReputation >= Req->RequiredReputation &&
               PlayerGold >= Req->RegistrationFee;
    }
    return false;
}

EExamRank UExaminationSystem::TakeExam(EExamLevel Level, int32 PlayerLearning, int32 PlayerTalent,
                                        int32 PlayerLuck, bool bHasRecommendation)
{
    TotalAttempts++;

    int32 Score = CalculateScore(PlayerLearning, PlayerTalent, PlayerLuck, bHasRecommendation);
    EExamRank Rank = DetermineRank(Score, Level);

    FExamAttempt Attempt;
    Attempt.Level = Level;
    Attempt.Score = Score;
    Attempt.Rank = Rank;
    Attempt.Feedback = GenerateFeedback(Rank, Level);
    PlayerExamHistory.Add(Attempt);

    if (Rank != EExamRank::Failed && (int32)Level > (int32)PlayerHighestLevel)
    {
        PlayerHighestLevel = Level;
    }

    UE_LOG(LogShanHe, Log, TEXT("科举考试: %s，得分: %d，结果: %d"),
        *GetExamTitle(Level).ToString(), Score, (int32)Rank);

    return Rank;
}

int32 UExaminationSystem::CalculateScore(int32 Learning, int32 Talent, int32 Luck, bool bHasRecommendation) const
{
    // 分数构成：学识60% + 天赋20% + 运气15% + 推荐5%
    float Score = Learning * 0.6f + Talent * 0.2f + Luck * 0.15f;
    if (bHasRecommendation) Score += 5.0f;
    // 随机波动 ±10
    Score += FMath::RandRange(-10.0f, 10.0f);
    return FMath::Clamp((int32)Score, 0, 100);
}

EExamRank UExaminationSystem::DetermineRank(int32 Score, EExamLevel Level) const
{
    // 越高级的考试通过率越低
    float PassThreshold = 60.0f - (int32)Level * 3.0f;
    float ExcellentThreshold = 80.0f - (int32)Level * 2.0f;
    float TopThreshold = 95.0f - (int32)Level;

    if (Score >= TopThreshold) return EExamRank::TopScorer;
    if (Score >= ExcellentThreshold) return EExamRank::Excellent;
    if (Score >= PassThreshold) return EExamRank::Passed;
    return EExamRank::Failed;
}

FText UExaminationSystem::GetExamTitle(EExamLevel Level) const
{
    switch (Level)
    {
        case EExamLevel::County: return NSLOCTEXT("Exam","County","县试");
        case EExamLevel::Prefecture: return NSLOCTEXT("Exam","Prefecture","府试");
        case EExamLevel::College: return NSLOCTEXT("Exam","College","院试");
        case EExamLevel::Provincial: return NSLOCTEXT("Exam","Provincial","乡试");
        case EExamLevel::Metropolitan: return NSLOCTEXT("Exam","Metropolitan","会试");
        case EExamLevel::Palace: return NSLOCTEXT("Exam","Palace","殿试");
        default: return FText::GetEmpty();
    }
}

FText UExaminationSystem::GetRankTitle(EExamRank Rank, EExamLevel Level) const
{
    if (Rank == EExamRank::Failed) return NSLOCTEXT("Exam","Failed","落第");
    if (Rank == EExamRank::TopScorer)
    {
        switch (Level)
        {
            case EExamLevel::County:
            case EExamLevel::Prefecture:
            case EExamLevel::College: return NSLOCTEXT("Exam","AnShou","案首");
            case EExamLevel::Provincial: return NSLOCTEXT("Exam","JieYuan","解元");
            case EExamLevel::Metropolitan: return NSLOCTEXT("Exam","HuiYuan","会元");
            case EExamLevel::Palace: return NSLOCTEXT("Exam","ZhuangYuan","状元");
            default: break;
        }
    }
    if (Rank == EExamRank::Excellent) return NSLOCTEXT("Exam","Excellent","优等");
    return NSLOCTEXT("Exam","Passed","通过");
}

FText UExaminationSystem::GenerateFeedback(EExamRank Rank, EExamLevel Level) const
{
    switch (Rank)
    {
        case EExamRank::TopScorer:
            return NSLOCTEXT("Exam","FBTop","文章惊艳，主考官拍案叫绝！此子前途不可限量！");
        case EExamRank::Excellent:
            return NSLOCTEXT("Exam","FBExcellent","文笔俱佳，策论有据，优等通过。");
        case EExamRank::Passed:
            return NSLOCTEXT("Exam","FBPassed","中规中矩，勉强通过。继续努力。");
        case EExamRank::Failed:
        default:
            // 落第也有不同的反馈，给玩家希望
            int32 R = FMath::RandRange(0, 3);
            switch (R)
            {
                case 0: return NSLOCTEXT("Exam","FBFail1","文章尚可，但立意不新。下次当有所突破。");
                case 1: return NSLOCTEXT("Exam","FBFail2","策论偏颇，未中主考官之意。科举之路，漫漫其修远。");
                case 2: return NSLOCTEXT("Exam","FBFail3","时运不济。落第非终点，许多名臣也曾屡试不第。");
                default: return NSLOCTEXT("Exam","FBFail4","学识尚浅，还需苦读。三年后再来。");
            }
    }
}
