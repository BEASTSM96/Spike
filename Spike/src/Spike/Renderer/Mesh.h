/*****************************************************************************/
/*                        Spike SourceCode                                   */
/*                                                                           */
/* File created by: Fahim Fuad                                               */
/* Other editors: beastboioioi                                               */
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

#include "Spike/Core/AABB.h"
#include "Spike/Core/Timestep.h"
#include "Shader.h"
#include "Buffer.h"

#include <glm/glm.hpp>

//------ Assimp Stuff ------
struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp {
    class Importer;
}

namespace Spike {

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Binormal;
        glm::vec2 Texcoord;
    };

    struct AnimatedVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Binormal;
        glm::vec2 Texcoord;

        uint32_t IDs[ 4 ] ={ 0, 0,0, 0 };
        float Weights[ 4 ]{ 0.0f, 0.0f, 0.0f, 0.0f };

        void AddBoneData( uint32_t BoneID, float Weight )
        {
            for( size_t i = 0; i < 4; i++ )
            {
                if( Weights[ i ] == 0.0 )
                {
                    IDs[ i ] = BoneID;
                    Weights[ i ] = Weight;
                    return;
                }
            }

            SPK_CORE_ASSERT( "Vertex has more than four bones/weights affecting it, extra data will be discarded (BoneID={0}, Weight={1})", BoneID, Weight );
        }
    };

    static const int NumAttributes = 5;

    struct Index
    {
        uint32_t V1, V2, V3;
    };

    static_assert( sizeof( Index ) == 3 * sizeof( uint32_t ) );

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;
    };

    struct VertexBoneData
    {
        uint32_t IDs[ 4 ];
        float Weights[ 4 ];

        VertexBoneData()
        {
            memset( IDs, 0, sizeof( IDs ) );
            memset( Weights, 0, sizeof( Weights ) );
        };

        void AddBoneData( uint32_t BoneID, float Weight )
        {
            for( size_t i = 0; i < 4; i++ )
            {
                if( Weights[ i ] == 0.0 )
                {
                    IDs[ i ] = BoneID;
                    Weights[ i ] = Weight;
                    return;
                }
            }

            SPK_CORE_ASSERT( false, "Too many bones!" );
        }
    };


    struct Triangle
    {
        Vertex V0, V1, V2;

        Triangle( const Vertex& v0, const Vertex& v1, const Vertex& v2 )
            : V0( v0 ), V1( v1 ), V2( v2 )
        {
        }
    };

    class Submesh
    {
    public:
        uint32_t BaseVertex;
        uint32_t BaseIndex;
        uint32_t MaterialIndex;
        uint32_t IndexCount;

        glm::mat4 Transform;
        AABB BoundingBox;

        std::string NodeName, MeshName;
    };

    class Mesh
    {
    public:
        Mesh( const std::string& filename );
        ~Mesh();

        void OnUpdate( Timestep ts );
        void DumpVertexBuffer( void );

        const std::string& GetFilePath() const { return m_FilePath; }
        std::string m_FilePath;
    protected:
        std::unique_ptr<Assimp::Importer> m_Importer;

        void TraverseNodes( aiNode* node, const glm::mat4& parentTransform = glm::mat4( 1.0f ), uint32_t level = 0 );

    private:
        const aiScene* m_Scene;
 
        ShaderLibrary m_ShaderLibrary;
        Ref<Shader> m_MeshShader;
        glm::mat4 m_InverseTransform;

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        std::vector<Index> m_Indices;
        std::vector<Vertex> m_StaticVertices;
        std::vector<Submesh> m_Submeshes;

        std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

        bool m_IsAnimated = false;
    };


}