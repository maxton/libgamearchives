#pragma once

#include <memory>

#include "gamearchives/stream/stream.h"

namespace gamearchives {
class HdrCryptStream : public Stream {
 public:
  explicit HdrCryptStream(std::shared_ptr<Stream> base)
      : base_(std::move(base)), xor_(0), key_position_(0), position_(0) {
    base->Seek(0, SEEK_CUR);
    // The initial key is found in the first 4 bytes.
    current_key_ = key_ = CryptRound(base->ReadInt32LE());
    length_ = base->Length() - 4;
  }
  ~HdrCryptStream() override = default;
  uint64_t Read(uint8_t* buf, uint64_t count) override;
  uint64_t Position() const override { return position_; }
  uint64_t Seek(int64_t offset, int whence) override {
    int adjust = (whence == SEEK_CUR) || (whence == SEEK_END) ? 0 : 4;
    position_ = base_->Seek(offset + adjust, whence) - 4;
    UpdateKey();
    return position_;
  }
  uint64_t Length() const override { return length_; }
  void set_xor(uint8_t val) { xor_ = val; }
 private:
  void UpdateKey();
  int32_t CryptRound(int32_t val);
  std::shared_ptr<Stream> base_;
  uint64_t position_, key_position_, length_;
  int32_t key_, current_key_;
  uint8_t xor_;
};

}