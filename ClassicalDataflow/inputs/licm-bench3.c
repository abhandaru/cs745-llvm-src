int loop (int a, int b, int c)
{
  int ret = 0;
  while (a < (b + c)) {
    int x;
    if (a % 2) {
      x = 5 + b; // should get hoisted
    } else {
      x = 4 + c; // should get hoisted
    }
    ret += x;
    a++;
  }
  return ret;
}

int main(int argc, char const *argv[]) {
  int a = loop(4, 20, 40);
  return a;
}
