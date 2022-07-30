#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <map>

std::vector<std::string>						words;
std::vector<std::vector<std::string>>			structures;
std::map<std::string, std::vector<std::string>> words_categorized;

std::string lowercase_string(std::string upper) {
	std::string lower = upper;

	for(unsigned int i = 0; i < upper.length(); i++) {
		lower[i] = std::tolower(upper[i]);
	}

	return lower;
}

void load_words(std::string filepath) {
	std::ifstream file;
	file.open(filepath);
	if(file.fail()) {
		file.close();
		printf("Failed to open file at %s\n", filepath.c_str());
		return;
	}
	std::string line;

	while(getline(file, line)) {
		line = lowercase_string(line);

		words.push_back(line);
		words_categorized[filepath].push_back(line);
	}

	file.close();
}

std::string categorize_word(std::string word) {
	word = lowercase_string(word);

	for(auto& e: words_categorized) {
		if(std::count(e.second.begin(), e.second.end(), word) > 0) {
			return e.first;
		}
	}

	printf("Warning: Uncategorizable word! %s\n", word.c_str());
	return "None";
}

std::string random_word(std::string type) {
	if(type == "None")
		return "(?)";

	return words_categorized[type][rand() % (words_categorized[type].size() - 1)];
}

bool str_contains(std::string str, std::string key) {
	if(str.find(key) != std::string::npos) {
		return true;
	}
	return false;
}

std::string random_phrase(std::vector<std::string> structure) {
	std::string phrase = "";

	for(unsigned int i = 0; i < structure.size(); i++) {
		std::vector<std::string> possible_words;
		std::string				 spacer = " ";

		phrase += random_word(structure[i]) + spacer;
	}

	unsigned int chance = rand() % 99;

	phrase[phrase.size() - 1] = '.';
	if(chance < 75)
		phrase[phrase.size() - 1] = '?';
	if(chance < 50)
		phrase[phrase.size() - 1] = ',';
	if(chance < 25)
		phrase[phrase.size() - 1] = '\n';

	phrase += "\n";

	return phrase;
}

int main(int argc, char** argv) {
	srand(time(NULL));

	std::string filepath = "./dicts/";

	std::vector<std::string> types = {"verbs",
									  "adjs",
									  "adverbs",
									  "nouns",
									  "dets",
									  "subj_pron",
									  "obj_prons",
									  "pron_obj_subj",
									  "pron_ind_gen_pos",
									  "pron_dep_gen_pos",
									  "conj",
									  "inter",
									  "preps"};

	for(unsigned int i = 0; i < types.size(); i++) {
		std::string full_file = filepath + types[i] + ".txt";
		load_words(full_file);
		std::printf("Loaded dictionary for %s. Loaded %ld.\n", types[i].c_str(), words_categorized[full_file].size());
	}
	std::printf("Loaded all dictionaries!\n");

	printf("Please enter some sample poetry:\n");
	std::string line = "";
	while(getline(std::cin, line)) {
		// Create an istream from the line.
		if(line == "") {
			break;
		}
		std::strstream strm;
		strm << line;
		std::string				 word;
		std::vector<std::string> phrase_structure;
		for(unsigned int i = 0; getline(strm, word, ' '); i++) {
			// Separate all punctuation:
			auto it = std::remove_if(word.begin(), word.end(), [](char const& c) { return std::ispunct(c); });
			word.erase(it, word.end());
			// Add the type into the structure
			if(word != "") {
				std::string category = categorize_word(word);
				if(category != "None") {
					phrase_structure.push_back(category);
				} else {
					phrase_structure.clear();
					break;
				}
			}
		}
		if(phrase_structure.size() > 0) {
			structures.push_back(phrase_structure);
		}
	}

	printf("\n");

	for(unsigned int i = 0; i < 200; i++) {
		std::vector<std::string> type;

		unsigned int chance = rand() % structures.size();
		type				= structures[chance];

		printf("%s", random_phrase(type).c_str());
	}

	printf("\n");
}
