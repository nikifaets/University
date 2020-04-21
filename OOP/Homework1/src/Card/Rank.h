#pragma once

enum RankValues {Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, J, K, Q, A};

struct Rank{

	Rank(RankValues r, const char* name);
	Rank();
	void print_rank() const;
	
	RankValues value;
	char* name;
};