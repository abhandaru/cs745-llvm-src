int loop (int a, int b, int c)
{
  int i;
  int ret = 0;
  for (i = 0; i < a; i++) {
    int x = c + 4;   // should bubble above i loop
    int j, k;
    for (j = 0; j < b; j++) {
      int y = x + i; // should bubble above j loop
      int z = x + a; // should bubble above i loop
      ret += (i * j) + y;
      for (k = 0; k < c; k++) {
        int w = (i + j) * c; // should bubble above k
        int r = z + k;
        ret *= r;
      }
    }
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  int a = loop(4, 20, 40);
  return a;
}
