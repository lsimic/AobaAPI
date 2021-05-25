#ifndef AOBA_OPS_TRANSFORM_HPP
#define AOBA_OPS_TRANSFORM_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

/// <summary>
///	Rotate the given verts around the given rotation center using the rotation specified by the rotation matrix
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to rotate</param>
/// <param name="center">Center of rotation</param>
/// <param name="mat">rotation matrix</param>
void Rotate(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& center, const Math::Mat3& mat);

/// <summary>
/// Translate the given verts using the given vector.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to translate</param>
/// <param name="vec">Translation vector</param>
void Translate(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& vec);

/// <summary>
/// Scale the given verts around the given center using the scale factors for x,y,z axis defined in the vector
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to rotate</param>
/// <param name="center">Center of scaling</param>
/// <param name="vec">X,Y,Z axis scale factor</param>
void Scale(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& center, const Math::Vec3& vec);

/// <summary>
/// Transform the mesh using the transform matrix. 
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to rotate</param>
/// <param name="matrix">Transform matrix</param>
void Transform(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Mat4 matrix);

} // namespace Ops
} // namespace Aoba

#endif