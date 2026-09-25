#pragma once
namespace aether::dsp { struct Metrics{unsigned rms,peak,dominantBin,energy;}; void init(); void tick(); void analyze(const unsigned char*,unsigned); Metrics metrics(); }