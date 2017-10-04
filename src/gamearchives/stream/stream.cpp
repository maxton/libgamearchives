#include <sstream>
#include "stream.h"


namespace gamearchives {

Stream::~Stream() = default;

#define SWAP16(x) (uint16_t)((((x) & 0xFF00u) >> 8) | (((x) & 0x00FFu) << 8))
#define SWAP32(x) (SWAP16(((x) & 0xFFFF0000) >> 16) | ((uint32_t)SWAP16((x) & 0xFFFF) << 16))
#define SWAP64(x) (SWAP32(((x) & 0xFFFFFFFF00000000) >> 32) | ((uint64_t)SWAP32((x) & 0xFFFFFFFF) << 32))

uint64_t Stream::ReadUInt64LE() {
  uint64_t result = 0;
  Read((uint8_t*) &result, 8);
  return result;
}
uint64_t Stream::ReadUInt64BE() {
  uint64_t result = ReadUInt64LE();
  return SWAP64(result);
}
int64_t Stream::ReadInt64LE() { return (int64_t) ReadUInt64LE(); }
int64_t Stream::ReadInt64BE() { return (int64_t) ReadUInt64BE(); }

uint32_t Stream::ReadUInt32LE() {
  uint32_t result = 0;
  Read((uint8_t*) &result, 4);
  return result;
}
uint32_t Stream::ReadUInt32BE() {
  uint32_t result = ReadUInt32LE();
  return SWAP32(result);
}
int32_t Stream::ReadInt32LE() { return (int32_t) ReadUInt32LE(); }
int32_t Stream::ReadInt32BE() { return (int32_t) ReadUInt32BE(); }

uint16_t Stream::ReadUInt16LE() {
  uint16_t result = 0;
  Read((uint8_t*) &result, 2);
  return result;
}
uint16_t Stream::ReadUInt16BE() {
  uint16_t result = ReadUInt16LE();
  return SWAP16(result);
}
int16_t Stream::ReadInt16LE() { return (int16_t)ReadUInt16LE(); }
int16_t Stream::ReadInt16BE() { return (int16_t)ReadUInt16BE(); }

uint8_t Stream::ReadUInt8() {
  uint8_t result = 0;
  Read(&result, 1);
  return result;
}
int8_t Stream::ReadInt8() {
  int8_t result = 0;
  Read((uint8_t*) &result, 1);
  return result;
}

std::string Stream::ReadASCIINullTerminated(uint64_t limit) {
  std::stringstream str;
  char c;
  while (str.tellg() < (int64_t)limit && Read((uint8_t*) &c, 1) == 1) {
    if (c == '\0') break;
    str << c;
  }
  return str.str();
}
std::string Stream::ReadString(uint32_t size) {
  std::string s(size, '\0');
  // This is HACKY! Accessing the raw string buffer so we only have to do a
  // single copy. I think it is guaranteed to work as of C++11
  char* raw_buf = &s[0];
  auto real_size = (uint32_t) Read((uint8_t*) raw_buf, size);
  s.resize(real_size);
  return s;
}
}