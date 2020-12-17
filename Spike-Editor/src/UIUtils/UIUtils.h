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

//UI utility function to be used in the Panels and the GUI stuff
#pragma once
#include <string>
#include "Spike/Scene/Entity.h"
#include <imgui/imgui.h>
#include <glm/glm.hpp>

namespace Spike
{
    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    void DrawComponents(Entity entity);

    template<typename UIFunction>
    void DrawToggleButton(const char* text, ImVec4 color, UIFunction uiFunction)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        if (ImGui::Button(text))
        {
            uiFunction();
        }
        ImGui::PopStyleColor();
    }
}