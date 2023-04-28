#include "project.h"

// TODO:: implement your project here!

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

int main(void) {
    int gameContinues = 1;
    Character* people = malloc(2 * sizeof(Character));
    people[0].name = NULL;
    int characterCount = 0;
    // Character** people = &initialPeople;

    while (gameContinues) {
        char currentInput[1000];
        fgets(currentInput, 1000, stdin);

        // remove \n from end of input string
        int i = 0;
        while (currentInput[i] != '\0') {
            if (currentInput[i] == '\n') currentInput[i] = '\0';
            i++;
        }

        // Split command into array of words
        char** splitInput = split_string(currentInput);
        char currentCommand = splitInput[0][0];

        // length of array to use for checking length of command is correct
        int inputWordCount = 0;
        while (splitInput[inputWordCount]) inputWordCount++;

        // Check what command should be executed and act accordingly, also checks if input has correct # of words for given command
        if ((currentCommand == 'A') && (inputWordCount == 5)) {
            // checking that names are unique before adding
            int nameOk = 1; 
            int a = 0;
            while (people[a].name) {
                if (strcmp(people[a].name, splitInput[1]) == 0) {
                    printf("Names must be unique and include characters and damage and healthpoints be numbers above 0.\n");
                    printf("Invalid command %s\n", currentInput);
                    nameOk = 0;
                }
                a++;
            }
            if (nameOk) {
                Character* newPeople = add_character(splitInput[1], atoi(splitInput[2]), 0, splitInput[3], atoi(splitInput[4]), people, characterCount);
                if (!newPeople) {
                    printf("Invalid command %s\n", currentInput);
                } else {
                    people = newPeople;
                    printf("SUCCESS\n");
                    characterCount++;
                }
            }
        }
        else if ((currentCommand == 'H') && (inputWordCount == 3)) {
            if (attack_character(splitInput[1], splitInput[2], people) == 0) printf("Invalid command %s\n", currentInput);

            // Dealing with dead characters separately after every attack.
            Character* newPeople = dead_characters(people);
            if (!newPeople) {
                printf("Must be existing characters.\nInvalid command %s\n", currentInput);
            } else {
                int l = -1;
                while (people[++l].name) {
                    free(people[l].name);
                    free(people[l].weaponName);
                }
                free(people);
                people = newPeople;
                // update characterCount to reflect removal of dead characters
                int i = 0;
                while (people[i].name) i++;
                characterCount = i;
            }
        }
        // the checking software inputs "L " and the ' ' becomes a second "word", hence 2
        else if ((currentCommand == 'L') && (inputWordCount <= 2)) print_game(people);
        else if ((currentCommand == 'W') && (inputWordCount == 2)) {
            if (save_to_file(splitInput[1], people) == 0) printf("Invalid command %s\n", currentInput);
        }
        else if ((currentCommand == 'O') && (inputWordCount == 2)) {
            Character* newPeople = load_from_file(splitInput[1]);
            if (!newPeople) {
                printf("Invalid command %s\n", currentInput);
            } else {
                int l = -1;
                while (people[++l].name) {
                    free(people[l].name);
                    free(people[l].weaponName);
                }
                free(people);
                people = newPeople;
                // update characterCount to reflect new / loaded gamestate
                int i = 0;
                while (people[i].name) i++;
                characterCount = i;
            }
        }
        // the checking software inputs "Q " and the ' ' becomes a second "word", hence 2
        else if ((currentCommand == 'Q') && (inputWordCount <= 2)) gameContinues = 0;
        else printf("Invalid command %s\n", currentInput);
        
        // Free the command array's allocated memory
        int k = -1;
        while (splitInput[++k]) free(splitInput[k]);
        free(splitInput);
    }

    // Finally free the character array
    int l = -1;
    while (people[++l].name) {
        free(people[l].name);
        free(people[l].weaponName);
    }
    free(people);
    printf("SUCCESS\n");
}

/**
 * \brief adds a new character to the array in use. "A" command.
 * 
 * The only function which doesn't print "SUCCESS\n" itself since its used later on in load_from_file
 * 
 * \param name the name of the new character
 * \param healthPoints healthpoints of the new character
 * \param weaponName name of weapon
 * \param weaponDamage damage of weapon
 * \param people the existing people array
 * 
 * \returns the new array
 * \returns NULL if operation unsuccessful
*/
Character* add_character(char* name, int healthPoints, int exp, char* weaponName, int weaponDamage, Character* people, int index) {
    if (healthPoints == 0 || weaponDamage == 0) {
        printf("Health points or weapon damage can't be 0 and must be a number.\n");
        return NULL;
    }

    // set up new array then move values over
    Character* newPeople = realloc(people, (index + 2) * sizeof(Character));
    if (!newPeople) return NULL;
    newPeople[index + 1].name = NULL;

    newPeople[index].name = malloc((strlen(name) + 1) * sizeof(char));
    if (!newPeople[index].name) { 
        free(newPeople);
        return NULL; 
    }
    strcpy(newPeople[index].name, name); 
    newPeople[index].healthPoints = healthPoints;
    newPeople[index].experience = exp;

    newPeople[index].weaponName = malloc((strlen(weaponName) + 1) * sizeof(char));
    if (!newPeople[index].weaponName) {
        free(newPeople[index].name);
        free(newPeople);
        return NULL; 
    }
    strcpy(newPeople[index].weaponName, weaponName);
    newPeople[index].weaponDamage = weaponDamage;

    return newPeople;
}

/** 
 * \brief completes the instructed attack. "H" command.
 * 
 * \param attacker name of the attacker
 * \param target target of the attack
 * \param people the people array in its state before the attack
 * 
 * \returns the people array in its new state
*/
int attack_character(char* attacker, char* target, Character* people) {
    int charCount = 0, attackerIndex = 0, targetIndex = 0;
    while (people[charCount].name) {
        if (!strcmp(people[charCount].name, attacker)) attackerIndex = charCount;
        if (!strcmp(people[charCount].name, target)) targetIndex = charCount;
        charCount++;
    }
    charCount++;

    // roots out bad commands
    if (attackerIndex == targetIndex || attackerIndex == charCount || targetIndex == charCount) {
        printf("Invalid name or names.\n");
        return 0;
    }

    int damage = people[attackerIndex].weaponDamage;
    int exp = people[attackerIndex].experience;
    int hp = people[targetIndex].healthPoints;

    people[attackerIndex].experience = exp + damage;
    people[targetIndex].healthPoints = hp - damage;

    char* attackingWeapon = people[attackerIndex].weaponName;

    printf("%s attacked %s with %s by %d damage.\n", attacker, target, attackingWeapon, damage);
    printf("%s has %d hit points remaining.\n", target, (hp - damage));
    if (people[targetIndex].healthPoints <= 0) printf("%s has died.\n", target);
    printf("%s gained %d experience points.\n", attacker, damage);

    printf("SUCCESS\n");
    return 1;
}

/**
 * \brief a function to deal with dead characters, simply returns an array without any of the dead characters.
 * 
 * \param people the people list
 * 
 * \returns a new array without the dead characters
 * \returns NULL if runs into error
*/
Character* dead_characters(const Character* people) {
    int tempindex = 0;
    Character* tempPeople = calloc(2, sizeof(Character));
    if (!tempPeople) return 0;
    tempPeople[0].name = NULL;
    int i = 0;
    while (people[i].name) {
        if (people[i].healthPoints > 0) {
            Character* newPeople = add_character(people[i].name, people[i].healthPoints, people[i].experience, people[i].weaponName, people[i].weaponDamage, tempPeople, tempindex);
            if (!newPeople) return NULL;
            else {
                tempPeople = newPeople;
                tempindex++;
            }
        }
        i++;
    }
    return tempPeople;
}

/**
 * \brief prints the current gamestate line-by-line. "L" command.
 * 
 * \param people the array containing the current characters
*/
void print_game(const Character* people) {
    int i = 0;
    while (people[i].name) {
        printf("%s %d %d %s %d\n", people[i].name, people[i].healthPoints, people[i].experience, people[i].weaponName, people[i].weaponDamage);
        i++;
    }
    printf("SUCCESS\n");
}

/**
 * \brief saves current gamestate to file. "W" command.
 * 
 * \param filename The name of the file.
 * 
 * \returns 0 if error
 * \returns 1 if error
*/
int save_to_file(const char* filename, const Character* people) {
    FILE* myFile = fopen(filename, "w");
    if (myFile == NULL) {
        printf("Couldn't open or create that file.");
        return 0;
    }

    int i = 0;
    while (people[i].name) {
        fprintf(myFile, "%s %d %d %s %d\n", people[i].name, people[i].healthPoints, people[i].experience, people[i].weaponName, people[i].weaponDamage);
        i++;
    }

    fclose(myFile);
    printf("SUCCESS\n");
    return 1;
}

/**
 * \brief loads saved gamestate from file. "O" command.
 * 
 * \param filename the name of the file.
 * 
 * \returns new people array if successful.
 * \returns NULL if error.
*/
Character* load_from_file(const char* filename) {
    FILE* myFile = fopen(filename, "r");
    if (myFile == NULL) {
        printf("Couldn't open that file.\n");
        return NULL;
    }

    int peopleNum = 0; 
    char line[1000];

    // get number of people
    while (fgets(line, sizeof(line), myFile) != NULL) peopleNum++;
    rewind(myFile); 

    Character* people = malloc((peopleNum + 1) * sizeof(Character)); 
    if (!people) return NULL;

    // loop through lines and add_character on each
    int characterCount = 0;
    while (fgets(line, sizeof(line), myFile) != NULL) {
        char** splitInput = split_string(line);
        Character* newPeople = add_character(splitInput[0], atoi(splitInput[1]), atoi(splitInput[2]), splitInput[3], atoi(splitInput[4]), people, characterCount);
        if (!newPeople) return NULL;
        else {
            people = newPeople;
            characterCount++;
        }
        int k = -1;
        while (splitInput[++k]) free(splitInput[k]);
        free(splitInput);
    }

    fclose(myFile);
    printf("SUCCESS\n");
    return people;
}

/**
 * \brief Splits a string on every " " and returns an array.
 * 
 * \param str The null-terminated string to split.
 * \return An array of strings.     
 */
char** split_string(const char* str) {
    // get number of words for "result" malloc
    int num = 1;
    const char* pointer = str;
    while ((pointer = strstr(pointer, " "))) {
        num++;
        pointer++;
    }
    pointer = str; // reset pointer

    char** result = malloc(sizeof(char*) * (num + 1));

    // pass through word by word dynamically allocating space in result array
    int i = 0;
    while ((pointer = strstr(pointer, " "))) {
        int len = pointer - str;
        result[i] = malloc(sizeof(char) * (len + 1));
        strncpy(result[i], str, len);
        result[i][len] = '\0'; // null terminated strings

        i++;
        pointer++; 
        str = pointer; // bring str to the end of the previous word
    }

	char *temp; 
    char *tempPointer; 
    int len = strlen(str);
    temp = malloc(len + 1);
    tempPointer = temp;
    while (*str) *tempPointer++ = *str++;
    *tempPointer = '\0';

    result[i] = temp;
    result[num] = NULL;
    return result;
}

