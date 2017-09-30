#include "gamearchives/stream/offset_stream.h"
#include "offset_file.h"

namespace gamearchives {

std::shared_ptr<Stream> OffsetFile::Open() {
  return std::make_shared<OffsetStream>(archive_.lock(), offset_, size_);
}

}

