// transposition table
// (glorified dynamic programming optimization)

#pragma once

#include <cstdint>
#include <cstddef>

enum Bound : uint64_t {
	
	BOUND_EXACT = 0,
	BOUND_LOWER = 1,
	BOUND_UPPER = 2
	
};

// sizeof(TT_Entry) -> 10
struct TT_entry {
	
	inline void update(uint64_t _depth, int16_t _value, uint64_t _key, uint64_t _bound, uint64_t _turn) {
		// do not override an existing entry with lower depth
		// do override the entry if depths are equal:
		// assume frequently appearing keys will be probed often too
		if (get_exists(data) && _depth < get_depth(data)) {
			return;
		}
		
		// do not override existing entries with exact bounds - if new candidate does not have exact bound
		if (get_exists(data) && get_bound(data) == BOUND_EXACT && _bound != BOUND_EXACT) {
			return;
		}
		
		data = 0;
		value = _value;
		
		data |= _key;
		data |= _depth << 49;
		data |= _bound << (49 + 6);
		data |= 1ULL << (49 + 6 + 3);
		data |= _turn << (49 + 6 + 3 + 1);
	}
	
	// key info      49 bits
	// depth info     6 bits
	// bound info     3 bits
	// exists info    1 bit
	// turn info      1 bit
	
	// total info    60 bits
	uint64_t data;
	
	// value info    16 bits
	int16_t value;
	
	constexpr uint64_t get_key(uint64_t x) const {
		return x & ((1ULL << 49) - 1);
	}
	
	constexpr uint64_t get_depth(uint64_t x) const {
		return (x >> 49) & ((1ULL << 6) - 1);
	}
	
	constexpr uint64_t get_bound(uint64_t x) const {
		return (x >> (49 + 6)) & ((1ULL << 3) - 1);
	}
	
	constexpr uint64_t get_exists(uint64_t x) const {
		return (x >> (49 + 6 + 3)) & ((1ULL << 1) - 1);
	}
	
	constexpr uint64_t get_turn(uint64_t x) const {
		return (x >> (49 + 6 + 3 + 1)) & ((1ULL << 1) - 1);
	}
	
};

class T_table {
	
public:
	
	T_table(const size_t _megabytes) {
		resize(_megabytes);
	}
	
	~T_table();
	
	constexpr size_t size() const {
		return m_size;
	}
	
	void resize(const size_t megabytes);
	
	void clear();
	
	void update(uint64_t depth, int16_t value, uint64_t key, uint64_t bound, uint64_t turn);
	
	TT_entry* query(uint64_t depth, uint64_t key, int alpha, int beta);
	
private:
	
	TT_entry* m_table;
	
	size_t m_size;
	
	size_t m_index(uint64_t key) const;
	
};

extern T_table TT;
