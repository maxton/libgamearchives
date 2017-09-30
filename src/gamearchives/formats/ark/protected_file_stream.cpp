#include "protected_file_stream.h"

namespace gamearchives {
uint64_t ProtectedFileStream::Seek(int64_t offset, int whence) {
  int64_t destination;
  switch (whence) {
    case SEEK_CUR:destination = position_ + offset;
      break;
    case SEEK_SET:destination = offset;
      break;
    case SEEK_END:destination = length_ + offset;
      break;
    default:return position_;
  }
  if (destination > (int64_t) length_) {
    destination = length_;
  } else if (destination < 0) {
    destination = 0;
  }
  position_ = (uint64_t)destination;
  if (position_ > 0) {
    base_->Seek(position_ - 1, SEEK_SET);
    current_key_ = (uint8_t) ((key_ ^ base_->ReadUInt8()) - position_ + 1);
  } else {
    current_key_ = key_;
  }
  return position_;
}

uint64_t ProtectedFileStream::Read(uint8_t* buffer, uint64_t count) {
  base_->Seek(position_, SEEK_SET);
  if (count + position_ > length_) {
    count = length_ - position_;
  }
  auto bytes_read = base_->Read(buffer, count);
  for (auto i = 0ULL; i < bytes_read; i++) {
    uint8_t tmp = buffer[i];
    buffer[i] ^= current_key_;
    current_key_ = (uint8_t) ((key_ ^ tmp) - position_);
    position_++;
  }
  return bytes_read;
}

#define BYTE(num, value) ((uint8_t)((value) >> ((num) * 8)))
#define FOLD(x) ((uint32_t)(BYTE(0, (x)) + BYTE(1, (x)) + BYTE(2, (x)) + BYTE(3, (x))))

uint32_t Mangle(const uint8_t bytes[], int offset, int count) {
  auto mangled = 0U;
  for (int i = 0; i < count; i++) {
    mangled = bytes[offset + i] ^ 2 * (bytes[offset + i] + mangled);
  }
  return mangled;
}

uint32_t rotl(uint32_t value, int count) {
  const unsigned int bits = sizeof(value) * 8;
  count %= bits;
  return (value << count) | (value >> (bits - count));
}

uint32_t Hash(uint8_t key[], int offset, int count) {
  uint32_t tmp;

  uint8_t counter = 0;
  uint32_t seed = 0xE3AFEC21;
  for (int i = 0L; i < count; i++) {
    tmp = key[offset + i] ^ FOLD(seed);
    key[offset + i] = (uint8_t) tmp;
    seed = rotl((tmp | ((tmp | ((tmp | (tmp << 8)) << 8)) << 8)) + rotl(seed, tmp & 0x1F), 1);
    if (counter > 16) {
      seed <<= 1;
      counter = 0;
    }
    counter++;
  }
  return seed;
}

uint8_t ProtectedFileStream::CalculateKey(uint8_t metadata[]) {
  uint16_t word_0xE = *(uint16_t*) (metadata + 0xE);

  auto mangled = (uint8_t) FOLD(
      Mangle(metadata, 4, 9) +
          Mangle(metadata, 0, 4) +
          Mangle(metadata, 13, 1) +
          Mangle(metadata, 16, 4) +
          Mangle(metadata, 24, word_0xE));

  Hash(metadata, 24, word_0xE);
  Hash(metadata, 13, 1);
  Hash(metadata, 16, 4);
  Hash(metadata, 0, 4);
  Hash(metadata, 4, 9);

  return (uint8_t) (metadata[5] ^ mangled);
}

}