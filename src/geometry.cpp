#include "geometry.h"

#include "util/util.h"

#include <string>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


bool LoadObjFile(const char* inputFile, std::vector<VertexPosNormal>& vertices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputFile, nullptr, true, false);

    if (!result || (attrib.vertices.size() % 3) != 0 || attrib.vertices.size() < 9)
    {
        return false;
    }

    // we want to normalize the mesh to center (0, 0, 0) and max extent [-0.5, 0.5] in each dimension
    Vec3 minPos(attrib.vertices[0], attrib.vertices[1], attrib.vertices[2]);
    Vec3 maxPos(attrib.vertices[0], attrib.vertices[1], attrib.vertices[2]);
    for (size_t i = 0; i < attrib.vertices.size() / 3; ++i)
    {
        Vec3 pos = Vec3(attrib.vertices[i * 3], attrib.vertices[i * 3 + 1], attrib.vertices[i * 3 + 2]);

        minPos = Vec3::Min(minPos, pos);
        maxPos = Vec3::Max(maxPos, pos);
    }

    Vec3 center = (minPos + maxPos) * 0.5f;

    Vec3 extent = maxPos - minPos;
    float maxDimExtent = std::max(std::max(extent.x, extent.y), extent.z);
    float scaleFactor = 1.f / maxDimExtent;

    // compute vertex count
    size_t vertexCount = 0;
    for (auto& shape : shapes)
    {
        for (size_t faceVertices : shape.mesh.num_face_vertices)
        {
            // we only support triangle meshes
            if (faceVertices != 3)
            {
                return false;
            }

            vertexCount += 3;
        }
    }

    // we don't bother to re-use vertices and create an index buffer, we just use a vertex list
    vertices.resize(vertexCount);

    size_t currentVertex = 0;
    for (auto& shape : shapes)
    {
        for (size_t face = 0; face < shape.mesh.num_face_vertices.size(); ++face)
        {
            // get coordinates af all face vertices
            tinyobj::index_t indices[3] = { shape.mesh.indices[face * 3], shape.mesh.indices[face * 3 + 1], shape.mesh.indices[face * 3 + 2] };

            Vec3 positions[3];
            for (size_t i = 0; i < 3; ++i)
            {
                // normalize the positions
                positions[i] = (Vec3(
                    attrib.vertices[static_cast<size_t>(indices[i].vertex_index) * 3],
                    attrib.vertices[static_cast<size_t>(indices[i].vertex_index) * 3 + 1],
                    attrib.vertices[static_cast<size_t>(indices[i].vertex_index) * 3 + 2]
                ) - Vec3(center)) * scaleFactor;
            }

            // we compute the face normals as a fallback if no normals are given for a vertex
            Vec3 faceNormal = Vec3::Normalize(Vec3::Cross(positions[1] - positions[0], positions[2] - positions[0]));

            Vec3 normals[3];
            for (size_t i = 0; i < 3; ++i)
            {
                if (indices[i].normal_index == -1)
                {
                    normals[i] = faceNormal;
                }
                else
                {
                    normals[i] = Vec3::Normalize(Vec3(
                        attrib.normals[static_cast<size_t>(indices[i].normal_index) * 3],
                        attrib.normals[static_cast<size_t>(indices[i].normal_index) * 3 + 1],
                        attrib.normals[static_cast<size_t>(indices[i].normal_index) * 3 + 2]
                    ));
                }
            }

            for (size_t i = 0; i < 3; ++i)
            {
                vertices[currentVertex++] = VertexPosNormal{ positions[i].x, positions[i].y, positions[i].z, normals[i].x, normals[i].y, normals[i].z };
            }
        }
    }

    return true;
}

