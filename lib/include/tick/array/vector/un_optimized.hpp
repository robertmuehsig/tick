

namespace tick{ namespace detail{

template <typename T>
template <typename K>
typename std::enable_if<std::is_same<T,std::atomic<K>>::value, T>::type
vector_operations_unoptimized<T>::dot(const ulong n, const T *x, const K *y) const {
  T result{0};
  for (uint64_t i = 0; i < n; ++i) {
    result += x[i] * y[i].load();
  }
  return result;
}
template <typename T>
template <typename K>
typename std::enable_if<!std::is_same<T,std::atomic<K>>::value, T>::type
vector_operations_unoptimized<T>::dot(const ulong n, const T *x, const K *y) const {
  T result{0};
  for (uint64_t i = 0; i < n; ++i) {
    result += x[i] * y[i];
  }
  return result;
}

template <typename T>
template <typename K>
typename std::enable_if<std::is_same<T,std::atomic<K>>::value>::type
vector_operations_unoptimized<T>::mult_incr(const uint64_t n, const K alpha, const T *x, T *y) const {
  for (uint64_t i = 0; i < n; ++i) {
    K y_i = y[i].load();
    y_i += alpha * x[i];
    y[i].store(y_i);
  }
}
template <typename T>
template <typename K>
typename std::enable_if<!std::is_same<T,std::atomic<K>>::value>::type
vector_operations_unoptimized<T>::mult_incr(const uint64_t n, const K alpha, const T *x, T *y) const {
  for (uint64_t i = 0; i < n; ++i) {
    y[i] += alpha * x[i];
  }
}

template <typename T>
template <typename K>
typename std::enable_if<std::is_same<T,std::atomic<K>>::value>::type
vector_operations_unoptimized<T>::set(const ulong n, const K alpha, T *x) const {
  for (uint64_t i = 0; i < n; ++i) x[i].store(alpha);
}
template <typename T>
template <typename K>
typename std::enable_if<!std::is_same<T,std::atomic<K>>::value>::type
vector_operations_unoptimized<T>::set(const ulong n, const K alpha, T *x) const {
  for (uint64_t i = 0; i < n; ++i) x[i] = alpha;
}

template <typename T>
template <typename K>
tick::promote_t<K> vector_operations_unoptimized<T>::sum(const ulong n, const T *x) const {
  return std::accumulate(x, x + n, tick::promote_t<K>{0});
}

template <typename T>
template <typename K>
typename std::enable_if<std::is_same<T,std::atomic<K>>::value>::type
vector_operations_unoptimized<T>::scale(const ulong n, const K alpha, T *x) const {
  for (uint64_t i = 0; i < n; ++i) {
    K x_i = x[i].load();
    x_i *= alpha;
    x[i].store(x_i);
  }
}
template <typename T>
template <typename K>
typename std::enable_if<!std::is_same<T,std::atomic<K>>::value>::type
vector_operations_unoptimized<T>::scale(const ulong n, const K alpha, T *x) const {
  for (uint64_t i = 0; i < n; ++i) x[i] *= alpha;
}

}  //  end namespace detail
}  //  end namespace tick
