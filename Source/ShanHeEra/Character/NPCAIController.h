#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCAIController.generated.h"

UCLASS()
class SHANHEERA_API ANPCAIController : public AAIController
{
    GENERATED_BODY()
public:
    ANPCAIController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="山河纪元|AI")
    float DecisionInterval = 5.0f;

    UFUNCTION(BlueprintCallable, Category="山河纪元|AI")
    void MakeDecision();

protected:
    virtual void BeginPlay() override;
    FTimerHandle DecisionTimer;
};
