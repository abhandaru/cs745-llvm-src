int deadvar(int a, int b) {
	int c = 2 + b;
	int z = 2 * c;
  int x = z - 2;
	return c + a;
}

int main(int argc, char const *argv[]) {
  int a = deadvar(3, 4);
  return a;
}
