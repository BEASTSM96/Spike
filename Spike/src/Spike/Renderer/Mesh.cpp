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
#include "spkpch.h"
#include "Mesh.h"

#include "Spike/Core/Log.h"

#include "Buffer.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Spike 
{

    glm::mat4 Mat4FromAssimpMat4( const aiMatrix4x4& matrix )
    {
        glm::mat4 result;
        result[ 0 ][ 0 ] = matrix.a1; result[ 1 ][ 0 ] = matrix.a2; result[ 2 ][ 0 ] = matrix.a3; result[ 3 ][ 0 ] = matrix.a4;
        result[ 0 ][ 1 ] = matrix.b1; result[ 1 ][ 1 ] = matrix.b2; result[ 2 ][ 1 ] = matrix.b3; result[ 3 ][ 1 ] = matrix.b4;
        result[ 0 ][ 2 ] = matrix.c1; result[ 1 ][ 2 ] = matrix.c2; result[ 2 ][ 2 ] = matrix.c3; result[ 3 ][ 2 ] = matrix.c4;
        result[ 0 ][ 3 ] = matrix.d1; result[ 1 ][ 3 ] = matrix.d2; result[ 2 ][ 3 ] = matrix.d3; result[ 3 ][ 3 ] = matrix.d4;
        return result;
    }

    static const uint32_t s_MeshImportFlags =
        aiProcess_CalcTangentSpace  |        // Create binormals/tangents just in case
        aiProcess_Triangulate       |             // Make sure we're triangles
        aiProcess_SortByPType       |             // Split meshes by primitive type
        aiProcess_GenNormals        |              // Make sure we have legit normals
        aiProcess_GenUVCoords       |             // Convert UVs if required 
        aiProcess_OptimizeMeshes    |          // Batch draws where possible
        aiProcess_ValidateDataStructure;

    struct LogStream : public Assimp::LogStream
    {
        static void Initialize()
        {
            if( Assimp::DefaultLogger::isNullLogger() )
            {
                Assimp::DefaultLogger::create( "", Assimp::Logger::VERBOSE );
                Assimp::DefaultLogger::get()->attachStream( new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn );
            }
        }

        virtual void write( const char* message ) override
        {
            SPK_CORE_LOG_WARN( "Assimp error: {0}", message );
        }
    };

    Mesh::Mesh(const std::string& filename) : m_FilePath(filename)
    {
        LogStream::Initialize();
        SPK_CORE_LOG_INFO( "Loading Mesh {0}...", filename.c_str() );

        m_Importer = std::make_unique<Assimp::Importer>();

        const aiScene* scene = m_Importer->ReadFile( filename, s_MeshImportFlags );
        const aiNode* node = scene->mRootNode;

        if( !scene || !scene->HasMeshes() )
            SPK_CORE_LOG_ERROR( "Failed to load mesh file: {0}", filename );

        m_Scene = scene;
        m_IsAnimated = scene->mAnimations != nullptr;

        m_MeshShader = m_ShaderLibrary.Load("Spike-Editor/assets/shaders/PBR_Static.glsl");
        m_InverseTransform = glm::inverse( Mat4FromAssimpMat4( scene->mRootNode->mTransformation ) );

        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;

        m_Submeshes.reserve( scene->mNumMeshes );
        for( size_t m = 0; m < scene->mNumMeshes; m++ )
        {
            aiMesh* mesh = scene->mMeshes[ m ];

            Submesh& submesh = m_Submeshes.emplace_back();
            submesh.BaseVertex = vertexCount;
            submesh.BaseIndex = indexCount;
            submesh.MaterialIndex = mesh->mMaterialIndex;
            submesh.IndexCount = mesh->mNumFaces * 3;
            submesh.MeshName = mesh->mName.C_Str();

            vertexCount += mesh->mNumVertices;
            indexCount += submesh.IndexCount;

            // Vertices
            auto& aabb = submesh.BoundingBox;
            aabb.Min ={ FLT_MAX, FLT_MAX, FLT_MAX };
            aabb.Max ={ -FLT_MAX, -FLT_MAX, -FLT_MAX };
            for( size_t i = 0; i < mesh->mNumVertices; i++ )
            {
                Vertex vertex;
                vertex.Position ={ mesh->mVertices[ i ].x, mesh->mVertices[ i ].y, mesh->mVertices[ i ].z };
                vertex.Normal ={ mesh->mNormals[ i ].x, mesh->mNormals[ i ].y, mesh->mNormals[ i ].z };
                aabb.Min.x = glm::min( vertex.Position.x, aabb.Min.x );
                aabb.Min.y = glm::min( vertex.Position.y, aabb.Min.y );
                aabb.Min.z = glm::min( vertex.Position.z, aabb.Min.z );
                aabb.Max.x = glm::max( vertex.Position.x, aabb.Max.x );
                aabb.Max.y = glm::max( vertex.Position.y, aabb.Max.y );
                aabb.Max.z = glm::max( vertex.Position.z, aabb.Max.z );

                if( mesh->HasTangentsAndBitangents() )
                {
                    vertex.Tangent ={ mesh->mTangents[ i ].x, mesh->mTangents[ i ].y, mesh->mTangents[ i ].z };
                    vertex.Binormal ={ mesh->mBitangents[ i ].x, mesh->mBitangents[ i ].y, mesh->mBitangents[ i ].z };
                }
                if( mesh->HasTextureCoords( 0 ) )
                    vertex.Texcoord ={ mesh->mTextureCoords[ 0 ][ i ].x, mesh->mTextureCoords[ 0 ][ i ].y };

                m_StaticVertices.push_back( vertex );
            }

            // Indices
            for( size_t i = 0; i < mesh->mNumFaces; i++ )
            {
                SPK_CORE_ASSERT( mesh->mFaces[ i ].mNumIndices == 3, "Must have 3 indices." );
                Index index ={ mesh->mFaces[ i ].mIndices[ 0 ], mesh->mFaces[ i ].mIndices[ 1 ], mesh->mFaces[ i ].mIndices[ 2 ] };
                m_Indices.push_back( index );

                if( !m_IsAnimated )
                    m_TriangleCache[ m ].emplace_back( m_StaticVertices[ index.V1 + submesh.BaseVertex ], m_StaticVertices[ index.V2 + submesh.BaseVertex ], m_StaticVertices[ index.V3 + submesh.BaseVertex ] );
            }

            TraverseNodes( scene->mRootNode );

            // Materials
            if( scene->HasMaterials() )
            {
                SPK_CORE_LOG_INFO( "---- Materials - {0} ----", filename );
            }

            BufferLayout layout;
            m_VertexBuffer = VertexBuffer::Create( m_StaticVertices.data(), m_StaticVertices.size() * sizeof( Vertex ) );
            layout ={
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float3, "a_Normal" },
                { ShaderDataType::Float3, "a_Tangent" },
                { ShaderDataType::Float3, "a_Binormal" },
                { ShaderDataType::Float2, "a_TexCoord" },
            };

            m_IndexBuffer = IndexBuffer::Create( m_Indices.data(), m_Indices.size() * sizeof( Index ) );

            DumpVertexBuffer();
        }
    }

    Mesh::~Mesh()
    {
        SPK_CORE_LOG_INFO( "~mesh()" );
    }

    void Mesh::TraverseNodes( aiNode* node, const glm::mat4& parentTransform /*= glm::mat4( 1.0f )*/, uint32_t level /*= 0 */ )
    {
        glm::mat4 transform = parentTransform * Mat4FromAssimpMat4( node->mTransformation );
        for( uint32_t i = 0; i < node->mNumMeshes; i++ )
        {
            uint32_t mesh = node->mMeshes[ i ];
            auto& submesh = m_Submeshes[ mesh ];
            submesh.NodeName = node->mName.C_Str();
            submesh.Transform = transform;
        }

        for( uint32_t i = 0; i < node->mNumChildren; i++ )
            TraverseNodes( node->mChildren[ i ], transform, level + 1 );

    }

    void Mesh::OnUpdate( Timestep ts )
    {

    }

    void Mesh::DumpVertexBuffer( void )
    {
        // TODO: Convert to ImGui
        SPK_CORE_LOG_INFO( "------------------------------------------------------" );
        SPK_CORE_LOG_INFO( "Vertex Buffer Dump" );
        SPK_CORE_LOG_INFO( "Mesh: {0}", m_FilePath );
        
        for( size_t i = 0; i < m_StaticVertices.size(); i++ )
        {
            auto& vertex = m_StaticVertices[ i ];
            SPK_CORE_LOG_INFO( "Vertex: {0}", i );
            SPK_CORE_LOG_INFO( "Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z );
            SPK_CORE_LOG_INFO( "Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z );
            SPK_CORE_LOG_INFO( "Binormal: {0}, {1}, {2}", vertex.Binormal.x, vertex.Binormal.y, vertex.Binormal.z );
            SPK_CORE_LOG_INFO( "Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z );
            SPK_CORE_LOG_INFO( "TexCoord: {0}, {1}", vertex.Texcoord.x, vertex.Texcoord.y );
            SPK_CORE_LOG_INFO( "--" );
        }
        
        SPK_CORE_LOG_INFO( "------------------------------------------------------" );
    }
}