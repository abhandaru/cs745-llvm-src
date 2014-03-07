int loop (int a, int b, int c)
{
  int i;
  int ret = 0;
  for (i = a; i < b; i++) {
    int x = c + 4;   // should bubble above i loop
    int j, k;
    for (j = i; j < b; j++) {
      int y = x + i; // should bubble above j loop
      int z = x + a; // should bubble above i loop
      ret += (i * j) + y;
    }
    for (k = i; k < c; k++) {
      ret += 1;
    }
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  int a = loop(4, 20, 40);
  return a;
}
