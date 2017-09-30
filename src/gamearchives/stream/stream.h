#pragma once

#include <cinttypes>
#include <string>

namespace gamearchives {

#ifndef SEEK_CUR
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

/**
 * Abstract base class for streams (file data)
 */
class Stream {
 public:
  virtual ~Stream();
  /**
   * Read `count` bytes from stream into the buffer.
   * @param buf Location to write bytes into
   * @param count Number of bytes to read from the stream
   * @return number of bytes actually copied to buf (guaranteed to be <= `count`)
   */
  virtual uint64_t Read(uint8_t* buf, uint64_t count) = 0;
  /**
   * Returns the current offset from the start of the stream.
   * @return the position of the stream
   */
  virtual uint64_t Position() const = 0;
  /**
   * Seeks the stream to the given offset. This function acts like fseek() in the C stdlib.
   *    SEEK_SET: Offset is from the beginning of the stream
   *    SEEK_CUR: Offset is from the current position
   *    SEEK_END: Offset is from the end of the stream
   * The resulting stream position is returned. You should check that value to make sure you actually seeked.
   * Non-seekable streams will just return the current position.
   * @param offset
   * @param whence SEEK_SET (0), SEEK_CUR (1), or SEEK_END (2)
   * @return the position of the stream after seeking
   */
  virtual uint64_t Seek(int64_t offset, int whence) = 0;
  /**
   * Returns the length of the stream.
   * TODO: what happens for streams of indeterminate length
   * @return the length of the stream
   */
  virtual uint64_t Length() const= 0;
  // The following functions return unconditionally, even if EOF is reached (in which case, the behaviour is undefined)
  uint64_t ReadUInt64LE();
  uint64_t ReadUInt64BE();
  int64_t ReadInt64LE();
  int64_t ReadInt64BE();
  uint32_t ReadUInt32LE();
  uint32_t ReadUInt32BE();
  int32_t ReadInt32LE();
  int32_t ReadInt32BE();
  uint16_t ReadUInt16LE();
  uint16_t ReadUInt16BE();
  int16_t ReadInt16LE();
  int16_t ReadInt16BE();
  uint8_t ReadUInt8();
  int8_t ReadInt8();
  std::string ReadASCIINullTerminated(uint64_t limit);
  std::string ReadString(uint32_t size);
};

}