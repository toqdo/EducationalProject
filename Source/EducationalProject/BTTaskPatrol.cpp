// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskPatrol.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIEnemyController.h"

EBTNodeResult::Type UBTTaskPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIEnemyController* Controller = Cast<AAIEnemyController>(OwnerComp.GetAIOwner());

	float PatrolRadius = Controller->GetPatrolRadius();

	if (PatrolRadius > 0.f)
	{
		FNavLocation ResultLocation;

		if (UNavigationSystemV1::GetNavigationSystem(&OwnerComp)->GetRandomReachablePointInRadius(Controller->GetNavAgentLocation(), PatrolRadius, ResultLocation))
		{
			Controller->GetBlackboardComponent()->SetValueAsVector(Controller->GetPatrolTargetKey(), ResultLocation.Location);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}