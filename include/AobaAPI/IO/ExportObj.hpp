#ifndef AOBA_IO_EXPORT_OBJ_HPP
#define AOBA_IO_EXPORT_OBJ_HPP

#include "../Core.hpp"

#include <string>

namespace Aoba {
namespace IO {

void ExportObj(std::string path, Core::Mesh* mesh);

}
} // namespace Aoba

#endif