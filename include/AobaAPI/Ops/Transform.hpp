#ifndef AOBA_OPS_TRANSFORM_HPP
#define AOBA_OPS_TRANSFORM_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

void Rotate(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& center, const Math::Mat3& mat);

void Translate(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& vec);

void Scale(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& center, const Math::Vec3& vec);

void Transform(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Mat4 matrix);

} // namespace Ops
} // namespace Aoba

#endif