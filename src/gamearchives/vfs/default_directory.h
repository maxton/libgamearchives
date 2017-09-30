#pragma once

#include "gamearchives/vfs/vfs.h"

namespace gamearchives {

class DefaultDirectory : public IDirectory {
 public:
  DefaultDirectory(const std::string& name, IDirectory* parent) : IDirectory(name), parent_(parent) {}

  IDirectory* GetParent() override { return parent_; }
  bool GetFile(const std::string& name, std::shared_ptr<IFile>& file) override;
  bool GetDir(const std::string& name, std::shared_ptr<IDirectory>& dir) override;
  const std::vector<std::string> ListNodes() override;
  //const std::vector<std::shared_ptr<IFile>> GetFiles() override;

  void AddFile(const std::shared_ptr<IFile>& f) { files_[f->name()] = f; }
  void AddDir(const std::shared_ptr<IDirectory>& d) { dirs_[d->name()] = d; }
 protected:
  IDirectory* parent_;
  std::map<std::string, std::shared_ptr<IFile>> files_;
  std::map<std::string, std::shared_ptr<IDirectory>> dirs_;
};

}