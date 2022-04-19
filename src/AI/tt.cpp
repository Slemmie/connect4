// implements tt.h

#include "tt.h"

#include "../util/aligned_memory_alloc.h"

#include <cassert>
#include <cstring>

T_table TT(0);

T_table::~T_table() {
	aligned_large_free(m_table);
}

void T_table::resize(const size_t megabytes) {
	if (!megabytes) {
		m_size = 0;
		return;
	}
	
	aligned_large_free(m_table);
	
	m_size = megabytes * 1024 * 1024 / sizeof(TT_entry);
	
	m_table = static_cast <TT_entry*> (aligned_large_allocate(m_size * sizeof(TT_entry)));
	
	assert(m_table);
	
	clear();
}

void T_table::clear() {
	memset(&m_table[0], 0, sizeof(TT_entry) * size());
}

void T_table::update(uint64_t depth, int16_t value, uint64_t key, uint64_t bound, uint64_t turn) {
	assert(size());
	
	m_table[m_index(key)].update(depth, value, key, bound, turn);
}

TT_entry* T_table::query(uint64_t depth, uint64_t key, int alpha, int beta) {
	assert(size());
	
	TT_entry* entry = &m_table[m_index(key)];
	
	// if the entry is empty or the depth makes it too imprecise or the keys do not match: return null
	if (!entry->get_exists(entry->data) ||
	entry->get_depth(entry->data) < depth ||
	entry->get_key(entry->data) != key) {
		return nullptr;
	}
	
	// if the entry does not fail any bound checks: return the entry
	if (entry->get_bound(entry->data) == BOUND_EXACT) {
		return entry;
	}
	if (entry->get_bound(entry->data) == BOUND_UPPER && entry->value <= alpha) {
		return entry;
	}
	if (entry->get_bound(entry->data) == BOUND_LOWER && entry->value >= beta) {
		return entry;
	}
	
	return nullptr;
}

size_t T_table::m_index(uint64_t key) const {
	const uint64_t mask = m_size - 1;
	uint64_t result = key;
	result ^= key >> 8;
	result ^= key >> 16;
	result ^= key >> 24;
	result ^= key >> 32;
	return result & mask;
}
