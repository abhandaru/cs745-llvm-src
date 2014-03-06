int loop (int a, int b, int c) {
  int i;
  int x = 2;
  int ret = 0;
  for (i = a; i < b; i++) {
    int constant = x + 2;
    ret += i;
  }
  return ret;
}
