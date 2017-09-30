#pragma once

#include "gamearchives/vfs/vfs.h"

namespace gamearchives {

class LocalFile : public IFile {
 public:
  explicit LocalFile(const std::string& path, IDirectory* parent_ = nullptr);
  ~LocalFile() override = default;
  std::shared_ptr<Stream> Open() override;
  IDirectory* GetParent() override;
 private:
  std::string path_;
  IDirectory* parent_;
};

}