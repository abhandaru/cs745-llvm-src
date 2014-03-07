int deadbranch(int a) {
	int b = 3;
	int c = 0;
	if (b < 0)
		c = c + 1;
	else 
		c = c + a;
	return c;
}