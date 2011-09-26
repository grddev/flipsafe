#define COMPOP BOOST_PP_ARRAY_ELEM(BOOST_PP_ITERATION(), SIHFT_TRI_COMPOPS)
template <typename T>
inline tri<T>& operator COMPOP(tri<T>& lhs, const T& rhs) {
  lhs.original COMPOP rhs;
  lhs.backup1 COMPOP rhs;
  lhs.backup2 COMPOP rhs;
  return lhs;
}
template <typename T>
inline tri<T>& operator COMPOP(tri<T>& lhs, const tri<T>& rhs) {
  lhs.original COMPOP rhs.original;
  lhs.backup1 COMPOP rhs.backup1;
  lhs.backup2 COMPOP rhs.backup2;
  return lhs;
}

