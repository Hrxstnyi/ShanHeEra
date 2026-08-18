#include "Character/NPCAIController.h"
#include "Core/ShanHeLog.h"

ANPCAIController::ANPCAIController() {}

void ANPCAIController::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(DecisionTimer, this, &ANPCAIController::MakeDecision, DecisionInterval, true);
}

void ANPCAIController::MakeDecision()
{
    // NPC 自主决策：工作、社交、移动、学习等
    // 不会等待玩家影响
}
