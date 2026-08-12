#include "RewindManager.h"
#include "GameInstance.h"
#include "ObjectManager.h"
#include "SoundManager.h"

void RewindManager::SetRewinding(bool boolean)
{
	if (!bIsRewinding && boolean == true)
	{
		SoundManager::GetInstance().PlaySFXWithReverb("rewind", 50.f, 1.0f, 7, 0.5f);
	}

	bIsRewinding = boolean;
	if(!bIsRewinding)
		GameInstance::GetInstance().GetObjectManager().RestartObject();
}
