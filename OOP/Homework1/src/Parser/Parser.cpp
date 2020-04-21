#include "Parser.h"
#include <iostream>
bool Parser::validate_int(int val, int lower, int upper){

	return (val >= lower && val <= upper);
}