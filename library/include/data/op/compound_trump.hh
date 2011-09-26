#define COMPOP BOOST_PP_ARRAY_ELEM(BOOST_PP_ITERATION(), SIHFT_TRUMP_COMPOPS)
// These operations can have considerable overhead compared to their native
// counterparts, but we rely on the compiler to optimize some of it away.
template <typename T, int A, typename U>
inline trump<T, A>& operator COMPOP(trump<T,A>& lhs, const U& rhs) {
  lhs.original COMPOP rhs;
  (lhs.backup COMPOP rhs) *= A;
  return lhs;
}
template <typename T, int A, typename U, int B>
inline trump<T,A>& operator COMPOP(trump<T,A>& lhs, const trump<U,B>& rhs) {
  lhs.original COMPOP rhs.original;
  (lhs.backup COMPOP (rhs.backup / B)) *= A;
  return lhs;
}
