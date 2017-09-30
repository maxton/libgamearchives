#include "offset_stream.h"

namespace gamearchives {

uint64_t OffsetStream::Read(uint8_t* buf, uint64_t count) {
  stream_->Seek(offset_ + position_, SEEK_SET);
  if (count + position_ > length_) {
    count = length_ - position_;
  }
  auto bytes_read = stream_->Read(buf, count);
  position_ += bytes_read;
  return bytes_read;
}

uint64_t OffsetStream::Seek(int64_t offset, int whence) {
  int64_t destination;
  switch (whence) {
    case SEEK_CUR:
      destination = position_ + offset;
      break;
    case SEEK_SET:
      destination = offset;
      break;
    case SEEK_END:
      destination = length_ + offset;
      break;
    default:
      return position_;
  }
  if (destination > (int64_t) length_) {
    destination = length_;
  } else if (destination < 0) {
    destination = 0;
  }
  position_ = (uint64_t)destination;
  return position_;
}
}
