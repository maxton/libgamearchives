#include "ark.h"
#include "hdr_crypt_stream.h"
#include "gamearchives/vfs/default_directory.h"

namespace gamearchives {

uint32_t TestArkStream(std::shared_ptr<Stream> hdr,
                       std::shared_ptr<Stream>& hdr_stream) {
  auto version = hdr->ReadUInt32LE();
  if (version > HIGHEST_VERSION && version != VERSION_ARK) {
    // hdr is encrypted, probably
    auto decryptor = std::make_shared<HdrCryptStream>(hdr);
    if (decryptor->ReadInt32LE() < 0) {
      decryptor->set_xor(0xFF);
    }
    decryptor->Seek(0, SEEK_SET);
    version = decryptor->ReadUInt32LE();
    hdr_stream = decryptor;
  } else {
    hdr_stream = hdr;
  }
  return version;
}

ArchiveTestResult ArkArchive::TestFile(std::shared_ptr<IFile>& file) {
  std::shared_ptr<Stream> ignore;
  auto version = TestArkStream(file->Open(), ignore);

  return (version >= LOWEST_VERSION && version <= HIGHEST_VERSION)
      || version == VERSION_ARK ? YES : NO;
}
std::shared_ptr<Archive> ArkArchive::LoadFile(
    std::shared_ptr<IFile>& file) {
  return std::shared_ptr<Archive>(new ArkArchive(file));
}

ArkArchive::ArkArchive(std::shared_ptr<IFile> hdrFile)
    : hdr_file_(std::move(hdrFile)) {
  std::shared_ptr<Stream> hdr_stream = nullptr;
  auto hdr = hdrFile->Open();
  auto version = TestArkStream(hdr, hdr_stream);
  if (version == VERSION_ARK) {
    ReadOldArk();
  } else if (version <= HIGHEST_VERSION && version >= LOWEST_VERSION) {
    ReadHeader(hdr_stream, version);
  } else {
    throw "Unrecognized ark header";
  }
}

void ArkArchive::ReadOldArk() {

}

void ArkArchive::ReadHeader(std::shared_ptr<Stream>, uint32_t) {

}

}