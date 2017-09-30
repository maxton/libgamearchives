#pragma once

#include "gamearchives/vfs/vfs.h"

namespace gamearchives {

std::shared_ptr<Archive> LoadArchiveFromFile(std::shared_ptr<IFile> f);
std::shared_ptr<Archive> LoadArchiveFromFile(const std::string& path);

}