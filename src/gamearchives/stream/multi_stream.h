#pragma once

#include <memory>
#include <vector>
#include "stream.h"

namespace gamearchives {
/**
 * A stream that acts as the concatenation of multiple streams.
 */
class MultiStream : public Stream {
 public:
  /**
   * Constructs a MultiStream from a vector of base streams.
   * The streams must have valid Length defined!
   * @param streams
   */
  explicit MultiStream(std::vector<std::shared_ptr<Stream>> streams)
      : streams_(streams), position_(0) {
    length_ = 0;
    for (auto& stream : streams) {
      lengths_.push_back(stream->Length());
      length_ += stream->Length();
    }
  }
  ~MultiStream() override = default;
  uint64_t Read(uint8_t* buf, uint64_t count) override;
  uint64_t Position() const override { return position_; }
  uint64_t Seek(int64_t offset, int whence) override;
  uint64_t Length() const override { return length_; }
 private:
  uint64_t offsetToStream(uint64_t offset, Stream** stream_out);
  uint64_t length_, position_;
  std::vector<std::shared_ptr<Stream>> streams_;
  std::vector<uint64_t> lengths_;
};

}