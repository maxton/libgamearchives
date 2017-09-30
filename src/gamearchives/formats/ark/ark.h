#pragma once

#include "gamearchives/stream/multi_stream.h"
#include "gamearchives/vfs/vfs.h"

namespace gamearchives {

const uint32_t HIGHEST_VERSION = 10;
const uint32_t LOWEST_VERSION = 2;
const uint32_t VERSION_ARK = 0x4B5241;


class ArkArchive : public Archive {
 public:
  static const char * type_name() { return "Ark/Hdr Pair"; }
  static std::vector<std::string> extensions() { return {"hdr","ark"}; }
  static ArchiveTestResult TestFile(std::shared_ptr<IFile>& file);
  static std::shared_ptr<Archive> LoadFile(std::shared_ptr<IFile>& file);
  explicit ArkArchive(std::shared_ptr<IFile> hdrFile);
  const std::string& name() const override { return hdr_file_->name(); }
  std::shared_ptr<IDirectory>& root() override { return root_; }
 private:
  std::shared_ptr<IFile> hdr_file_;
  std::shared_ptr<MultiStream> content_meta_stream_;
  std::shared_ptr<IDirectory> root_;

  void ReadOldArk();
  void ReadHeader(std::shared_ptr<Stream>, uint32_t);
};
ArchiveProxy<ArkArchive> ark_archive_proxy;  /* NOLINT */

}