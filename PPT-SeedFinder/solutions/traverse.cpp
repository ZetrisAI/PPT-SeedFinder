#include "traverse.h"

bool traverse(int* bag, int hold, int* out, const std::vector<int>* graph) {
	bool placed[7] = {0, 0, 0, 0, 0, 0, 0};
	bool held = false;

	int start = 0;

	if (hold == HOLD_FIRST_BAG) {
		hold = bag[0];
		start++;
	}

	for (int i = start; i < 7; i++) {
		bool broke = false;

		bool cant = placed[bag[i]];

		if (!cant) {
			for (int j = 0; j < graph[bag[i]].size(); j++) {
				if (!placed[graph[bag[i]][j]]) {
					cant = true;
					break;
				}
			}
		}

		if (cant) {
			if (!held) {
				if (hold == HOLD_EMPTY) {
					hold = bag[i];
				} else if (!placed[hold]) {
					int temp = hold;
					hold = bag[i];
					bag[i] = temp;
					i--;
				} else {
					return false;
				}
				held = true;
				broke = true;
			} else {
				return false;
			}
		}

		if (!broke) {
			held = false;
			placed[bag[i]] = true;
		}
	}

	if (!placed[hold]) {
		hold = HOLD_FIRST_BAG;
	}

	*out = hold;

	return true;
}

bool traverse_with_preserve(int* bag, int hold, int preserve, int* out, const std::vector<int>* graph) {
	bool placed[7] = {0, 0, 0, 0, 0, 0, 0};
	bool held = false;

	int start = 0;

	if (hold == HOLD_FIRST_BAG) {
		hold = bag[0];
		start++;
	}

	for (int i = start; i < 7; i++) {
		bool broke = false;

		bool cant = placed[bag[i]];

		if (!cant) {
			for (int j = 0; j < graph[bag[i]].size(); j++) {
				if (!placed[graph[bag[i]][j]]) {
					cant = true;
					break;
				}
			}
		}

		if (cant) {
			if (!held) {
				if (hold == HOLD_EMPTY) {
					hold = bag[i];
				} else if (!placed[hold]) {
					int temp = hold;
					hold = bag[i];
					bag[i] = temp;
					i--;
				} else {
					return false;
				}
				held = true;
				broke = true;
			} else {
				return false;
			}
		}

		if (!broke) {
			held = false;
			placed[bag[i]] = true;
		}
	}

	if (hold == preserve && !placed[hold]) {
		*out = preserve;
		return true;
	}

	return false;
}

bool traverse_preserved(int* bag, int hold, int* out, const std::vector<int>* graph) {
	bool placed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	bool held = false;

	if (hold == HOLD_FIRST_BAG) {
		return false;
	}

	int bag_new[8];
	bag_new[0] = 7;
	memcpy(&bag_new[1], &bag[0], 7 * sizeof(*bag));
	hold = HOLD_EMPTY;

	for (int i = 0; i < 8; i++) {
		bool broke = false;

		bool cant = placed[bag_new[i]];

		if (!cant) {
			for (int j = 0; j < graph[bag_new[i]].size(); j++) {
				if (!placed[graph[bag_new[i]][j]]) {
					cant = true;
					break;
				}
			}
		}

		if (cant) {
			if (!held) {
				if (hold == HOLD_EMPTY) {
					hold = bag_new[i];
				} else if (!placed[hold]) {
					int temp = hold;
					hold = bag_new[i];
					bag_new[i] = temp;
					i--;
				} else {
					return false;
				}
				held = true;
				broke = true;
			} else {
				return false;
			}
		}

		if (!broke) {
			held = false;
			placed[bag_new[i]] = true;
		}
	}

	if (!placed[hold]) {
		hold = HOLD_FIRST_BAG;
	}

	*out = hold;

	return true;
}

bool traverse_preserved_with_preserve(int* bag, int hold, int preserve, int* out, const std::vector<int>* graph) {
	bool placed[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	bool held = false;

	if (hold == HOLD_FIRST_BAG) {
		return false;
	}

	int bag_new[8];
	bag_new[0] = 7;
	memcpy(&bag_new[1], &bag[0], 7 * sizeof(*bag));
	hold = HOLD_EMPTY;

	for (int i = 0; i < 8; i++) {
		bool broke = false;

		bool cant = placed[bag_new[i]];

		if (!cant) {
			for (int j = 0; j < graph[bag_new[i]].size(); j++) {
				if (!placed[graph[bag_new[i]][j]]) {
					cant = true;
					break;
				}
			}
		}

		if (cant) {
			if (!held) {
				if (hold == HOLD_EMPTY) {
					hold = bag_new[i];
				} else if (!placed[hold]) {
					int temp = hold;
					hold = bag_new[i];
					bag_new[i] = temp;
					i--;
				} else {
					return false;
				}
				held = true;
				broke = true;
			} else {
				return false;
			}
		}

		if (!broke) {
			held = false;
			placed[bag_new[i]] = true;
		}
	}

	if (hold == preserve && !placed[hold]) {
		*out = preserve;
		return true;
	}

	return false;
}