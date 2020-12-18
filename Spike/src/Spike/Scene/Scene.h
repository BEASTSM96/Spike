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

#include "Spike/Renderer/EditorCamera.h"
#include "Spike/Core/Timestep.h"

#pragma warning(push, 0)
#include <entt.hpp>
#pragma warning(pop)

namespace Spike
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnUpdateEditor(Timestep ts, EditorCamera& camera);
        void OnUpdateRuntime(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);
        Entity GetPrimaryCameraEntity();
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        entt::registry m_Registry;
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };
}