#include "include.h"
#include "GameInstance.h"

int main()
{
    GameInstance::GetInstance().Initialize(WIDTH, HEIGHT, "Katana Zero");

    GameInstance::GetInstance().Run();

    GameInstance::GetInstance().Release();
}