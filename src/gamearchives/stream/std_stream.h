#pragma once

#include <memory>
#include <fstream>
#include "gamearchives/stream/stream.h"

namespace gamearchives {

/**
 * A stream that wraps the STL's ifstream class
 */
class StdStream : public Stream {
 public:
  explicit StdStream(std::ifstream* stream) : stream_(stream) {
    stream->seekg(0, std::ios_base::end);
    length_ = static_cast<uint64_t>(stream_->tellg());
    stream->seekg(0, std::ios_base::beg);
  }
  ~StdStream() override = default;
  uint64_t Read(uint8_t* buf, uint64_t count) override {
    auto pos = stream_->tellg();
    stream_->read((char*) buf, static_cast<std::streamsize>(count));
    return static_cast<uint64_t>(stream_->tellg() - pos);
  }
  uint64_t Position() const override { return static_cast<uint64_t>(stream_->tellg()); }
  uint64_t Seek(int64_t offset, int whence) override {
    switch (whence) {
      case SEEK_SET:
        stream_->seekg(offset, std::ios_base::beg);
        break;
      case SEEK_CUR:
        stream_->seekg(offset, std::ios_base::cur);
        break;
      case SEEK_END:
        stream_->seekg(offset, std::ios_base::end);
        break;
      default:
        break;
    }
    return static_cast<uint64_t>(stream_->tellg());
  }
  uint64_t Length() const override { return length_; }
 private:
  uint64_t length_;
  std::unique_ptr<std::ifstream> stream_;
};

}