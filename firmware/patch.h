// Copyright 2018 Patrick Dowling
//
// Author: Patrick Dowling (pld@gurkenkiste.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//

#ifndef OCF4_PATCH_H_
#define OCF4_PATCH_H_

#include "ocf4.h"
#include "io_frame.h"
#include "ui/debug_menu.h"
#include "util/util_memory_pool.h"
#include <type_traits>

namespace ocf4 {

using PatchMemoryPool = stm32x::MemoryPool<kPatchPoolSize>;

class Processor {
public:
  virtual void Init(PatchMemoryPool &memory_pool) = 0;
  virtual void Process(IOFrame &io_frame) = 0;
};

class Patch : public Debuggable {
public:
  DISALLOW_COPY_AND_ASSIGN(Patch);
  Patch() { Init(); }
  ~Patch() { }

  void Init();
  void Reset();
  void Process(IOFrame &io_frame);

  virtual void DebugView(Display::Frame &) const final;

  inline void enable(bool e) {
    enabled_ = e;
  }

  inline bool enabled() const {
    return enabled_;
  }

  template <typename T>
  T* AddProcessor() {
    static_assert(std::is_convertible<T*, Processor*>::value, "Invalid processor type");
    static_assert(T::type_id, "Invalid processor type_id");
    auto &slot = processors_[num_processors_++];

    size_t pool_available = memory_pool_.available();
    slot.type_id = T::type_id;
    slot.buffer = memory_pool_.Alloc(sizeof(T));
    new (slot.buffer) T;
    slot->Init(memory_pool_);
    slot.used = pool_available - memory_pool_.available();

    return reinterpret_cast<T*>(slot.buffer);
  }

private:
  bool enabled_ = false;

  struct ProcessorSlot {
    uint32_t type_id = 0;
    size_t used = 0;
    uint8_t *buffer = nullptr;

    Processor* operator -> () {
      return reinterpret_cast<Processor *>(buffer);
    }

    const Processor* operator -> () const {
      return reinterpret_cast<const Processor *>(buffer);
    }

    bool valid() const {
      return type_id && buffer;
    }

    void Reset() {
      type_id = 0;
      used = 0;
      buffer = nullptr;
    }
  };

  std::array<ProcessorSlot, kNumProcessorSlots> processors_;
  size_t num_processors_ = 0;

  PatchMemoryPool memory_pool_;
};

}; // namespace ocf4

#endif // OCF4_PATCH_H_
