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

#ifndef OCF4_CALIBRATION_PATCH_H_
#define OCF4_CALIBRATION_PATCH_H_

#include "patch.h"
#include "calibration_data.h"

namespace ocf4 {

class CalibrationPatch {
public:
  static void Load(Patch &patch);
};

class CalibrationProcessor : public Processor {
public:
  static constexpr uint32_t type_id = FOURCC<'p','C','A','L'>::value;

  virtual void Init(PatchMemoryPool &memory_pool) final;
  virtual void Process(IOFrame &io_frame) final;

protected:

  EditableParameter<int32_t> octave_ = {"OCTAVE", 0, 0, 10};
};

}; // namespace ocf4

#endif // OCF4_CALIBRATION_PATCH_H_
