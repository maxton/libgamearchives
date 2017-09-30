#include "default_directory.h"

namespace gamearchives {

bool DefaultDirectory::GetFile(const std::string& name, std::shared_ptr<IFile>& file) {
  auto it = files_.find(name);
  if (it == files_.end()) {
    file = nullptr;
    return false;
  }
  file = it->second;
  return true;
}

bool DefaultDirectory::GetDir(const std::string& name, std::shared_ptr<IDirectory>& dir) {
  auto it = dirs_.find(name);
  if (it == dirs_.end()) {
    dir = nullptr;
    return false;
  }
  dir = it->second;
  return true;
}

const std::vector<std::string> DefaultDirectory::ListNodes() {
  std::vector<std::string> ret;
  for (auto& d : dirs_) {
    ret.push_back(d.first);
  }
  for (auto& f : files_) {
    ret.push_back(f.first);
  }
  return ret;
}

//const std::vector<std::shared_ptr<IFile>> DefaultDirectory::GetFiles() {
//  std::vector<std::shared_ptr<IFile>> ret;
//  for (auto& f : files_) {
//    ret.push_back(f.second);
//  }
//  return ret;
//}

}