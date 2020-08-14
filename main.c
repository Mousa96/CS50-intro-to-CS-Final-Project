#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include "doctors.h"
#include "students.h"

int Menu(void);
void signup(void);
void login(void);
void load_doctorsUsers(const char *sheet);
void load_studentsUsers(const char *sheet);
void unload_doctorsUsers(const char *sheet);
void unload_studentsUsers(const char *sheet);
void unload_studentsCourses(void);
void unloadSystem(void);

int main(void)
{
    int choice = 0;
    while (choice != 3)
    {
        choice = Menu();
    }
    unloadSystem();
}

int Menu(void)
{
    load_doctorsUsers("doctors_users.csv");
    load_studentsUsers("students_users.csv");
    load_studentsCourses();
    load_doctorsCourses();
    int choice = 0;
    // printing options for the user
    printf("Please make a choice:\n"
            "1 - Sign up\n"
            "2 - Login\n"
            "3 - Shutdown system\n"
          );

    // ensuring that the user chooses one of the options
    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice != 1 && choice != 2 && choice != 3);

    if (choice == 1)
    {
        signup();
        return 1;
    }
    else if (choice == 2)
    {
        login();
        return 2;
    }
    else
    {
        return 3;
    }
}

void signup(void)
{
    int choice = 3;
    printf("Please make a choice or enter 0 to go back:\n"
            "1 - Sign up as a Doctor.\n"
            "2 - Sign up as a Student.\n"
            );
    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice != 1 && choice != 2 && choice != 0);

    if (choice == 0)
    {
        Menu();
    }

    else if (choice == 1)
    {
        signup_doctor();
    }

    else
    {
        signup_student();
    }


}

void login(void)
{
    int choice = 3;
    printf("Please make a choice or enter 0 to go back:\n"
            "1 - Login as a Doctor\n"
            "2 - Login as a Student\n"
            );
    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice != 1 && choice != 2 && choice != 0);

    if (choice == 0)
    {
        Menu();
    }
    else if (choice == 1)
    {
        login_doctor(3);
    }
    else
    {
        login_student(3);
    }

}

//this functions loads the database of the registered users of type doctor
void load_doctorsUsers(const char *sheet)
{
    FILE *file = fopen(sheet, "r");
    if (file == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    char *temp = malloc(sizeof(char) * 150);
    string fields[5];
    char *token;

    while (fgets(temp, 150, file))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields[i] = token;
            token = strtok(NULL, ",");
        }

        //generating unique index for the username
        int index = hash(fields[1], 100);
        //creating a new struct of type doctor and allocating memory to it
        doctor *doc = malloc(sizeof(doctor));
        //copying the data of the user to the struct members
        strcpy(doc->id, fields[0]);
        //printf(" %s ",doc->id);
        strcpy(doc->username, fields[1]);
        //printf(" %s ",doc->username);
        strcpy(doc->password, fields[2]);
        //printf(" %s ",doc->password);
        strcpy(doc->name, fields[3]);
        //printf(" %s ",doc->name);
        strcpy(doc->email, fields[4]);
        //printf(" %s \n",doc->email);
        doc->next = NULL;
        //initializing all the pointers to NULL
        for (int i = 0; i < C; i++)
        {
            doc->doctorCourses[i] = NULL;
        }
        table_doctors[index] = doc;

    }

    fclose(file);
    free(temp);
}

void load_studentsUsers(const char *sheet)
{
    FILE *file = fopen(sheet, "r");
    if (file == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    char *temp = malloc(sizeof(char) * 150);
    string fields[5];
    char *token;

    while (fgets(temp, 150, file))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields[i] = token;
            token = strtok(NULL, ",");
        }

        //generating unique index for the username
        int index = hash(fields[1], 10000);
        //creating a new struct of type doctor and allocating memory to it
        student *stud = malloc(sizeof(student));
        //copying the data of the user to the struct members
        strcpy(stud->id, fields[0]);
        //printf(" %s ",doc->id);
        strcpy(stud->username, fields[1]);
        //printf(" %s ",doc->username);
        strcpy(stud->password, fields[2]);
        //printf(" %s ",doc->password);
        strcpy(stud->name, fields[3]);
        //printf(" %s ",doc->name);
        strcpy(stud->email, fields[4]);
        //printf(" %s \n",doc->email);
        stud->next = NULL;
        //initializing all the pointers to NULL
        for (int i = 0; i < C; i++)
        {
            stud->studentCourses[i] = NULL;
        }
        table_students[index] = stud;

    }

    fclose(file);
    free(temp);
}


void unload_doctorsUsers(const char *sheet)
{
    FILE *file = fopen(sheet, "r");
    if (file == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    char *temp = malloc(sizeof(char) * 150);
    string fields[5];
    char *token;

    while (fgets(temp, 150, file))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields[i] = token;
            token = strtok(NULL, ",");
        }

        int index = hash(fields[1], 100);
        free(table_doctors[index]);

    }

    fclose(file);
    free(temp);
}

void unload_studentsUsers(const char *sheet)
{
    FILE *file = fopen(sheet, "r");
    if (file == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    char *temp = malloc(sizeof(char) * 150);
    string fields[5];
    char *token;

    while (fgets(temp, 150, file))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields[i] = token;
            token = strtok(NULL, ",");
        }

        //generating unique index for the username
        int index = hash(fields[1], 10000);
        free(table_students[index]);

    }

    fclose(file);
    free(temp);
}


void unload_studentsCourses(void)
{

    //unload students assignments
    FILE *file2 = fopen("students_assignments.csv", "r");
    if (file2 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    char *temp = malloc(sizeof(char) * 400);
    char *token;
    string fields[5];
    string fields2[4];
    int index;

    while (fgets(temp, 400, file2))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields2[i] = token;
            token = strtok(NULL, ",");
        }

        index = hash(fields2[2], 20);
        free(table_students[atoi(fields2[0])]->studentCourses[atoi(fields2[1])]->assignments[index]);
    }
    free(temp);
    fclose(file2);


    //unload students courses
    FILE *file = fopen("students_courses.csv", "r");
    if (file == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    temp = malloc(sizeof(char) * 100);

    while (fgets(temp, 100, file))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields[i] = token;
            token = strtok(NULL, ",");
        }

        free(table_students[atoi(fields[1])]->studentCourses[atoi(fields[2])]);
    }

    free(temp);
    fclose(file);
}

void unloadSystem(void)
{

    string fields1[3];
    string fields2[3];
    string fields3[4];
    string fields4[7];
    string fields5[6];



    //unload grades
    FILE *file5 = fopen("results.csv", "r");
    if (file5 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }
    char *temp = malloc(sizeof(char) * 30);
    char *token;

    while (fgets(temp, 30, file5))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields5[i] = token;
            token = strtok(NULL, ",");
        }

        free(table_doctors[atoi(fields5[1])]->doctorCourses[atoi(fields5[2])]->assignments[atoi(fields5[3])]->submissions[atoi(fields5[0])]->results[atoi(fields5[0])]);
        free(table_students[atoi(fields5[4])]->studentCourses[atoi(fields5[2])]->assignments[atoi(fields5[3])]->submissions[atoi(fields5[0])]->results[atoi(fields5[0])]);
    }
    free(temp);
    fclose(file5);


    //unload solutions
    FILE *file4 = fopen("submissions.csv", "r");
    if (file4 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }
    temp = malloc(sizeof(char) * 400);

    while (fgets(temp, 400, file4))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields4[i] = token;
            token = strtok(NULL, ",");
        }

        free(table_doctors[atoi(fields4[5])]->doctorCourses[atoi(fields4[2])]->assignments[atoi(fields4[1])]->submissions[atoi(fields4[0])]);
    }
    free(temp);
    fclose(file4);

        //unload assignments
    FILE *file3 = fopen("doctors_assignments.csv", "r");
    if (file3 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }
    temp = malloc(sizeof(char) * 300);

    while (fgets(temp, 300, file3))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields3[i] = token;
            token = strtok(NULL, ",");
        }

        int index = hash(fields3[2], 20);
        course_doctor *CD = table_doctors[atoi(fields3[0])]->doctorCourses[atoi(fields3[1])];

        free(CD->assignments[index]);
    }
    free(temp);
    fclose(file3);


        //unload registeredStudents
    FILE *file2 = fopen("registeredStudents.csv", "r");
    if (file2 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    temp = malloc(sizeof(char) * 20);

    while (fgets(temp, 20, file2))
    {
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields2[i] = token;
            token = strtok(NULL, ",");
        }


        free(table_doctors[atoi(fields2[0])]->doctorCourses[atoi(fields2[1])]->registeredStudents[atoi(fields2[2])]);
    }

    free(temp);
    fclose(file2);


    //unload doctors courses

    FILE *file = fopen("doctors_courses.csv", "r");
    if (file == NULL)
    {
        printf("file doesn't exist");
    }
    temp = malloc(sizeof(char) * 100);
    while(fgets(temp, 100, file))
    {
        token = strtok(temp, ",");

        for (int i = 0; token != NULL; i++)
        {
            fields1[i] = token;
            token = strtok(NULL, ",");
        }

        int index = hash(fields1[2], 20);
        free(table_doctors[atoi(fields1[0])]->doctorCourses[index]);
    }

    fclose(file);
    free(temp);

    unload_studentsCourses();
    unload_doctorsUsers("doctors_users.csv");
    unload_studentsUsers("students_users.csv");
}
