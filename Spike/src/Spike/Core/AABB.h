/*****************************************************************************/
/*                             Spike SourceCode                              */
/*                                                                           */
/* File created by: beastboioioi                                             */
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

#include <glm/glm.hpp>

namespace Spike 
{
    struct AABB
    {
        glm::vec3 Min, Max;

        AABB()
            : Min(0.0f), Max(0.0f)
        {
        }

        AABB(const glm::vec3& min, const glm::vec3& max)
            : Min(min), Max(max)
        {
        }

    };
}