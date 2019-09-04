#include "tokenizer.h"

// line break characters
const std::string LINE_BREAKS = "\n";
// blank characters, serving as delimiters but thrown after parsed
const std::string BLANKS = " \t\n";
// symbol characters, serving as delimiters but kept after parsed
const std::string SYMBOLS = "{}[]();*,";

bool is_line_break(char c) {
	for (char t : LINE_BREAKS) {
		if (c == t) return true;
	}
	return false;
}

bool is_blank(char c) {
	for (char t : BLANKS) {
		if (c == t) return true;
	}
	return false;
}

bool is_symbol(char c) {
	for (char t : SYMBOLS) {
		if (c == t) return true;
	}
	return false;
}

void raw_token::append(char c) {
	token.push_back(c);
}

void raw_token::set_location(size_t row, size_t col) {
	this -> row = row; this -> col = col;
}

bool raw_token::empty() const {
	return token.empty();
}

void raw_token::clear() {
	token.clear();
	row = col = 0;
}

std::string raw_token::display() const {
	return std::to_string(row) + ", " +
		std::to_string(col) + ": ";
}

// adding a new token
void append_to_tokens(std::vector<raw_token>& tokens, raw_token& new_token) {
	if (!new_token.empty()) {
		tokens.push_back(new_token);
		new_token.clear();
	}
}

// tokenization of a file
std::vector<raw_token> tokenize(const std::string& raw_string) {
	std::vector<raw_token> tokens;
	size_t current_row = 1u, current_col = 0u;	// current position of the cursor
	raw_token current_token;
	for (char c : raw_string) {
		++current_col;
		if (is_line_break(c)) {		// handling line breaks
			++current_row;
			current_col = 1u;
		}
		if (is_blank(c)) {			// handling blanks
			append_to_tokens(tokens, current_token);
		} else if (is_symbol(c)) {	// handling symbols
			append_to_tokens(tokens, current_token);
			tokens.push_back(raw_token(std::string(1, c), current_row, current_col));	// put the symbol in
		} else {					// handling other characters
			if (current_token.empty()) {	// encountering the first character of a token
				current_token.set_location(current_row, current_col);
			}
			current_token.append(c);
		}
	}
	append_to_tokens(tokens, current_token);	// add the last token
	return tokens;
}

