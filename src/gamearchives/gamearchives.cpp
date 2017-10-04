#include "gamearchives.h"
#include "gamearchives/vfs/local_file.h"

namespace gamearchives {

std::shared_ptr<Archive> LoadArchiveFromFile(std::shared_ptr<IFile> f) {
  std::vector<ArchiveProxyBase*> possible;

  for (auto type : archive_types) {
    auto result = type->TestFile(f);
    if (result == YES) {
      return type->LoadFile(f);
    }
    else if(result == MAYBE) {
      possible.push_back(type);
    }
  }
  for (auto type : possible) {
    try {
      return type->LoadFile(f);
    }
    catch(std::exception&) {
      continue;
    }
  }
  throw "Given file was not a supported archive format.";
}

std::shared_ptr<Archive> LoadArchiveFromFile(const std::string& f) {
  return LoadArchiveFromFile(std::shared_ptr<IFile>(new LocalFile(f)));
}

}