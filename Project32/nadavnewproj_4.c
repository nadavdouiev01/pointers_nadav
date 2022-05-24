#define _CRT_SECURE_NO_WARNINGS
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SIZE 1023

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Complexity Theory", +5);
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)

	//Part B
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students (1).bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students (1).bin");

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	FILE* file = fopen(fileName, "rt"); //פתחיחת קובץ
	int pipesnum;
	if (file == NULL) { 
		printf("ERROR\n");
		exit(1);
	}

	int t = 0; 
	char s[SIZE];
	while (fgets(s, SIZE, file) != NULL) { //סופר את מספר השורות (סטודנטים)
		t++;
	}
	*numberOfStudents = t;

	*coursesPerStudent = (int*)malloc(t * sizeof(int*));
	if (coursesPerStudent == NULL) {
		printf("ERROR");
		exit(1);
	}
	rewind(file); //מחזיר סמן 

	for (int i = 0; i < t; i++) {  
		
		fgets(s, SIZE, file);//קריאת שורה מהקובץ
		pipesnum = countPipes(s, SIZE); // מגלה את מספר הקורסים של אותו סטודנט
		(*coursesPerStudent)[i] = pipesnum;
	}
	fclose(file);//סגירת הקובץ
}

int countPipes(const char* lineBuffer, int maxCount)// פונקציה הסופרת את מספר הקורסים של אותו סטודנט בעזרת ספירת "|"
{
	int c = 0;
	if (maxCount <= 0) {
		return 0;
	}
	
	for (int i = 0; i < maxCount; i++) {

		if (lineBuffer[i] == '|') c++;

		if (lineBuffer[i] == '\0') break;
	}
	return c;
}



		char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
		{
			FILE* file = fopen(fileName, "rt");

			if (file == NULL) {
				printf("error\n");
				exit(1);
			}

			char*** p = (char***)malloc(*numberOfStudents * sizeof(char**));
			if (!p) {
				printf("NULL\n");
				exit(1);
			}

			countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);

			for (int j = 0; j < *numberOfStudents; j++) {
				char* k = (char*)malloc(1023 * sizeof(char));
				char* e = k;
				int y = (*coursesPerStudent)[j] * 2 + 1;
				p[j] = (char**)malloc(y * sizeof(char*));
				fgets(k, 1023, file);

				for (int i = 0; i < y; i = i + 2) {
					char* r = "|";
					char* f = strtok(k, r);//מכניסה לF את מה שיש בקובץ עד לפייפליין הראשון שהיא רואה
					p[j][i] = (char*)malloc(strlen(f) * sizeof(char) + 1);
					if (!p[j][i]) {
						printf("error\n");
						exit(1);
					}
					strcpy(p[j][i], f);
					char* h = ",";
					k = k + strlen(f) + 1;//קידום K
					char* m = strtok(k, h);
					p[j][i + 1] = (char*)malloc(strlen(m) * sizeof(char) + 1);
					if (!p[j][i + 1]) {
						printf("error\n");
						exit(1);
					}
					strcpy(p[j][i + 1], m);
					k = k + strlen(m) + 1;
				}
				/*free(e);*/
			}
			fclose(file);
		     return(p);
		   }


		void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
		{
			char** n = &students;
			int counter = 0;
			int hhh = 0;
			for (int i = 0; i < numberOfStudents; i++) {

				hhh = coursesPerStudent[i];
	
				for (int j = 0; j < (hhh*2)+1 ; j++)
				{
					if (!strcmp(students[i][j],courseName)) {//השואה בין מחרוזות על מנת לגלות אם זה אותו מקצוע 
						counter = atoi(students[i][j+1]);//המרת תו האסקי למספר שלם
						counter = counter + factor;//הוספת פקטור
						if (counter >= 100) {//אם יוצא ציון גדול מ100
							counter = 100;
							_itoa(counter, students[i][j + 1], 10);//המרת מספר שלם לתו
						}
						else
							_itoa(counter, students[i][j + 1], 10);
					}
					
				}
			}
		}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	FILE* newfile = fopen("newstudentList.txt", "wt");
	if (newfile == NULL) {
		printf("ERROR\n");
		exit(1);
	}
	char s[SIZE];
	char* p;
	char* w = "\n";
	int hhh = 0;
	for (int i = 0; i < numberOfStudents; i++) {//כתיבה לתוך הקובץ  עם הציונים המעודכנים 
		hhh = coursesPerStudent[i];
		for (int j = 0; j < ((hhh) * 2 + 1); j++) {

			strcpy(s, students[i][j]);
			if (j == (coursesPerStudent[i] * 2)) {
				p = strtok(s, w);
				fputs(p, newfile);
				fputc(w, newfile);
				break;
			}

			fputs(s, newfile);

			if (j % 2 == 0) {
				fputc('|', newfile);
			}
			else {
				fputc(',', newfile);
			}
			/*free(students[i][j]);*/ //שחרורי זיכרון 
		}
		fputc('\n', newfile);
		/*free(students[i]);*/
	}
	/*free(students);*/
	fclose(newfile);
	
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	int y = 0;
	Student* s = (Student*)calloc(numberOfStudents, sizeof(Student));
	if (!s) {
		printf("ERROR\n");
		exit(1);
	}

	for (int i = 0; i < numberOfStudents; i++) {//להכניס את מערך המצביעים לתוך מבנה מסוג סטודנט בשדות המתאימים
		strcpy(s[i].name, *students[i]);//שם הסטודנט
		s[i].numberOfCourses = coursesPerStudent[i];//מספר הקורסים של הסטודנט
		s[i].grades = (StudentCourseGrade*)calloc((coursesPerStudent[i]) * 2, sizeof(StudentCourseGrade));
		for (int j = 1; j < s[i].numberOfCourses; j = j + 2) {
			strcpy(s[i].grades[j].courseName, students[i][j]);//שם הקורס
			y = atoi(students[i][j + 1]);//המרת למספר שלם 
			s[i].grades[j].grade = y;//ציון
		}
	}
	return(s);
}


void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	FILE* binarFile = fopen(fileName, "wb");
	if (binarFile == NULL) {
		printf("ERROR - NOT OPEN\n");
		exit(1);
	}

	int counter = 0;
	fwrite(&numberOfStudents, sizeof(int),1,binarFile);//כתיבה לתוך קובץ בינארי מס סטודנטים
	int hhh; 
	for (int i = 0; i < numberOfStudents; i++) {
		fwrite(&students[i].name, sizeof(char) * 35, 1, binarFile);//שם הסטודנט
		fwrite(&students[i].numberOfCourses, sizeof(int), 1, binarFile);//מספר קורסים
		for (int j = 0; j < students[i].numberOfCourses ; j++) 
		{
			fwrite(&students[i].grades[j].courseName,sizeof(char)*35,1,binarFile);//שם הקורס
			fwrite(&students[i].grades[j].grade, sizeof(int),1, binarFile);//ציון

		}

	}
	fclose(binarFile);
}

Student* readFromBinFile(const char* fileName)//קריאה מקובץ בינארי
{

	FILE* wfile = fopen(fileName, "rb");
	if (!wfile) {
		printf("ERROE\n");
		exit(1);
	}
	int k[30];
	int u;
	fgets(k, 4, wfile);
	int r = k[0];//מספר ההסטודנטים
	Student* d = (Student*)calloc(k, sizeof(Student));
	for (int i = 0; i < r; i++) {
		
		fread(&d[i].name, sizeof(char)*35, 1, wfile);//כתיבה לקובץ שם סטודנט
		fread(d[i].numberOfCourses, sizeof(int), 1, wfile);
		d[i].grades = (StudentCourseGrade*)calloc((d[i].numberOfCourses) * 2, sizeof(StudentCourseGrade));
		for (int j = 1; j < (d[i].numberOfCourses); j = j + 2) {
			fread(&d[i].grades[j].courseName, sizeof(int), 1, wfile);
			fread(&d[i].grades[j + 1].courseName, sizeof(int), 1, wfile);
       }
	}
	fclose(wfile);
	return(d);
}








