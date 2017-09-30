#include "gamearchives/stream/std_stream.h"
#include "gamearchives/vfs/local_file.h"

namespace gamearchives {

LocalFile::LocalFile(const std::string& path, IDirectory* parent)
    : IFile("", 0, false, 0), path_(path), parent_(parent) {

}
std::shared_ptr<Stream> LocalFile::Open() {
  return std::make_shared<StdStream>(new std::ifstream(path_));
}
IDirectory* LocalFile::GetParent() {
  return parent_;
}

}