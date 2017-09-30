#pragma once

#include <memory>
#include "gamearchives/stream/stream.h"

namespace gamearchives {

class ProtectedFileStream : public Stream {
 public:
  explicit ProtectedFileStream(const std::shared_ptr<Stream> &base) : base_(base), position_(0), key_position_(0) {
    base->Seek(-36, SEEK_END);
    auto size = base->ReadUInt32LE();
    if (size > 0x40) { // I've only seen up to 0x3C
      throw "Protected file footer was bigger than expected";
    }
    base->Seek(-size, SEEK_END);
    uint8_t metadata[size];
    base->Read(metadata, size);
    current_key_ = key_ = CalculateKey(metadata);
    length_ = base->Length() - size;
  }
  ~ProtectedFileStream() override = default;
  uint64_t Read(uint8_t* buf, uint64_t count) override;
  uint64_t Position() const override { return position_; }
  uint64_t Seek(int64_t offset, int whence) override;
  uint64_t Length() const override { return length_; }
 private:
  static uint8_t CalculateKey(uint8_t metadata[]);
  std::shared_ptr<Stream> base_;
  uint64_t position_, key_position_, length_;
  uint8_t key_, current_key_;
};

}