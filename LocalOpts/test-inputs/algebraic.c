int compute (int a, int b)
{
  int result = (a/a);

  a = a + 0;
  b = b - 0;

  result *= (b/b);
  result += (b-b);
  result /= result;
  result -= result;
  return result;
}
