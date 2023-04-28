#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct {
    char* name;
    int healthPoints;
    int experience;
    char* weaponName;
    int weaponDamage;
} Character;

// Add character, A
Character* add_character(char* name, int healthPoints, int exp, char* weaponName, int weaponDamage, Character* people, int index);

// Attack target, H
int attack_character(char* attacker, char* target, Character* people);

// Returns an array without dead characters
Character* dead_characters(const Character* people);

// Print game, L
void print_game(const Character* people);

// Save to file, W
int save_to_file(const char* filename, const Character* people);

// Load from file, O
Character* load_from_file(const char* filename);

// Splits string from Module 8: Task 2
char** split_string(const char* str);

#endif //! _PROJECT__H_