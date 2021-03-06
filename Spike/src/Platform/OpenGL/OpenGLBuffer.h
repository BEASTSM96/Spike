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

#include "Spike/Renderer/Buffer.h"

namespace Spike
{
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(void* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();
    
        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;

        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer( void* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();
    
        virtual void Bind() const;
        virtual void Unbind() const;
    
        virtual uint32_t GetCount() const { return m_Count; }
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };

}
