This is CS50's final project where an education management console system using C was implemented, where the program
allows doctors to register and add courses and for students to register and enroll in any of the courses added by the doctors,
as well as the doctors having the option to add assignments for each course, and students having the option to submit solutions to
those assignments where doctors would be assigning them grades which will be shown to students when accessing their assignments.
The project is mainly implemented using hash tables, and it consists of 3 main files:
1-elements.h, contains all the structs used in the system.
2-doctors.h, contains all the functions used to implement the doctors part of the system.
3-students.h, contains all the functions used to implement the students part of the system.

All the data is stored in csv files where there are a total of 9 csv files to store the following data:
1-doctors users
2-doctors courses
3-doctors assignments
4-students users
5-students courses
6-students assignments
7-students registered in courses
8-all the submissions done by students
9-the grades results of the submissions
