#include "main.h"

solution* bin;

std::vector<rng_solution> solutions;
boost::mutex solutions_locker;

int max_tetrises = 9;

const char pieceSymbols[8] = {'E', 'S', 'Z', 'J', 'L', 'T', 'O', 'I'};

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

int set_encode(int* set, int index, int hold, int twolines) {
	int size[4] = {}, encoded[4];

	int i = 0;
	int c_max = 11;

	if (hold != -1) {
		size[0] = 1;
		encoded[0] = hold;
		i = 1;
		c_max = 10;
	}

	int s = index * 10 + i + (twolines * 5);
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

#if USE_VISITED_HASHING
	std::unordered_set<ulong> visited = {};
	boost::mutex visited_locker;

	ulong hash_encode(int* set) {
		ulong hash = 0;

		for (int i = 0; i < 5; i++) {
			hash *= 5040;
			hash += perm_encode(7, set + (i * 7));
		}

		return hash;
	}
#endif

#if SEARCH_TWOLINES
	const int twol_piecemap[7] = {4, 5, 3, 2, 0, 6, 1};
	std::map<int, int> twol_hash5 = {}, twol_hash6 = {};

	int twol_hash_load(const char* filename, std::map<int, int>* twol_hash) {
		FILE* handle = fopen(filename, "r");
		if (handle == NULL) {
			printf("  > Failed to open 2L database!\n");
			exit(1);
		}

		int _fr, _hash, _left, _type;
		while (fscanf(handle, "%d %d %d %d", &_fr, &_type, &_hash, &_left) > 0) {
			(*twol_hash)[_hash] = _fr | (_left << 8) | (_type << 16);
		}

		fclose(handle);
	}

	int twol_hash_encode(int hold, int* pieces) {
		int hash = hold == -1 ? 0 : twol_piecemap[hold];

		for (int i = 0; i < 5; i++) {
			hash *= 7;
			hash += twol_piecemap[pieces[i]];
		}

		return hash;
	}
#endif

uint rng_progress;

ulong rng_next(ulong& rng, uint limit) {
	#if PPT2
		ulong next = rng * 0x5851F42D4C957F2Dllu + 0x01llu;
		rng = (next >> 0x20) & 0x7FFFFFFFu;

		return limit == 0? rng : (next - (next / limit) * limit);

	#else // PPT1
		ulong next = (rng * 0x5D588B65 + 0x269EC3) & 0xFFFFFFFFu;
		rng = next;

		return ((next >> 16) * limit) >> 16;
	#endif
}

void rng_next(ulong& rng) {
	rng_next(rng, 0);
}

void rng_generate(ulong rng, int* set) {
	for (int i = 0; i < 1973; i++) // Global RNG to Piece generation RNG
		rng_next(rng);

	for (int x = 0; x < SET_SIZE;) {
		#if PPT2
			if (x == 14)
				for (int i = 0; i < 49; i++)
					rng_next(rng);
		#endif

		int bag[7];
		for (int i = 0; i < 7; i++) bag[i] = i;

		for (int i = 0; i < 7; i++) {
			int newIndex = rng_next(rng, 7 - i) + i;

			int newValue = bag[newIndex];
			int oldValue = bag[i];
			bag[i] = newValue;
			bag[newIndex] = oldValue;
		}

		for (int i = 0; i < 7 && x < SET_SIZE; i++)
			set[x++] = bag[i];
	}
}

void set_iterate(int* set, int index, int hold, const rng_solution* candidate);

void set_next(const rng_solution* next_candidate, int* set, int index, int hold_next, bool last_pc, bool is_twoline) {
	int twolines =
		#if SEARCH_TWOLINES
			next_candidate->twolines;
		#else
			0;
		#endif

	if (last_pc) {
		if (next_candidate->tetrises + (twolines >> 1) >= max_tetrises) {
			solutions_locker.lock();
			solutions.push_back(*next_candidate);
			solutions_locker.unlock();
		}

	} else if (index - (next_candidate->tetrises + ((twolines + 1) >> 1)) < SET_ITERATIONS - max_tetrises)
		set_iterate(set, index + !is_twoline, hold_next, next_candidate);
}

#if SEARCH_TWOLINES
	void twol_next(const rng_solution* from_candidate, int index, int* set, int hash, int kind, bool last_pc) {
		rng_solution next_candidate = *from_candidate;

		next_candidate.twol_data[next_candidate.twolines++] = {index, hash, kind};

		set_next(&next_candidate, set, index, kind == 6? (twol_hash6[hash] >> 8) & 0xFF : -1, last_pc, true);
	}
#endif

void set_iterate(int* set, int index, int hold, const rng_solution* candidate) {
	int twolines =
		#if SEARCH_TWOLINES
			candidate->twolines;
		#else
			0;
		#endif

	int pc = (index + twolines * 4) % 7;
	int s = index * 10 + twolines * 5;

	if (hold != -1) {
		pc += 7;
		s += 1;
	}

	bool last_pc = s >= 90;
	ulong offset = (ulong)bin + (BIN_OFFSETS[pc] + (ulong)set_encode(&set[s], index, hold, twolines)) * BIN_ELEMENT;

	if (s < 93)
	for (int i = hold != -1; i < 8; i++) {
		solution result = *((solution*)(offset + i * sizeof(solution)));
		rng_solution next_candidate = *candidate;

		if (result.solution_exists) {
			next_candidate.tetrises++;
			next_candidate.frames += next_candidate.costs[index] = result.frames;
			next_candidate.ending[index] = 4;
		}
		#if REASONABLE_DT_ASSUMPTIONS
			else if (hold != -1
				? (set[s + 9] == 2 || set[s + 9] == 3 || set[s + 9] == i - 1)
				: i != -1
					? (set[s + 10] == 2 || set[s + 10] == 3 || set[s + 10] == i - 1)
					: (set[s + 9] == 2 || set[s + 9] == 3)
			) {
				next_candidate.frames += 40;
				next_candidate.ending[index] = 3;

			} else if (hold != -1
				? (set[s + 9] == 4 || set[s + 9] == 5 || set[s + 9] == i - 1)
				: i != -1
					? (set[s + 10] == 4 || set[s + 10] == 5 || set[s + 10] == i - 1)
					: (set[s + 9] == 4 || set[s + 9] == 5)
			) {
				next_candidate.frames += 45;
				next_candidate.ending[index] = 2;

			} else if (hold != -1
				? (set[s + 9] == 6 || set[s + 9] == i - 1)
				: i != -1
					? (set[s + 10] == 6 || set[s + 10] == i - 1)
					: (set[s + 9] == 6)
				) {
				next_candidate.frames += 45;
				next_candidate.ending[index] = 1;

			} else continue;
		#endif

		set_next(&next_candidate, set, index, next_candidate.holds[index] = i - 1, last_pc, false);
	}

	#if SEARCH_TWOLINES
		// twoline
		if (s == 90 || s == 91) return;

		int hash = twol_hash_encode(hold, set + s);

		if (hold != -1) {
			// hash6 HPPPPP
			if (twol_hash6.count(hash))
				twol_next(candidate, index, set, hash, 6, last_pc);

		// hash5 PPPPP
		} else if (twol_hash5.count(hash))
			twol_next(candidate, index, set, hash, 5, last_pc);

		else {
			// hold empty and we do use hold (convert to hash5 to hash6)
			hash *= 7;
			hash += twol_piecemap[set[s + 5]];

			if (twol_hash6.count(hash))
				twol_next(candidate, index, set, hash, 6, last_pc);
		}
    #endif
}

void rng_search(uint i) {
	printf("  > Thread %d started\n", std::this_thread::get_id());

	do {
		rng_progress = i;

		int set[SET_SIZE];
		rng_generate(i, set);

		rng_solution candidate = {i};

		#if USE_VISITED_HASHING
			ulong hash = hash_encode(set);
			//printf("Hash: %016llx = ", hash);
		
			visited_locker.lock();
			if (visited.count(hash)) {
				//printf("Visited\n");
				visited_locker.unlock();
				goto next;
			}

			//printf("New\n");
			visited.insert(hash);
			visited_locker.unlock();
		#endif

		set_iterate(set, 0, -1, &candidate);

	next:;
	} while ((i += THREADS) - THREADS < RNG_MAX - THREADS + 1);
}

void rng_check_progress() {
	while (true) {
		(void)getchar();
		printf("\n   > Current RNG: 0x%08X\n   > Found RNGs: %llu\n\n", rng_progress, solutions.size());
	}
}

void print_solution(rng_solution* solution) {
	int set[SET_SIZE];
	rng_generate(solution->rng, set);

	printf("  > 0x%08X (tet: %u; cost: %u; path:", solution->rng, solution->tetrises, solution->frames);

	for (int j = 0; j < SET_ITERATIONS; j++) {
		printf(" %d%c(%d)", solution->ending[j], pieceSymbols[solution->holds[j] + 1], solution->costs[j]);

		#if SEARCH_TWOLINES
			for (int k = 0; k < solution->twolines; k++) {
				if (solution->twol_data[k].after == j) {
					std::map<int, int>* twol = solution->twol_data[k].kind == 5 ? &twol_hash5 : &twol_hash6;
					int val = (*twol)[solution->twol_data[k].hash];
					printf(" TWOL%d%d%c(%d)", solution->twol_data[k].kind, (val >> 16) & 0xFF, pieceSymbols[solution->twol_data[k].kind == 5 ? 0 : (((val >> 8) & 0xFF) + 1)], val & 0xFF);
				}
			}
		#endif
	}

	printf(") - ");

	for (int j = 0; j < SET_SIZE; j++)
		printf("%c", pieceSymbols[set[j] + 1]);

	printf("\n");
}

bool endings_equal(rng_solution* a, rng_solution* b) {
	for (int i = 0; i < SET_ITERATIONS; i++) {
		if (a->ending[i] != b->ending[i]) return false;
	}
	
	return true;
}

bool non_tets_equal(rng_solution* a, rng_solution* b) {
	for (int i = 0; i < SET_ITERATIONS; i++) {
		if ((a->ending[i] < 4 || (i < SET_ITERATIONS - 1 && a->ending[i + 1] < 4)) && a->holds[i] != b->holds[i]) return false;
	}

	return true;
}

int main() {
	printf("\n Puyo Puyo Tetris Sprint - Tetris PC RNG Seed Finder\n by mat1jaczyyy with help from knewjade and ChiCubed\n\n > Loading binary PC knowledge base...\n");

	#if USE_MEMORY_MAP
		boost::iostreams::mapped_file_source file;

		file.open(MOV_FILENAME, BIN_SIZE);

		if (!file.is_open()) {
			printf("  > Failed to open PC knowledge base!\n");
			exit(1);
		}

		bin = (solution*)file.data();

	#else
		printf("  > Allocating %llu bytes (%.2fGB) of memory...\n", BIN_SIZE, (double)BIN_SIZE / (1024. * 1024 * 1024));

		bin = (solution*)malloc(BIN_SIZE);
		if (bin == NULL) {
			printf("  > Failed to allocate memory!\n");
			exit(1);
		}

		FILE* handle = fopen(MOV_FILENAME, "rb");
		if (handle == NULL) {
			printf("  > Failed to open PC knowledge base!\n");
			exit(1);
		}

		(void)fread(bin, BIN_MAX * BIN_ELEMENT, 1, handle);
		fclose(handle);
	#endif

	#if SEARCH_TWOLINES
		printf("  > Loading 2L database...\n");

		twol_hash_load("D:\\framedata5.txt", &twol_hash5);
		twol_hash_load("D:\\framedata6.txt", &twol_hash6);
	#endif

	printf("  > Done\n\n > Starting RNG search with Tetris count %u...\n", max_tetrises);

	std::thread progress(rng_check_progress);

	do {
		#ifdef SEARCH_SPECIFIC_SEED
			rng_search(SEARCH_SPECIFIC_SEED);

		#else
			std::thread threads[THREADS];
			for (int i = 0; i < THREADS; i++) {
				threads[i] = std::thread(rng_search, i);
			}

			for (int i = 0; i < THREADS; i++) {
				if (threads[i].joinable()) threads[i].join();
			}
		#endif

		#if USE_VISITED_HASHING
			visited.clear();
		#endif

		if (solutions.empty()) printf("\n > Nothing found, restarting RNG search with Tetris count %u\n", --max_tetrises);
	} while (solutions.empty());

	printf("  > Done\n\n > Found a total of %llu RNG values\n  > Sorting results...\n", solutions.size());

	std::sort(solutions.begin(), solutions.end());

	std::vector<rng_solution> optimal_solutions;

	for (int i = 0; i < solutions.size(); i++) {
		print_solution(&solutions[i]);
		bool add = true;

		for (int j = 0; j < optimal_solutions.size(); j++) {
			if (optimal_solutions[j].rng == solutions[i].rng && endings_equal(&optimal_solutions[j], &solutions[i])) {
				add = (solutions[i] < optimal_solutions[j]) || !non_tets_equal(&optimal_solutions[j], &solutions[i]);

				if (!add) break;
			}
		}

		if (add) optimal_solutions.push_back(solutions[i]);
	}

	printf("\n > Found %llu optimal solutions\n", optimal_solutions.size());

	for (int i = 0; i < optimal_solutions.size(); i++) {
		print_solution(&optimal_solutions[i]);
	}

	#if USE_MEMORY_MAP
		file.close();
	#endif

	exit(0);
}
