#ifndef AOBA_IO_EXPORT_OBJ_HPP
#define AOBA_IO_EXPORT_OBJ_HPP

#include "../Core.hpp"

#include <string>

namespace Aoba {
namespace IO {

/// <summary>
/// Export the given mesh into a text-based obj file stored at the given path.
/// </summary>
/// <remarks>
/// The functinality is currently rather limited.
/// </remarks>
/// <param name="path">File path</param>
/// <param name="mesh">Mesh to export</param>
void ExportObj(std::string path, Core::Mesh* mesh);

} // namespace IO
} // namespace Aoba

#endif