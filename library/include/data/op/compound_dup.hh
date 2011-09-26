#define COMPOP BOOST_PP_ARRAY_ELEM(BOOST_PP_ITERATION(), SIHFT_DUP_COMPOPS)
template <typename T>
inline dup<T>& operator COMPOP(dup<T>& lhs, const T& rhs) {
  lhs.original COMPOP rhs;
  lhs.backup COMPOP rhs;
  return lhs;
}
template <typename T>
inline dup<T>& operator COMPOP(dup<T>& lhs, const dup<T>& rhs) {
  lhs.original COMPOP rhs.original;
  lhs.backup COMPOP rhs.backup;
  return lhs;
}
