int dcebench3(int a, int b) {
	int c = 2 + b;
	int w = 2 * c;

	for(int i = 0; i < 1000; i++) {
		w = b + c;
		c = c + i;
	}
	
	return c + a;
}

int main(int argc, char const *argv[]) {
	dcebench3(2, 3);
	return 0;
}