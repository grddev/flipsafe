#define COMPOP BOOST_PP_ARRAY_ELEM(BOOST_PP_ITERATION(), COMPOPS)
inline dupint& operator COMPOP(dupint& lhs, const int& rhs) {
  lhs.original COMPOP rhs;
  lhs.backup COMPOP rhs;
  return lhs;
}
inline dupint& operator COMPOP(dupint& lhs, const dupint& rhs) {
  lhs.original COMPOP rhs.original;
  lhs.backup COMPOP rhs.backup;
  return lhs;
}
