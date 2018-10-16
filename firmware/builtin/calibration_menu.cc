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

#include "builtin/calibration_menu.h"

namespace ocf4 {

enum CALIBRATION_STEP {
  HELLO,
  CENTER_DISPLAY,

  DAC_A_VOLT_3m, DAC_A_VOLT_2m, DAC_A_VOLT_1m, DAC_A_VOLT_0, DAC_A_VOLT_1, DAC_A_VOLT_2, DAC_A_VOLT_3, DAC_A_VOLT_4, DAC_A_VOLT_5, DAC_A_VOLT_6,
  DAC_B_VOLT_3m, DAC_B_VOLT_2m, DAC_B_VOLT_1m, DAC_B_VOLT_0, DAC_B_VOLT_1, DAC_B_VOLT_2, DAC_B_VOLT_3, DAC_B_VOLT_4, DAC_B_VOLT_5, DAC_B_VOLT_6,
  DAC_C_VOLT_3m, DAC_C_VOLT_2m, DAC_C_VOLT_1m, DAC_C_VOLT_0, DAC_C_VOLT_1, DAC_C_VOLT_2, DAC_C_VOLT_3, DAC_C_VOLT_4, DAC_C_VOLT_5, DAC_C_VOLT_6,
  DAC_D_VOLT_3m, DAC_D_VOLT_2m, DAC_D_VOLT_1m, DAC_D_VOLT_0, DAC_D_VOLT_1, DAC_D_VOLT_2, DAC_D_VOLT_3, DAC_D_VOLT_4, DAC_D_VOLT_5, DAC_D_VOLT_6,

  CV_OFFSET_0, CV_OFFSET_1, CV_OFFSET_2, CV_OFFSET_3,
  ADC_PITCH_C2, ADC_PITCH_C4,
  CALIBRATION_SCREENSAVER_TIMEOUT,
  CALIBRATION_EXIT,
  CALIBRATION_STEP_LAST,
  CALIBRATION_STEP_FINAL = ADC_PITCH_C4
};

enum CALIBRATION_TYPE {
  CALIBRATE_NONE,
  CALIBRATE_OCTAVE,
  CALIBRATE_ADC_OFFSET,
  CALIBRATE_ADC_1V,
  CALIBRATE_ADC_3V,
  CALIBRATE_DISPLAY,
  CALIBRATE_SCREENSAVER,
};

struct CalibrationStep {
  const char *title;
/*
  const char *message;
  const char *help; // optional
  const char *footer;

  CALIBRATION_TYPE calibration_type;
  int index;

  const char * const *value_str; // if non-null, use these instead of encoder value
  int min, max;
*/
};

struct CalibrationStepDesc {
  const char *title;
};
static constexpr std::array<CalibrationStep, CALIBRATION_STEP_LAST> calibration_steps = {
  {"START"}
};

void CalibrationMenu::Init()
{

}

void CalibrationMenu::Tick()
{
}

void CalibrationMenu::HandleEvent(const EventType &)
{
}

void CalibrationMenu::Draw(Display::Frame &frame) const
{
  frame->printf(0, 0, "%d/%d %s", current_step_ + 1, calibration_steps.size(), calibration_steps[current_step_].title);
  frame->drawHLine(0, 10, 128);
}

void CalibrationMenu::SerialCommand(uint8_t)
{
}

void CalibrationMenu::DebugView(Display::Frame &) const
{
}

}; // namespace ocf4
