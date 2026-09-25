#pragma once
namespace aether::ai { struct Result{unsigned confidence,pattern,classId;}; void init(); void tick(); void generate(); Result result(); }