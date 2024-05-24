#include "UserScript.h"
#include "../DX12/src/Objects.cpp"
#include "../DX12/src/Scene.cpp"

void SCRIPTINGDLL_API UserScript::OnUpdate(float dt)
{
    InputOnUpdate(dt);
}

void UserScript::InputOnUpdate(float dt)
{
    using namespace AE::INPUT;
    const float angleUnit = 75.0f * dt;
    const float moveUnit = 3 * 125.0f * dt;

    static float cameraTimer = 0;
    cameraTimer += dt;

    auto& entity = GetComponent<ECS::Transforms>();
    //auto& entTexture = GetComponent<ECS::Texture2D>();
    //entTexture.uvwOffset.u += dt * 0.1f;
    //entTexture.uvwOffset.v += dt * 0.1f;

    // Camera
    auto& activeScene = AE::SCENE::Manager->GetScene(AE::SCENE::Manager->GetActiveSceneName().c_str());
    auto& camera = activeScene.GetCamera();

    const Vec3f moveForward = MoveInDirection(camera, {0, 0, 1});
    const Vec3f moveRight = MoveInDirection(camera, {1, 0, 0});

    if (GameInput->KeyDown(VK_UP))
    {
        entity.Rotation.y += angleUnit;
    }
    if (GameInput->KeyDown(VK_DOWN))
    {
        entity.Rotation.y -= angleUnit;
    }
    if (GameInput->KeyDown(VK_RIGHT))
    {
        entity.Rotation.x += angleUnit;
    }
    if (GameInput->KeyDown(VK_LEFT))
    {
        entity.Rotation.x -= angleUnit;
    }
    if (GameInput->KeyDown('I'))
    {
        entity.Rotation.z += angleUnit;
    }
    if (GameInput->KeyDown('O'))
    {
        entity.Rotation.z -= angleUnit;
    }
    ///////////////////////////////////////////////////////////////

    if (GameInput->KeyDown('W'))
    {
        entity.Translation += moveForward * moveUnit;
    }
    if (GameInput->KeyDown('S'))
    {
        entity.Translation += moveForward * -moveUnit;
    }
    if (GameInput->KeyDown('A'))
    {
        entity.Translation += moveRight * -moveUnit;
    }
    if (GameInput->KeyDown('D'))
    {
        entity.Translation += moveRight * moveUnit;
    }
    //if (GameInput->KeyDown(VK_PRIOR)) // pg up
    //{
    //    entity.Translation += up * moveUnit;
    //}
    //if (GameInput->KeyDown(VK_NEXT)) // pg down
    //{
    //    entity.Translation += -up * moveUnit;
    //}
}

void SCRIPTINGDLL_API Synchronization(_AppSync* appState)
{
	using namespace ECS;
    using namespace AE::INPUT;

    {
        GameInput = std::make_unique<_InputLink>();
        GameInput->GetMouseWheelTurn = appState->m_inputHandler.GetMouseWheelTurn;
        GameInput->GetMouseX = appState->m_inputHandler.GetMouseX;
        GameInput->GetMouseY = appState->m_inputHandler.GetMouseY;
        GameInput->KeyDown = appState->m_inputHandler.KeyDown;
    }

    {
        AE::SCENE::Manager = std::make_unique<_SManager>();
        AE::SCENE::Manager->GetScene = appState->m_sManager.GetScene;
        AE::SCENE::Manager->GetActiveScene = appState->m_sManager.GetActiveScene;
        AE::SCENE::Manager->GetActiveSceneName = appState->m_sManager.GetActiveSceneName;
        AE::SCENE::Manager->SetActiveScene = appState->m_sManager.SetActiveScene;
    }

	_compGetter<Transforms>::_compInstances = std::make_unique<_compGetter<Transforms>>();
	_compGetter<MaterialProperties>::_compInstances = std::make_unique<_compGetter<MaterialProperties>>();

	_compGetter<Transforms>::_compInstances->Get = appState->transformFunc;
    _compGetter<MaterialProperties>::_compInstances->Get = appState->materialFunc;
}