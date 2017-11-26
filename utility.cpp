
int getIndexOfMinimum(const float& a, const float& b, const float& c) {
	if (a <= b) {
		if (a <= c)
			return 0;
		return 2;
	}

	if (b <= c)
		return 1;
	return 2;
}
