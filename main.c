#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PEOPLE 100

typedef struct Person Person;
struct Person{
char name[50];
int age;
Person* Mother;
Person* Father;
};

Person* persons[MAX_PEOPLE];
int numPersons = 0;

Person* findPersonByName(const char* name) { //Function to find person by name.
    for (int i = 0; i < numPersons; i++) {
        if (strcmp(persons[i]->name, name) == 0) {
            return persons[i];
        }
    }
    return NULL; 
}

void writePersonToFile(FILE* fp, const Person* person) {  //Function to write a person into a file using fprintf.
    fprintf(fp, "Name: %s\n", person->name);
    fprintf(fp, "Age: %d\n", person->age);

    // Write the mother's name or '-' if the mother is NULL
    if (person->Mother != NULL) {
        fprintf(fp, "Mother: %s\n", person->Mother->name);
    } else {
        fprintf(fp, "Mother: -\n");
    }

    // Write the father's name or '-' if the father is NULL
    if (person->Father != NULL) {
        fprintf(fp, "Father: %s\n\n", person->Father->name);
    } else {
        fprintf(fp, "Father: -\n\n");
    }
}

Person* createfirstPerson(const char* pname, int age) {  //Creating the first person and setting the mother and father pointers NULL.
    Person* person = (Person*)malloc(sizeof(Person));
    strcpy(person->name, pname);
    person->age = age;
    person->Mother = NULL;
    person->Father = NULL;
    
    if (numPersons < MAX_PEOPLE) {
        persons[numPersons] = person;
        numPersons++;
    }
    
    return person;
}

Person* createPerson(const char* pname, int age, Person* mother, Person* father) {  //Creating children.
    Person* person = (Person*)malloc(sizeof(Person));
    strcpy(person->name, pname);
    person->age = age;
    person->Mother =(struct Person*) mother;
    person->Father = (struct Person*) father;

    if (numPersons < MAX_PEOPLE) {
        persons[numPersons] = person;
        numPersons++;
    }
    
    return person;
}


char** add_family_tree(int familycount, char** familyarr){  //Function to add a family tree.
int choice, exit=0, personage;
char filename[100], familyname[50], personname[50], mothername[50], fathername[50];
Person* person;
printf("Enter the family tree name: ");
scanf("%s", familyname);
strcpy(filename, "family");
strcat(filename, familyname);
strcat(filename, ".txt");
familyarr[familycount - 1] = strdup(filename);
FILE *fp;
fp = fopen(filename, "w");
while(exit==0){
	printf("\n1-Add node\n2-Save Family Tree\n3-Exit\n>");
	scanf("%d", &choice);
	switch(choice){

	case 1:
	printf("Enter the name of the person: ");
	scanf("%s", personname);
	printf("\nEnter the age of the person: ");
	scanf("%d", &personage);
	printf("\nEnter the name of the mother: ");
	scanf("%s", mothername);
	printf("\nEnter the name of the father: ");
	scanf("%s", fathername);
	// Check if the person has no parents
	if (strcmp(mothername, "-") == 0 && strcmp(fathername, "-") == 0) {
	person = createfirstPerson(personname, personage);
	writePersonToFile(fp, person);
	printf("\n%s has been added to the family tree succesfully...\n", personname);
	}
	else{
	// Find the mother and father by name
	Person* mother = findPersonByName(mothername);
        Person* father = findPersonByName(fathername); 
	person = createPerson(personname, personage, mother, father);
        if (mother == NULL || father == NULL) {
            printf("Mother or father not found. Cannot add %s to the family tree.\n", personname);
        } else {  // Create the person with the given mother and father
            person = createPerson(personname, personage, mother, father);
            writePersonToFile(fp, person);
            printf("\n%s has been added to the family tree successfully...\n", personname);
        }
        }
	break;

	case 2:
	printf("Saved the family tree succesfully...\n");
	exit=1;
	break;

	case 3:
	printf("Returning to System Menu.");
	exit=1;
	break;
	}
}

fclose(fp);
return familyarr;
}

// Function to remove a family tree file
void remove_family(){
char filename[50];
int ret;

printf("Enter the name of the txt file of the family tree: ");
scanf("%s", filename);

ret = remove(filename);  //Removing the wanted family file.

if(ret == 0) {
	printf("%s has been removed successfully...\n", filename);
	}
else{
	printf("Could not remove the file!\n");
	}
}

void load_family_tree(int familycount, char** familyarr){  // Function to load a family tree from a file
int familychoice, menuchoice, exit=0, relativechoice, personage;
char personname[50], mothername[50], fathername[50];
printf("\nListing Family Trees:\n");
for(int i=0;i<familycount;i++){
	printf("%d-%s\n", i+1, familyarr[i]);
	}
printf(">");
scanf("%d", &familychoice);
printf("%s has been selected\n", familyarr[familychoice-1]);
Person* person;
FILE *fp;
fp = fopen(familyarr[familychoice-1], "w");
while(exit==0){
printf("\nSelect an operation\n1-Add new person\n2-Remove a person\n3-Print a Person's nuclear family\n4-Search Relatives of Given Person\n5-Return to the main menu\n>");
scanf("%d", &menuchoice);
	switch(menuchoice){
	case 1:
	printf("Enter the name of the person: ");
	scanf("%s", personname);
	printf("\nEnter the age of the person: ");
	scanf("%d", &personage);
	printf("\nEnter the name of the mother: ");
	scanf("%s", mothername);
	printf("\nEnter the name of the father: ");
	scanf("%s", fathername);
	if (strcmp(mothername, "-") == 0 && strcmp(fathername, "-") == 0) {   // Check if the person has no parents
	person = createfirstPerson(personname, personage);
	writePersonToFile(fp, person);
	printf("\n%s has been added to the family tree succesfully...\n", personname);
	}
	break;

	case 2:
	break;

	case 3:
	printf("Enter the name of the person: ");
	scanf("%s", personname);
	person = findPersonByName(personname);

	break;

	case 4:
	printf("Enter the name of the person: ");
	scanf("%s", personname);
	person = findPersonByName(personname);
	printf("Select type of relatives:\n1-Parents\n2-Spouse\n3-Children\n4-Grandparents\n5-Cousins\n>");
	scanf("%d", &relativechoice); 
	switch(relativechoice){
		case 1:  //Printing parents of the specified person.
		printf("Printing parents of %s:\n", personname);
		if (person->Mother != NULL && person->Father != NULL) {
	        printf("\nParents: %s %s\n", person->Mother->name, person->Father->name);
                } else {
                printf("\nParents not available.\n");
                }
		break;
		
		case 2:
		printf("Printing spouse of %s:\n", personname);
		break;
		
		case 3:
		
	       if (person != NULL) {  //Printing children
	       printf("Printing children of %s:\n", personname);
               int foundChildren = 0;
               for (int i = 0; i < numPersons; i++) {
                        if (persons[i]->Mother == person || persons[i]->Father == person && persons[i] != person) {
                            printf("- %s\n", persons[i]->name);
                            foundChildren = 1;
                        }
               }
               if (!foundChildren) {
                       printf("No children found.\n");
		}
		}
	        else {
			printf("Person not found.\n");
		}
		break;
		
		case 4:
		printf("Printing grandparents of %s:\n", personname);
		if ( person->Mother != NULL && person->Father != NULL && person->Mother->Mother != NULL && person->Father->Father != NULL && person->Mother->Father != NULL && person->Father->Mother != NULL) {
	        printf("\nGrandparents(Mother's side): %s %s\nGrandparents(Father's side): %s %s\n", person->Mother->Mother->name, person->Mother->Father->name, person->Father->Mother->name, person->Father->Father->name);  //Printing grandparents.
               } else {
                printf("\nGrandparents not available.\n");
               }
		break;
		
		case 5:
		printf("Printing cousins of %s:\n", personname);
		break;
	}	
	break;

	case 5:
	printf("Returning to main menu\n");
	exit=1;
	break;

	}
}
}
int main(){
int choice,exit=0,familycount=0;
char** familyarr = calloc(10, sizeof(char*)); 
while(exit==0){
printf("********************************\nFamily Tree System Menu:\n1-Add Family Tree\n2-Remove Family Tree\n3-Load Family Tree\n4-Exit\n>");
scanf("%d", &choice);
switch(choice){

case 1:
familycount++;
add_family_tree(familycount, familyarr);

break;

case 2:
familycount--;
remove_family();
break;

case 3:
load_family_tree(familycount,familyarr);
break;

case 4:
printf("Have a good day!\n");
exit=1;
break;
//default ekle
}

}

for (int i = 0; i < familycount; i++) {
        free(familyarr[i]);
        }
free(familyarr);

return 0;
}






