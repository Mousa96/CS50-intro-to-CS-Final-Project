#include <stdio.h>
#include <ctype.h>
#include <cs50.h>
#include <stdlib.h>
#include <string.h>
#include "elements.h"
#include "functions.h"


void signup_student(void);
void login_student(int trials);
void student_homepage(int index);
void RegisterCourse(int index);
void student_viewCourse(int course, int index);
void load_studentsCourses(void);
void ListMyCourses(int index);
void student_viewCourse(int course, int index);


int S = 10000; // number of allowed students in the system
// this is the signup function if the user choosed to signup as a student
void signup_student(void)
{
    bool emailCheck1 = false, emailCheck2 = false, emailValid = false;

    // getting data from the user
    char *id = get_string("Please enter your ID number: ");
    char *username = get_string("Username: ");
    char *password = get_string("Password: ");
    char *name = get_string("Name: ");
    char *email;
    do
    {
        email = get_string("Email: ");
        //check if email format is correct or not
        for (int i = 0, n = strlen(email); i < n; i++)
        {
            if (email[i] == '@')
            {
                emailCheck1 = true;
            }
            if (email[i] == '.' && emailCheck1 == true)
            {
                emailCheck2 = true;
            }
        }

        // if email format is incorrect prompt the user to enter email again
        if (emailCheck1 == true && emailCheck2 == true)
        {
            emailValid = true;
        }
        else
        {
            printf("Email format is invalid, correct usage: xxxxx@domain.com\n");
            emailCheck1 = false;
            emailCheck2 = false;
        }
    }
    while (emailValid == false);

    //using the hash function to generate a unique index for the created node above
    int index = hash(username, S);
    //if no nodes exist then this username is unique and sign up process is correct
    if (table_students[index] == NULL)
    {
        FILE *file = fopen("students_users.csv", "a");
        fprintf(file, "%s,%s,%s,%s,%s\n", id, username, password, name, email);
        fclose(file);
        printf("\n*User registered successfully.*\n\n");
    }
    else
    {
        printf("Username is taken.\n\n");
        signup_student();
    }
}


// login function for students
void login_student(int trials)
{
    if (trials != 0)
    {
        //prompts the user to enter username and password
        printf("Please enter username and password:\n");
        string username = get_string("username: ");
        string password = get_string("password: ");

        int index = hash(username, S);
        //if the index doesn't exist in the table then the username doesn't exist
        if (table_students[index] == NULL)
        {
            printf("This username doesn't exist.\n\n");
            login_student(trials - 1);
        }
        //if username exists then it checks for the password correcteness
        else if (strcmp(table_students[index]->password, password) != 0)
        {
            printf("Password is incorrect.\n");
            login_student(trials - 1);
        }
        //if both username and password are correct then it redirects the username to the homepage
        else
        {
            student_homepage(index);
        }
    }
    //if the user fails to enter the username and password correct for x times then it shutsdown the system
    else
    {
        printf("maximum number of incorrect logins reached. Shutting down the system.\n");
        return ;
    }
}

// student's homepage
void student_homepage(int index)
{
    load_studentsCourses();
    load_doctorsCourses();
    printf("\n\nWelcome %s. You are logged in.\n\n",table_students[index]->name);
    printf("Please make a choice:\n"
            "1 - Register in a course.\n"
            "2 - List my courses.\n"
            //"3 - Grades report\n"
            "3 - Log out\n"
            );

    int choice;
    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice != 1 && choice != 2 && choice != 3 && choice != 4);
    if (choice == 1)
    {
        RegisterCourse(index);
        return;
    }
    else if (choice == 2)
    {
        ListMyCourses(index);
        return;
    }
    else if (choice == 3)
    {
        //GradesReport(index);
       return;
    }
    //else if (choice == 4)
    //{
      //  return;
    //}
}

void RegisterCourse(int index)
{
    int num = 0;
    int doctorIndex[2000];
    int courseIndex[2000];
    //temp *temps[2000];
    printf("Available Courses for registeration:\n\n");
    for (int i = 0; i < 100; i++)
    {
        if (table_doctors[i] != NULL)
        {
            for (int j = 0; j < 20; j++)
            {
                if (table_doctors[i]->doctorCourses[j] != NULL)
                {
                    doctorIndex[num] = i;
                    courseIndex[num] = j;

                    printf("%i- %s\n",num + 1, table_doctors[i]->doctorCourses[j]->name);
                    num++;
                }
            }
        }
    }
    printf("\nRegister in a course by entering its menu number, or enter 0 if you wish to go back.\n");
    int choice;

    if (num != 0)
    {
        do
        {
            choice = get_int("Enter choice: ");
        }
        while (choice > num || choice < 0);

        if (choice == 0)
        {
            student_homepage(index);
            return;
        }
        else
        {
            int docIndex = doctorIndex[choice - 1];
            int chosenCourseIndex = courseIndex[choice - 1];
            //update doctor's registeredStudents list
            FILE *file = fopen("registeredStudents.csv", "a");
            fprintf(file, "%d,%d,%d\n", docIndex, chosenCourseIndex, index);
            fclose(file);

            //update student courses with the new course added
            course_doctor *course = table_doctors[docIndex]->doctorCourses[chosenCourseIndex];
            int index2 = hash(course->code, 20);
            if (table_students[index]->studentCourses[index2] == NULL)
            {
                FILE *file2 = fopen("students_courses.csv", "a");
                fprintf(file2, "%d,%d,%d,%s,%s",docIndex, index, index2, course->name, course->code);
                fclose(file2);

                //update student assignments for the registered course
                for (int i = 0; i < 20; i++)
                {
                    if (course->assignments[i] != NULL)
                    {
                        FILE *file3 = fopen("students_assignments.csv", "a");
                        fprintf(file3, "%d,%d,%s,%s",index, index2, course->assignments[i]->name, course->assignments[i]->description);
                        fclose(file3);
                    }
                }
                printf("\nCourse has been registered successfully.\n\n");
                student_homepage(index);
                return;
            }
            else
            {
                printf("*You've already registered in this course.*\n");
            }
        }
    }
    else
    {
        printf("\n**There's no available courses for registeration at the moment.**");
        student_homepage(index);
    }
}


void ListMyCourses(int index)
{
    load_studentsCourses();
    load_doctorsCourses();
    int num = 0;
    string name, code;
    for (int i = 0; i < C; i++)
    {
        if (table_students[index]->studentCourses[i] != NULL)
        {
            num++;
            name = table_students[index]->studentCourses[i]->name;
            code = table_students[index]->studentCourses[i]->code;
            printf("%d - %s - %s\n",num, name, code);
        }
    }

    if (num == 0)
    {
        printf("\nThere's no courses to view.\n");
        student_homepage(index);
        return;
    }
    else
    {
        printf("Choose a course to view by entering its number or enter 0 to return to homepage.\n");
    }
    int choice;
    do
    {
        choice = get_int("Enter choice: ");
    }
    while (choice > num || choice < 0 || isalpha(choice) != 0);

    if (choice == 0)
    {
        student_homepage(index);
        return;
    }
    else
    {
        int course = hash(code, C);
        student_viewCourse(course, index);
        return;
    }
}


void student_viewCourse(int course, int index)
{
    string name = table_students[index]->studentCourses[course]->name;
    string code = table_students[index]->studentCourses[course]->code;
    int num = 0;
    int assIndices[20];
    int docIndex = atoi(table_students[index]->studentCourses[course]->docIndex);
    bool assExists = false;
    printf("Course name: %s  Course code: %s\n", name, code);

    for (int i = 0; i < 20; i++)
    {
        if (table_students[index]->studentCourses[course]->assignments[i] != NULL)
        {
            assExists= true;
            int subID = (index + course + docIndex + i) % 100;
            assIndices[num] = i;
            string assName = table_students[index]->studentCourses[course]->assignments[i]->name;
            string assDescription = table_students[index]->studentCourses[course]->assignments[i]->description;
            string status;
            int grade;
            if (table_students[index]->studentCourses[course]->assignments[i]->submissions[subID]->results[subID] != NULL)
            {
                status = "Submitted";
                grade = table_students[index]->studentCourses[course]->assignments[i]->submissions[subID]->results[subID]->grade;
            }
            else
            {
                status ="Not Submitted";
                grade = 0;
            }

            printf("%d-Assignment name: %s Assignment description: %s Assignment status: %s Assignment Grade: %d\n", num + 1, assName, assDescription, status, grade);
            num++;
        }
    }
    if (assExists != true)
    {
        printf("There's no assignments yet.\n");
        ListMyCourses(index);
    }
    else
    {
        int choice;
        do
        {
            choice = get_int("Please choose the # of the assignment you wish to submit its solution or choose 0 to go back:");
        }
        while (choice > num || choice < 0);

        if (choice == 0)
        {
            ListMyCourses(index);
            return;
        }
        else
        {
            int assIndex = assIndices[choice - 1];
            int subID = (index + course + docIndex + assIndex) % 100;
            string solution = get_string("Enter solution:");
            FILE *file = fopen("submissions.csv", "a");
            fprintf(file, "%d,%d,%d,%d,%s,%d,%s", subID, assIndex, course, index, name, docIndex, solution);
            fclose(file);
            printf("Solution submitted successfully.\n");
            ListMyCourses(index);
            return;
        }
    }
}


void load_studentsCourses(void)
{
    //load students courses
    FILE *file = fopen("students_courses.csv", "r");
    if (file == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    char *temp = malloc(sizeof(char) * 100);
    char *token;
    string fields[5];
    string fields2[4];
    int index;
    while (fgets(temp, 100, file))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields[i] = token;
            token = strtok(NULL, ",");
        }

        course_student *course = malloc(sizeof(course_student));
        strcpy(course->docIndex, fields[0]);
        strcpy(course->name, fields[3]);
        strcpy(course->code, fields[4]);

        for (int i = 0; i < 20; i++)
        {
            course->assignments[i] = NULL;
        }

        table_students[atoi(fields[1])]->studentCourses[atoi(fields[2])] = course;
    }

    free(temp);
    fclose(file);

    //load students assignments
    FILE *file2 = fopen("students_assignments.csv", "r");
    if (file2 == NULL)
    {
        printf("file doesn't exist.\n");
        return;
    }

    temp = malloc(sizeof(char) * 400);
    assignment_student *assignment = malloc(sizeof(assignment_student));

    while (fgets(temp, 400, file2))
    {
        //printf("%s", temp);
        token = strtok(temp, ",");
        for (int i = 0; token != NULL; i++)
        {
            fields2[i] = token;
            token = strtok(NULL, ",");
        }

        strcpy(assignment->name, fields2[2]);
        strcpy(assignment->description, fields2[3]);

        for (int i = 0; i < 100; i++)
        {
            assignment->submissions[i] = NULL;
        }

        index = hash(fields2[2], 20);
        table_students[atoi(fields2[0])]->studentCourses[atoi(fields2[1])]->assignments[index] = assignment;
    }
    free(temp);
    fclose(file2);

    //load students submissions
    //TODO


}