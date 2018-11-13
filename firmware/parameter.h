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

#ifndef OCF4_PARAMETER_H_
#define OCF4_PARAMETER_H_

#include <stdint.h>
#include <limits>
#include <type_traits>

namespace ocf4 {

enum class ParameterType : uint32_t {
  INVALID = 0,
  I32   = (0x1 << 1),
  U32   = (0x1 << 2),
  FLOAT = (0x1 << 3),
};

template <typename T> struct ParameterTypeTraits { };
template <> struct ParameterTypeTraits<int32_t> { static constexpr ParameterType parameter_type = ParameterType::I32; };
template <> struct ParameterTypeTraits<uint32_t> { static constexpr ParameterType parameter_type = ParameterType::U32; };
template <> struct ParameterTypeTraits<float> { static constexpr ParameterType parameter_type = ParameterType::FLOAT; };

template <typename T> class EditableParameter;

class EditableParameterBase {
public:
  using storage_type = typename std::aligned_union<4, int32_t, uint32_t, float>::type;
  static constexpr auto storage_size = sizeof(storage_type);

  // TODO This is pretty unsafe. Ideally it would be checkable at compile time, except type is run-time...
  template <typename T>
  EditableParameter<T> &typed() {
    return *static_cast<EditableParameter<T> *>(this);
  }

  ParameterType parameter_type() const {
    return parameter_type_;
  }

  const char *parameter_name() const {
    return parameter_name_;
  }

  void Reset() {
    value_ = default_;
  }

protected:
  const ParameterType parameter_type_ = ParameterType::INVALID;
  const char *const parameter_name_ = 0;

  storage_type value_;
  storage_type default_;
  storage_type min_;
  storage_type max_;

  EditableParameterBase() = default;

  template <typename T>
  EditableParameterBase(const char *name, T value)
  : EditableParameterBase(name, value, std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) { }

  template <typename T>
  EditableParameterBase(const char *name, T value, T min, T max)
  : parameter_type_{ParameterTypeTraits<T>::parameter_type}
  , parameter_name_{name}
  {
    static_assert(sizeof(T) <= storage_size, "Type larger than storage");
    new (&value_) T{value};
    new (&default_) T{value};
    new (&min_) T{min};
    new (&max_) T{max};
  }
};

template <typename T>
class EditableParameter : public EditableParameterBase {
public:
//  static_assert(sizeof(T) <= storage_size);

  EditableParameter() = default;
  EditableParameter(const char *name, T v) : EditableParameterBase(name, v) { }
  EditableParameter(const char *name, T v, T minv, T maxv) : EditableParameterBase(name, v, minv, maxv) {  }

  T value() const {
    return *reinterpret_cast<const T *>(&value_);
  }

  T min() const {
    return *reinterpret_cast<const T*>(&min_);
  }

  T max() const {
    return *reinterpret_cast<const T*>(&max_);
  }

  void update(T delta) {
    set(value() + delta);
  }

  void set(T v) {
    if (v > max()) v = max();
    else if (v < min()) v = min();
    new (&value_) T{v};
  }
};

} // namespace ocf4

#endif // OCF4_PARAMETER_H_
