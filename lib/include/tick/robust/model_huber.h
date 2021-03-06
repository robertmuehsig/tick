
#ifndef LIB_INCLUDE_TICK_ROBUST_MODEL_HUBER_H_
#define LIB_INCLUDE_TICK_ROBUST_MODEL_HUBER_H_

// License: BSD 3 clause

#include "tick/base_model/model_generalized_linear.h"
#include "tick/base_model/model_lipschitz.h"

template <class T>
class DLL_PUBLIC TModelHuber : public virtual TModelGeneralizedLinear<T>,
                               public TModelLipschitz<T> {
 protected:
  using TModelGeneralizedLinear<T>::features_norm_sq;
  using TModelGeneralizedLinear<T>::compute_features_norm_sq;
  using TModelGeneralizedLinear<T>::n_samples;
  using TModelGeneralizedLinear<T>::n_features;
  using TModelGeneralizedLinear<T>::fit_intercept;
  using TModelGeneralizedLinear<T>::compute_grad_i;
  using TModelGeneralizedLinear<T>::n_threads;
  using TModelGeneralizedLinear<T>::get_inner_prod;
  using TModelLipschitz<T>::ready_lip_consts;
  using TModelLipschitz<T>::lip_consts;

 public:
  using TModelGeneralizedLinear<T>::get_label;
  using TModelGeneralizedLinear<T>::grad_i;
  using TModelGeneralizedLinear<T>::get_features;
  using TModelGeneralizedLinear<T>::grad_i_factor;
  using TModelGeneralizedLinear<T>::get_class_name;

 private:
  T threshold, threshold_squared_over_two;

 public:
  // This exists soley for cereal/swig
  TModelHuber() : TModelHuber(nullptr, nullptr, false, 1) {}

  TModelHuber(const std::shared_ptr<BaseArray2d<T> > features,
              const std::shared_ptr<SArray<T> > labels,
              const bool fit_intercept, const T threshold,
              const int n_threads = 1)
      : TModelLabelsFeatures<T>(features, labels),
        TModelGeneralizedLinear<T>(features, labels, fit_intercept, n_threads) {
    set_threshold(threshold);
  }

  T loss_i(const ulong i, const Array<T> &coeffs) override;

  T grad_i_factor(const ulong i, const Array<T> &coeffs) override;

  void compute_lip_consts() override;

  virtual T get_threshold(void) const { return threshold; }

  virtual void set_threshold(const T threshold) {
    if (threshold <= 0.) {
      TICK_ERROR("threshold must be > 0");
    } else {
      this->threshold = threshold;
      threshold_squared_over_two = threshold * threshold / 2;
    }
  }

  template <class Archive>
  void serialize(Archive &ar) {
    ar(cereal::make_nvp("ModelGeneralizedLinear",
                        cereal::base_class<TModelGeneralizedLinear<T> >(this)));
    ar(cereal::make_nvp("ModelLipschitz",
                        cereal::base_class<TModelLipschitz<T> >(this)));
    ar(threshold);
    ar(threshold_squared_over_two);
  }

  BoolStrReport compare(const TModelHuber<T> &that, std::stringstream &ss) {
    ss << get_class_name() << std::endl;
    bool are_equal = TModelGeneralizedLinear<T>::compare(that, ss) &&
                     TModelLipschitz<T>::compare(that, ss) &&
                     TICK_CMP_REPORT(ss, threshold) &&
                     TICK_CMP_REPORT(ss, threshold_squared_over_two);

    return BoolStrReport(are_equal, ss.str());
  }
  BoolStrReport compare(const TModelHuber<T> &that) {
    std::stringstream ss;
    return compare(that, ss);
  }
  BoolStrReport operator==(const TModelHuber<T> &that) {
    return TModelHuber<T>::compare(that);
  }
};

using ModelHuber = TModelHuber<double>;
using ModelHuberDouble = TModelHuber<double>;
using ModelHuberFloat = TModelHuber<float>;

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(ModelHuberDouble,
                                   cereal::specialization::member_serialize)
CEREAL_REGISTER_TYPE(ModelHuberDouble)

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(ModelHuberFloat,
                                   cereal::specialization::member_serialize)
CEREAL_REGISTER_TYPE(ModelHuberFloat)

#endif  // LIB_INCLUDE_TICK_ROBUST_MODEL_HUBER_H_
