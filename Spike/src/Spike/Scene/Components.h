/*****************************************************************************/
/*                             Spike SourceCode                              */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: None                                                       */
/*                                                                           */
/*   Licensed under the Apache License, Version 2.0 (the "License");         */
/*   you may not use this file except in compliance with the License.        */
/*   You may obtain a copy of the License at                                 */
/*                                                                           */
/*       http://www.apache.org/licenses/LICENSE-2.0                          */
/*                                                                           */
/*   Unless required by applicable law or agreed to in writing, software     */
/*   distributed under the License is distributed on an "AS IS" BASIS,       */
/*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*/
/*   See the License for the specific language governing permissions and     */
/*   limitations under the License.                                          */
/*****************************************************************************/
#pragma once
#include "Spike/Renderer/Texture.h"
#include "Panels/ConsolePanel.h"
#include "SceneCamera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <FontAwesome.h>
#include "ScriptableEntity.h"
#include "Spike/Renderer/Mesh.h"

namespace Spike
{
    struct TagComponent
    {
        std::string Tag;
        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string tag)
            :Tag(tag) {}

        const char* GetName() { return "Tag"; }
    };
    struct TransformComponent
    {
        glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation)
            :Translation(translation) {}

        void Reset()
        {
            Translation = { 0.0f, 0.0f, 0.0f };
            Rotation = { 0.0f, 0.0f, 0.0f };
            Scale = { 1.0f, 1.0f, 1.0f };
        }
        const char* GetName() { return ICON_FK_WRENCH" Transform"; }
        glm::mat4 GetTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent
    {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture2D> Texture = nullptr;
        std::string TextureFilepath;
        float TilingFactor = 1.0f;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color)
            :Color(color) {}

        void SetTexture(const std::string& filepath)
        {
            Texture = Texture2D::Create(filepath);
            TextureFilepath = filepath;
            SPK_CORE_LOG_INFO("Successfully loaded Texture from: {0}", filepath);
        }

        void RemoveTexture() { Texture = nullptr; Console::Get()->Print("Successfully removed the Texture!", Console::LogLevel::LVL_INFO); }

        void Reset()
        {
            Color = { 1.0f, 1.0f, 1.0f, 1.0f };
            RemoveTexture();
        }
        const char* GetName() { return "Sprite Renderer"; }
    };

    struct MeshComponent
    {
        Ref<Spike::Mesh> Mesh;

        MeshComponent() = default;
        MeshComponent( const MeshComponent& other ) = default;
        MeshComponent( Ref<Spike::Mesh>& model )
            : Mesh( model )
        {
        }

        void Reset()
        {
        }

        operator Ref<Spike::Mesh>() { return Mesh; }

        const char* GetName() { return "Mesh"; }
    };

    struct CameraComponent
    {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        void Reset()
        {
            Primary = true;
            FixedAspectRatio = false;
        }
        const char* GetName() { return ICON_FK_CAMERA" Camera"; }
    };

    struct NativeScriptComponent
    {
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity* (*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
            DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
        }
        const char* GetName() { return "Native Script"; };
    };
}
