// Copyright 2020 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HEAP_CPPGC_GARBAGE_COLLECTOR_H_
#define V8_HEAP_CPPGC_GARBAGE_COLLECTOR_H_

#include "src/heap/cppgc/marker.h"
#include "src/heap/cppgc/sweeper.h"

namespace cppgc {
namespace internal {

// GC interface that allows abstraction over the actual GC invocation. This is
// needed to mock/fake GC for testing.
class GarbageCollector {
 public:
  struct Config {
    using StackState = cppgc::Heap::StackState;
    using MarkingType = Marker::MarkingConfig::MarkingType;
    using SweepingType = Sweeper::Config;

    static constexpr Config ConservativeAtomicConfig() {
      return {StackState::kMayContainHeapPointers, MarkingType::kAtomic,
              SweepingType::kAtomic};
    }

    static constexpr Config PreciseAtomicConfig() {
      return {StackState::kNoHeapPointers, MarkingType::kAtomic,
              SweepingType::kAtomic};
    }

    StackState stack_state = StackState::kMayContainHeapPointers;
    MarkingType marking_type = MarkingType::kAtomic;
    SweepingType sweeping_type = SweepingType::kAtomic;
  };

  // Executes a garbage collection specified in config.
  virtual void CollectGarbage(Config config) = 0;

  // The current epoch that the GC maintains. The epoch is increased on every
  // GC invocation.
  virtual size_t epoch() const = 0;
};

}  // namespace internal
}  // namespace cppgc

#endif  // V8_HEAP_CPPGC_GARBAGE_COLLECTOR_H_
