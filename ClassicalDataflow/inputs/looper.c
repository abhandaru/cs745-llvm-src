int loop (int a, int b, int c) {
  int i;
  int ret = 0;
  for (i = a; i < b; i += 2) {
    int constant = a + b;
    ret += i;
    b++;
  }
  return ret;
}
