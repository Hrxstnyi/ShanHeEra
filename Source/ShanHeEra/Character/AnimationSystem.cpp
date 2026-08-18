#include "Character/AnimationSystem.h"
#include "Core/ShanHeLog.h"

void UAnimationSystem::SetAction(ECharacterAction NewAction)
{
    if (NewAction != State.CurrentAction)
    {
        State.CurrentAction = NewAction;
        UE_LOG(LogShanHe, Log, TEXT("动画状态切换：%d"), (int32)NewAction);
    }
}

void UAnimationSystem::UpdateLocomotion(float Speed, float Direction)
{
    State.Speed = Speed;
    State.Direction = Direction;
    if (Speed > 0.1f && Speed < 200.0f)
        State.CurrentAction = ECharacterAction::Walk;
    else if (Speed >= 200.0f)
        State.CurrentAction = ECharacterAction::Run;
    else
        State.CurrentAction = ECharacterAction::Idle;
}

void UAnimationSystem::PlayAttackMontage(int32 ComboIndex)
{
    State.CurrentAction = ECharacterAction::Attack;
    State.ComboCount = ComboIndex;
    UE_LOG(LogShanHe, Log, TEXT("攻击动画：连击 %d"), ComboIndex);
}

void UAnimationSystem::PlayHitReact()
{
    State.CurrentAction = ECharacterAction::Hit;
    UE_LOG(LogShanHe, Log, TEXT("受击动画"));
}

void UAnimationSystem::PlayDeath()
{
    State.CurrentAction = ECharacterAction::Death;
    UE_LOG(LogShanHe, Log, TEXT("死亡动画"));
}

void UAnimationSystem::SetFacialExpression(FName Expression)
{
    // 喜/怒/哀/惧/惊/思
    TMap<FName, float> Defaults;
    Defaults.Add(TEXT("Joy"), 0.0f);
    Defaults.Add(TEXT("Anger"), 0.0f);
    Defaults.Add(TEXT("Sadness"), 0.0f);
    Defaults.Add(TEXT("Fear"), 0.0f);
    Defaults.Add(TEXT("Surprise"), 0.0f);
    Defaults.Add(TEXT("Thinking"), 0.0f);

    FacialBlendshapes = Defaults;
    if (FacialBlendshapes.Contains(Expression))
        FacialBlendshapes[Expression] = 1.0f;

    UE_LOG(LogShanHe, Log, TEXT("面部表情：%s"), *Expression.ToString());
}
