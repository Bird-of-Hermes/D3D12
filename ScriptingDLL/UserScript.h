#ifndef USER_SCRIPT_H
#define USER_SCRIPT_H

#ifdef SCRIPTINGDLL_EXPORTS
#define SCRIPTINGDLL_API __declspec(dllexport)
#else
#define SCRIPTINGDLL_API __declspec(dllimport)
#endif

#include <functional>
#include "framework.h"
#include "../DX12/src/headers/Components.h"
#include "../DX12/src/headers/Scene.h"

// If going to add new support for user script, create 2 objects (1 for _AppSync + 1 for AE namespace) that holds the function pointer (or std::function<>)
// and pass the appropriate pointers through App.cpp's Synchronize(_AppSync*) and bind them through UserScript.cpp's Synchronization(_AppSync*) 

struct _InputLink 
{
	std::function<int16_t()> GetMouseX;
	std::function<int16_t()> GetMouseY;
	std::function<int32_t()> GetMouseWheelTurn;
	std::function<const bool(uint8_t)> KeyDown;
};

struct _SManager
{
	std::function<const Map&(const char*)> GetScene;
	std::function<const Map&()> GetActiveScene;
	std::function<const std::string&()> GetActiveSceneName;
	std::function<void(const std::string&)> SetActiveScene;
};

// Used exclusively for DLL Initialization
struct _AppSync 
{
	_InputLink m_inputHandler;
	_SManager m_sManager;
	std::function<ECS::Transforms& (uint32_t)> transformFunc;
	std::function<ECS::MaterialProperties&	(uint32_t)> materialFunc;
};

// Interfaces for user
namespace AE
{
	namespace INPUT
	{
		inline static std::unique_ptr<_InputLink> GameInput{};
	}
	namespace SCENE
	{
		inline static std::unique_ptr<_SManager> Manager{};
	}
};

template <typename T>
struct _compGetter 
{
	friend void Synchronization(_AppSync*);
	T& GetComponent(uint32_t id) 
	{
		return Get(id);
	}

	inline static std::unique_ptr<_compGetter> _compInstances{};

private:
	std::function<T& (uint32_t)> Get;
};

extern "C" void SCRIPTINGDLL_API Synchronization(_AppSync*);
 
struct SCRIPTINGDLL_API UserScript : public ECS::EntityScript
{
	UserScript() : ECS::EntityScript(UINT32_MAX) {}
	UserScript(uint32_t entity) : ECS::EntityScript(entity) {};

	void OnUpdate(float dt);
	void InputOnUpdate(float dt);

	template <typename T>
	T& GetComponent()
	{
		return _compGetter<T>::_compInstances->GetComponent(m_ID);
	}
};

#endif