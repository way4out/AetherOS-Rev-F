#pragma once
namespace aether::dsp {
struct Metrics{unsigned rms,peak,dominantBin,energy;};
void init(); void tick(); void analyze(const unsigned char*,unsigned); Metrics metrics();
void fft128(const unsigned char*,float*,float*);
}
