/*****************************************************************************/
/*                        Lightengine  SourceCode                            */
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

#include "LightEngine/Core/Layer.h"
#include "LightEngine/Events/ApplicationEvent.h"
#include "LightEngine/Events/KeyEvent.h"
#include "LightEngine/Events/MouseEvent.h"

namespace LightEngine
{
    class  ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;

        void Begin();
        void End();
        void BlockEvents(bool block) { m_BlockEvents = block; }
        void SetDarkThemeColors();
    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };

}