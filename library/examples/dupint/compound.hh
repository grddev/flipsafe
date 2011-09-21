#define COMPOP_NUM(x) COMPOP ## x
#define COMPOP_FORCE_EVAL(x) COMPOP_NUM(x)
#define COMPOP COMPOP_FORCE_EVAL(BOOST_PP_ITERATION())
inline dupint& operator COMPOP(dupint& lhs, const int& rhs) {
  lhs.original COMPOP rhs;
  lhs.backup COMPOP rhs;
  return lhs;
}
