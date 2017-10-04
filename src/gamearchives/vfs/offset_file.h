#pragma once

#include <utility>

#include "gamearchives/stream/stream.h"
#include "gamearchives/vfs/vfs.h"

namespace gamearchives {

class OffsetFile : public IFile {
 public:
  OffsetFile(const std::string& name, const std::shared_ptr<Stream>& archive, IDirectory* parent,
             uint64_t size, uint64_t offset)
      : IFile(name, size, false, size), archive_(archive), offset_(offset), parent_(parent) {}
  IDirectory* GetParent() override { return parent_; }
  std::shared_ptr<Stream> Open() override;
 private:
  std::weak_ptr<Stream> archive_;
  uint64_t offset_;
  IDirectory* parent_;
};

}