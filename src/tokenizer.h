#ifndef __TOKENIZER_H
#define __TOKENIZER_H

#include <string>
#include <vector>

// raw token information
struct raw_token {
	std::string token;				// the string representation
	size_t row, col;				// the location in the original file
	raw_token() {}
	raw_token(std::string token, size_t row, size_t col) : token(token), row(row), col(col) {}
	void append(char c);			// appends to the token
	void set_location(size_t row, size_t col);
	bool empty() const;				// checks if the token contains any character
	void clear();					// clears the token
	std::string display() const;	// displays token information
};

// tokenization of a file
std::vector<raw_token> tokenize(const std::string& raw_string);

#endif
