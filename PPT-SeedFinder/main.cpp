#include "main.h"

char bin[BIN_MAX];

int perm_encode(int n, int* perm) {
	int encoded = 0;
	char seen[7] = {};

	for (int i = 0; i < n; i++) {
		int x = perm[i];
		for (int j = 0; j <= perm[i]; ++j) x -= seen[j];
		encoded += x * (FACT[6 - i] / FACT[7 - n]);

		seen[perm[i]] = 1;
	}

	return encoded;
}

int set_encode(int* set, int index, int hold) {
	int size[4] = {}, encoded[4];

	int i = 0;
	int c_max = 11;

	if (hold != -1) {
		size[0] = 1;
		encoded[0] = hold;
		i = 1;
		c_max = 10;
	}

	int s = index * 10 + i;
	int c = 0;

	for (; i < 4; i++) {
		int x = 7 - (s % 7);
		if (x > c_max - c) x = c_max - c;

		size[i] = x;
		encoded[i] = perm_encode(x, &set[c]);

		s += x;
		c += x;

		if (c == c_max) break;
	}

	int result = 0;
	int multiplier = 1;

	if (hold != -1) index += 7;

	for (int i = 3; i >= 0; i--) {
		if (size[i] != 0) {
			result += multiplier * encoded[i];
			multiplier *= FACT[7] / FACT[7 - size[i]];
		}
	}

	return result;
}

uint rng_progress;

uint rng_next(uint rng) {
	return rng * 0x5D588B65 + 0x269EC3;
}

void rng_generate(uint rng, int* set) {
	for (int x = 0; x < 105;) {
		int bag[7];
		for (int i = 0; i < 7; i++) bag[i] = i;

		for (int i = 0; i < 7; i++) {
			rng = rng_next(rng);

			int newIndex = ((((rng) >> 16) * (7 - i)) >> 16) + i;

			int newValue = bag[newIndex];
			int oldValue = bag[i];
			bag[i] = newValue;
			bag[newIndex] = oldValue;
		}

		for (int i = 0; i < 7; i++) set[x++] = bag[i];
	}
}

void set_iterate(int* set, int index, int hold, bool* output) {
	int pc = index % 7;
	int s = index * 10;

	if (hold != -1) {
		pc += 7;
		s += 1;
	}

	char result = bin[BIN_OFFSETS[pc] + set_encode(&set[s], index, hold)];

	if (result != -2) {
		if (index == 9) {
			*output = true;
			if (hold == -1) printf("no hold!!!!11");

		} else if (result == -1) {
			set_iterate(set, index + 1, -1, output);

		} else {
			for (int i = 0; i < 7; i++) {
				if ((result >> (6 - i)) & 1) set_iterate(set, index + 1, i, output);
			}
		}
	}
}

uint sol = 0;

void rng_search(uint i) {
	printf("  > Thread %d started\n", std::this_thread::get_id());
	do {
		rng_progress = i;
		
		int set[105];
		rng_generate(i, set);

		bool result = false;
		set_iterate(set, 0, -1, &result);

		if (result) {
			printf("  > Found %u!\n   > ", i);

			for (int j = 0; j < 101; j++) {
				printf("%d ", set[j]);
			}

			printf("\n");

			sol++;
		}
	} while ((i += THREADS) - THREADS < RNG_MAX - THREADS + 1);
}

void rng_check_progress() {
	while (true) {
		getchar();
		printf("\n   > Current RNG: %u\n   > Found RNGs: %u\n\n", rng_progress, sol);
	}
}

int main() {
	printf("\n Puyo Puyo Tetris Sprint 10PC RNG Seed Finder\n by mat1jaczyyy with help from knewjade and ChiCubed\n\n");
	
	printf(" > Loading binary PC knowledge base...\n");

	FILE *handle = fopen("D:\\all.bin", "rb");
	fread(bin, BIN_MAX, 1, handle);
	fclose(handle);

	printf("  > Done\n\n > Starting RNG search...\n");

	std::thread threads[THREADS];
	for (int i = 0; i < THREADS; i++) {
		threads[i] = std::thread(rng_search, i);
	}
	std::thread progress(rng_check_progress);

	for (int i = 0; i < THREADS; i++) {
		if (threads[i].joinable()) threads[i].join();
	}

	printf("  > Done\n\n > Found a total of %u RNG values\n\n", sol);

	exit(0);
}
