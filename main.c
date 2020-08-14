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

int main(void)
{
    int choice = 0;
    while (choice != 3)
    {
        choice = Menu();
    }
}

int Menu(void)
{
    load_doctorsUsers("doctors_users.csv");
    load_studentsUsers("students_users.csv");
    load_studentsCourses();
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
