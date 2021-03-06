#ifndef LIB_INCLUDE_TICK_PROX_PROX_POSITIVE_H_
#define LIB_INCLUDE_TICK_PROX_PROX_POSITIVE_H_

// License: BSD 3 clause

#include "prox_separable.h"

template <class T>
class DLL_PUBLIC TProxPositive : public TProxSeparable<T> {
 public:
  using TProxSeparable<T>::get_class_name;

 private:
  T call_single(T x, T step) const override;

  // Repeat n_times the prox on coordinate i
  T call_single(T x, T step, ulong n_times) const override;

 public:
  // This exists soley for cereal/swig
  TProxPositive() : TProxPositive(0) {}

  explicit TProxPositive(T strength) : TProxSeparable<T>(strength, true) {}

  TProxPositive(T strength, ulong start, ulong end)
      : TProxSeparable<T>(strength, start, end, true) {}

  // Override value, only this value method should be called
  T value(const Array<T>& coeffs, ulong start, ulong end) override;

  template <class Archive>
  void serialize(Archive& ar) {
    ar(cereal::make_nvp("ProxSeparable",
                        cereal::base_class<TProxSeparable<T> >(this)));
  }

  BoolStrReport compare(const TProxPositive<T>& that) {
    std::stringstream ss;
    ss << get_class_name();
    auto super = TProxSeparable<T>::compare(that, ss);
    return BoolStrReport(super, ss.str());
  }
  BoolStrReport operator==(const TProxPositive<T>& that) {
    return compare(that);
  }
};

using ProxPositive = TProxPositive<double>;
using ProxPositiveDouble = TProxPositive<double>;
using ProxPositiveFloat = TProxPositive<float>;

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(ProxPositiveDouble,
                                   cereal::specialization::member_serialize)
CEREAL_REGISTER_TYPE(ProxPositiveDouble)

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(ProxPositiveFloat,
                                   cereal::specialization::member_serialize)
CEREAL_REGISTER_TYPE(ProxPositiveFloat)

#endif  // LIB_INCLUDE_TICK_PROX_PROX_POSITIVE_H_
