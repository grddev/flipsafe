typedef struct cdup
{
  int orig, back;
} cdup;

cdup cdup_init(int x)
{
  return { x, protected_clone(x) };
}

int cdup_val(cdup x)
{
  assert(x.orig == x.back);
  return x.orig;
}

cdup cdup_neg(cdup x)
{
  assert(x.orig == x.back);
  return { !x.orig, !x.back };
}

