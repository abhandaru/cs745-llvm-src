int loop (int a, int b, int c)
{
  int i;
  int ret = 0;
  for (i = a; i < b; i++) {
    int j, k;
    for (j = i; j < b; j++) {
      ret += i * j;
    }
    for (k = i; k < c; k++) {
      ret += 1;
    }
  }
  return ret;
}
