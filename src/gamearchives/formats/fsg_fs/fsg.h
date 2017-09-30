#pragma once

#include "gamearchives/vfs/vfs.h"
#include "gamearchives/stream/stream.h"

namespace gamearchives {

class FSGFileSystem : public Archive {
 public:
  static const char* type_name() { return "FSG-FILE-SYSTEM"; }
  static std::vector<std::string> extensions() {
    return {"img", "img.part000", "img.part0"};
  }
  static bool TestFile(std::shared_ptr<IFile>& file);
  static std::shared_ptr<Archive> LoadFile(std::shared_ptr<IFile>& file);
  const std::string& name() const override;
  std::shared_ptr<IDirectory>& root() override;
 private:
  FSGFileSystem(std::shared_ptr<IFile> primary_part_);
};

}