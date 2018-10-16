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
#include "patch.h"
#include <algorithm>

namespace ocf4 {

void Patch::Init()
{
}

void Patch::Reset()
{
  enabled_ = false;
  num_processors_ = 0;

  if (root_menu_.valid()) {
    root_menu_->~Menu();
    root_menu_.Reset();
  }

  memory_pool_.Free();
  std::for_each(
      std::begin(processors_),
      std::end(processors_),
      [](ProcessorSlot &slot) { slot.Reset(); }
  );
}

void Patch::Process(IOFrame &io_frame)
{
  if (!num_processors_)
    return;

  // TODO ADC -> calibrated values

  std::for_each(
      std::begin(processors_),
      std::end(processors_),
      [&](ProcessorSlot &slot) {
        if (slot.valid())
          slot->Process(io_frame);
      }
  );

  // TODO DAC -> calibrated values
}

void Patch::IdleLoop()
{
}

/*virtual*/ void Patch::DebugView(Display::Frame &frame) const
{
  weegfx::coord_t y = 16;
  std::for_each(
      std::begin(processors_),
      std::end(processors_),
      [&](const ProcessorSlot &slot) {
        if (slot.valid())
          frame->printf(0, y, "%.4s (%u)", (const char *)&slot.type_id, slot.used);
        else
          frame->printf(0, y, "--");
        y += 8;
      }
  );
  if (root_menu_.valid())
    frame->printf(0, y, "%.4s (%u)", (const char *)&root_menu_.type_id, root_menu_.used);
  else
    frame->printf(0, y, "____ (%u)", root_menu_.used);
}

}; // namespace ocf4
