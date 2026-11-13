

	int l = 1, r = n;
	while (l <= r) {
		int mid = (l + r) / 2;
		if (a[mid] == 查询值) {
			cout << mid;
			break;
		}
		if (a[mid] < 查询值) {
			l = mid + 1;
		} else {
			r = mid - 1;
		}
	}
	 1  2  3
	 key = 4
a = {1, 3, 5}
l = 1, r = 3, mid = 2
a[mid] < key  l = mid + 1 = 3
l = 3, r = 3, mid = 3;
a[mid] > key  r = mid = 3
