/*
	The MIT License(MIT)

	Copyright(c) 2015 Vixen Team, Matt Guerrette

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files(the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions :
	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <vix_prefab.h>
#include <vix_stlutil.h>
#include <vix_scenemanager.h>
#include <vix_luascriptmanager.h>
#include <vix_luascript.h>
#include <vix_objectmanager.h>
#include <vix_resourcemanager.h>
#include <vix_components.h>

namespace Vixen {

	Prefab::Prefab()
	{
		m_refCount = 0;
	}

	Prefab::~Prefab()
	{
		for (auto& c : m_components)
			c->VOnDestroy();

		STLVEC_DELETE(m_components);
		STLVEC_DELETE(m_children);
	}


	std::string Prefab::GetName()
	{
		return m_name;
	}

	bool Prefab::GetEnabled()
	{
		return m_enabled;
	}

	std::string Prefab::GetModelFile()
	{
		return m_modelFile;
	}

	Transform Prefab::GetTransform()
	{
		return m_transform;
	}

	void Prefab::SetName(std::string name)
	{
		m_name = name;
	}

	void Prefab::SetEnabled(bool enabled)
	{
		m_enabled = enabled;
	}

	void Prefab::SetTransform(Transform t)
	{
		m_transform = t;
	}

	void Prefab::SetModelFile(std::string file)
	{
		m_modelFile = file;
	}

	void Prefab::AddChild(Prefab* child)
	{
		m_children.push_back(child);
	}

	void Prefab::AddScriptFile(std::string file)
	{
		m_scriptFiles.push_back(file);
	}

	GameObject* Prefab::BuildFromPrefab(Prefab* prefab)
	{
		//need to create game object with prefab data
		Vector3 _objectPos = prefab->GetTransform().GetPosition();
		Vector3 _objectRot = prefab->GetTransform().GetRotation();
		Vector3 _objectScale = prefab->GetTransform().GetScale();
		Transform* _objectTransform = new Transform(_objectPos, _objectRot, _objectScale);
		GameObject* _object = new GameObject(_objectTransform);
		_object->SetName(UStringFromCharArray(prefab->GetName().c_str()));
		_object->SetEnabled(prefab->GetEnabled(), false);
		ObjectManager::MapSceneObject(_object);


		uint32_t numComponents = prefab->m_components.size();
		for (uint32_t i = 0; i < numComponents; i++)
		{
			Component* component = prefab->m_components[i];

			if (component)
			{
				Component::Type type = component->VGetType();
				switch (type)
				{
					case Component::Type::LUA_SCRIPT:
					{

						//Should be able to be created from copy construct not reloading.
						LuaScript* _script = LuaScriptManager::LoadScript(((LuaScript*)component)->GetPath());
						_script->VBindParent(_object);
						_object->AddComponent(_script);

					} break;

					case Component::Type::MODEL:
					{
						ModelComponent* _modelComponent = (ModelComponent*)component;

						ModelComponent* _newComponent = new ModelComponent;
						_modelComponent->GetModel()->IncrementRefCount();
						_modelComponent->GetMaterial()->IncrementRefCount();
						_newComponent->SetModel(_modelComponent->GetModel());
						_newComponent->SetMaterial(_modelComponent->GetMaterial());

						_newComponent->VBindParent(_object);
						_object->AddComponent(_newComponent);
					} break;

					case Component::Type::CAMERA:
					{
						Camera3DComponent* _cameraComponent = (Camera3DComponent*)component;

						Camera3DComponent* _newCamera = new Camera3DComponent();

						//copy camera viewports
						Viewport v = _cameraComponent->GetCamera()->VGetViewport();
						_newCamera->GetCamera()->VSetViewport(v);


						_newCamera->VBindParent(_object);
						_object->AddComponent(_newCamera);

					} break;

					case Component::Type::UI_TEXT:
					{



					} break;
				}
			}
		}

		//build children
		size_t numChildren = prefab->ChildCount();
		for (size_t i = 0; i < numChildren; i++)
		{
			Prefab* _child = prefab->GetChild(i);

			_object->AddChild(BuildFromPrefab(_child));
		}

		return _object;
	}

	GameObject* Prefab::CreateObject()
	{
		GameObject* _object = BuildFromPrefab(this);

		SceneManager::ActiveScene()->AddSceneObject(_object);

		return _object;
	}

	void Prefab::AddComponent(Component* component)
	{
		m_components.push_back(component);
	}

	void Prefab::IncRefCount()
	{
		m_refCount++;
	}

	void Prefab::DecRefCount()
	{
		if (m_refCount > 0)
			m_refCount--;
	}

	uint32_t Prefab::RefCount()
	{
		return m_refCount;
	}

	Prefab* Prefab::GetChild(int index)
	{
		if (index > m_children.size())
			return NULL;
		else
			return m_children[index];
	}

	std::string Prefab::GetScriptFile(int index)
	{
		if (index > m_scriptFiles.size())
			return NULL;
		else
			return m_scriptFiles[index];
	}

	size_t Prefab::ScriptCount()
	{
		return m_scriptFiles.size();
	}

	size_t Prefab::ChildCount()
	{
		return m_children.size();
	}
}