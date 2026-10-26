#include "graphics/host_gpu/renderer/image/imageInfo.h"

#include <cstdio>
#include <cstdlib>

namespace {
void Check(bool passed, const char *message) {
  if (!passed) {
    std::fprintf(stderr, "FAIL: %s\n", message);
    std::exit(1);
  }
}
} // namespace

int main() {
  using Libs::Graphics::ClassifyVideoOutCompression;
  using Libs::Graphics::VideoOutCompression;
  Check(
      ClassifyVideoOutCompression(true, 0x215e4c000ull, 0x48, 0xff040304ull) ==
          VideoOutCompression::Dcc256_256_0,
      "PPSA38510 DCC buffer accepts its nonzero clear register");
  Check(ClassifyVideoOutCompression(true, 0x10000, 0x208,
                                    0x1122334455667788ull) ==
            VideoOutCompression::Dcc256_64_64,
        "alternate supported DCC mode accepts nonzero clear register");
  Check(ClassifyVideoOutCompression(true, 0x10000, 0x48, 0) ==
            VideoOutCompression::Dcc256_256_0,
        "existing zero-clear compressed buffers remain supported");
  Check(ClassifyVideoOutCompression(true, 0, 0x48, 1) ==
            VideoOutCompression::Unsupported,
        "missing DCC metadata is still rejected");
  Check(ClassifyVideoOutCompression(true, 0x10001, 0x48, 1) ==
            VideoOutCompression::Unsupported,
        "unaligned DCC metadata is still rejected");
  Check(ClassifyVideoOutCompression(true, 0x10000, 0x49, 1) ==
            VideoOutCompression::Unsupported,
        "unknown DCC control remains unsupported");
  Check(ClassifyVideoOutCompression(false, 0, 0, 0) ==
            VideoOutCompression::Uncompressed,
        "plain uncompressed surfaces remain supported");
  Check(ClassifyVideoOutCompression(false, 0, 0, 1) ==
            VideoOutCompression::Unsupported,
        "DCC clear register on uncompressed surface remains rejected");
  Check(ClassifyVideoOutCompression(false, 0x10000, 0x48, 1) ==
            VideoOutCompression::Unsupported,
        "compressed data cannot be silently reclassified as uncompressed");
  std::puts(
      "PASS: video-out DCC clear register regression and validation guards");
  return 0;
}
