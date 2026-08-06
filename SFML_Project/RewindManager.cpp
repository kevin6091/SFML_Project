#include "RewindManager.h"
#include "GameInstance.h"
#include "ObjectManager.h"

void RewindManager::SetRewinding(bool boolean)
{
	bIsRewinding = boolean;
	if(!bIsRewinding)
		GameInstance::GetInstance().GetObjectManager().RestartObject();
}
