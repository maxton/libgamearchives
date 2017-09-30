#include "hdr_crypt_stream.h"

namespace gamearchives {

uint64_t HdrCryptStream::Read(uint8_t *buffer, uint64_t count) {
  base_->Seek(position_ + 4, SEEK_SET);
  if (position_ == length_ || position_ + count > length_) {
    count = length_ - position_;
  }

  auto bytesRead = base_->Read(buffer, count);

  for (size_t i = 0; i < bytesRead; i++) {
    buffer[i] ^= (uint8_t) (current_key_ ^ xor_);
    position_++;
    UpdateKey();
  }
  return bytesRead;
}

void HdrCryptStream::UpdateKey() {
  if (key_position_ == position_)
    return;
  if (key_position_ > position_) {
    key_position_ = 0;
    current_key_ = key_;
  }
  while (key_position_ < position_) {
    current_key_ = CryptRound(current_key_);
    key_position_++;
  }
}

int32_t HdrCryptStream::CryptRound(int32_t val) {
  int32_t ret = (key_ - ((key_ / 0x1F31D) * 0x1F31D)) * 0x41A7 - (key_ / 0x1F31D) * 0xB14;
  if (ret <= 0)
    ret += 0x7FFFFFFF;
  return ret;
}

}