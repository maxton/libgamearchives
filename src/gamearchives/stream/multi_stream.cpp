#include "multi_stream.h"

namespace gamearchives {

uint64_t MultiStream::offsetToStream(uint64_t offset, Stream** stream_out) {
  for (size_t i = 0; i < lengths_.size(); i++) {
    if (lengths_[i] > offset) {
      *stream_out = streams_[i].get();
      return offset;
    }
    offset -= lengths_[i];
  }
  return 0;
}

uint64_t MultiStream::Read(uint8_t* buffer, uint64_t count) {
  if (position_ == length_ || position_ + count > length_) {
    count = length_ - position_;
  }

  uint64_t totalBytesRead = 0;
  while (count > 0) {
    Stream* current = nullptr;
    uint64_t current_position = offsetToStream(position_, &current);
    if (current == nullptr) {
      break;
    }
    current->Seek(current_position, SEEK_SET);
    uint64_t bytesRead = current->Read(buffer, count);
    count -= bytesRead;
    position_ += bytesRead;
    totalBytesRead += bytesRead;
    if (bytesRead == 0) {
      break;
    }
  }
  return totalBytesRead;
}

uint64_t MultiStream::Seek(int64_t offset, int whence) {
  int64_t destination;
  switch (whence) {
    case SEEK_SET:
      destination = offset;
      break;
    case SEEK_CUR:
      destination = position_ + offset;
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