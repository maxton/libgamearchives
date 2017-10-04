#pragma once

#include <memory>
#include "stream.h"

namespace gamearchives {

/**
 * A stream that acts as a "window" into another stream.
 */
class OffsetStream : public Stream {
 public:
  OffsetStream(std::shared_ptr<Stream> stream, uint64_t offset, uint64_t length)
      : stream_(std::move(stream)),
        offset_(offset),
        length_(length),
        position_(0) {}
  ~OffsetStream() override = default;
  uint64_t Read(uint8_t* buf, uint64_t count) override;
  uint64_t Position() const override { return position_; }
  uint64_t Seek(int64_t offset, int whence) override;
  uint64_t Length() const override { return length_; }
 private:
  std::shared_ptr<Stream> stream_;
  uint64_t offset_, length_, position_;
};

}