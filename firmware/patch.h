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
// NOTE It might be possible to template the slot types such that they are
// concrete processor types, rather than interfaces.

#ifndef OCF4_PATCH_H_
#define OCF4_PATCH_H_

#include "ocf4.h"
#include "processor.h"
#include "io_frame.h"
#include "builtin/debug_menu.h"
#include <type_traits>

namespace ocf4 {

class Patch : public Debuggable {
public:
  DISALLOW_COPY_AND_ASSIGN(Patch);
  Patch(PatchMemoryPool &memory_pool) : memory_pool_(memory_pool) { Init();}
  ~Patch() { }

  void Init();
  void Reset();
  void Process(IOFrame &io_frame);
  void IdleLoop();

  virtual void DebugView(Display::Frame &) const final;

  inline void enable(bool e) {
    enabled_ = e;
  }

  inline bool enabled() const {
    return enabled_ && num_processors_;
  }

  Menu *root_menu() {
    return root_menu_.get();
  }

  template <typename T, typename... Ts>
  T* AddProcessor(Ts && ...params) {
    static_assert(std::is_convertible<T*, Processor*>::value, "Invalid processor type");
    static_assert(T::type_id, "Invalid processor type_id");
    auto &slot = processors_[num_processors_++];

    size_t pool_available = memory_pool_.available();
    auto buffer = memory_pool_.Alloc(sizeof(T));
    auto t = new (buffer) T{params...};
    slot.type_id = T::type_id;
    slot.instance = t;
    slot->Init(memory_pool_);
    slot.used = pool_available - memory_pool_.available();
    return t;
  }

  template <typename T, typename... Ts>
  T* AddRootMenu(Ts && ...params) {
    static_assert(std::is_convertible<T*, Menu*>::value, "Invalid menu type");
    static_assert(T::type_id, "Invalid menu type_id");

    size_t pool_available = memory_pool_.available();
    auto buffer = memory_pool_.Alloc(sizeof(T));
    auto menu = new (buffer) T{params...};
    root_menu_.type_id = T::type_id;
    root_menu_.instance = menu;
    root_menu_->Init();
    root_menu_.used = pool_available - memory_pool_.available();
    return menu;
  }

private:
  bool enabled_ = false;

  template <typename T> struct Slot {
    uint32_t type_id = 0;
    size_t used = 0;
    T *instance = nullptr;

    T* get() { return instance; }
    T* operator -> () { return instance; }
    const T* operator -> () const { return instance; }

    bool valid() const {
      return type_id && instance;
    }

    void Reset() {
      type_id = 0;
      used = 0;
      instance = nullptr;
    }
  };
  using ProcessorSlot = Slot<Processor>;
  using MenuSlot = Slot<Menu>;

  PatchMemoryPool &memory_pool_;

  std::array<ProcessorSlot, kNumProcessorSlots> processors_{};
  size_t num_processors_ = 0;

  MenuSlot root_menu_{};
};

} // namespace ocf4

#endif // OCF4_PATCH_H_
