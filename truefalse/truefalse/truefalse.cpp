#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2\opencv.hpp>

#include <fstream>
#include <string>
#include <stdlib.h>
#include <Windows.h>



using namespace cv;
using namespace std;

bool my_cmp(const CvPoint& a, const CvPoint& b)
{
	return a.y < b.y;
}



void main()
{
	

	//1. Find the number of questions. ------------------------------Begin -----------------
	double min, max;
	int question_number=0;
	CvPoint left_top;
	IplImage *A = cvLoadImage("input/Blank.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *B = cvLoadImage("templete/templete4.png", CV_LOAD_IMAGE_GRAYSCALE);



	IplImage* C = cvCreateImage( cvSize( A->width - B->width+1, A->height - B->height+1 ), IPL_DEPTH_32F, 1 ); 

	int standardX=0;  
	for (int i=0; i<40;i++)
	{		
		cvMatchTemplate(A, B, C, CV_TM_CCOEFF_NORMED); 
		cvMinMaxLoc(C, &min, &max, NULL, &left_top); 
		printf("%d : %f x: %d, y: %d \n",i,max,left_top.x,left_top.y);
		cvRectangle(A, left_top, cvPoint(left_top.x + B->width, left_top.y + B->height), CV_RGB(255,255,255),CV_FILLED); 

		if(i==0)
			standardX = left_top.x;
		if(left_top.x <  standardX-5 || left_top.x >  standardX + 5)
		{
			question_number = i;
			break;	  
		}

	}
	printf("The number of Questions : %d\n",question_number);

	//1. Find the number of questions. ------------------------------End -----------------
	bool solutionanswer[40];
	for(int i=0;i<40;i++)
		solutionanswer[i]=true;
	//2. save solution  ----- begin -----
	string line;
	ifstream myfile ("input/Solution.txt");
	int line_n=0;
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			if(line.find("F") == std::string::npos)
			{
				cout << line << "True Success"  << '\n';
				solutionanswer[line_n]=true;
			}
			else
			{
				cout << line << "False Success"  << '\n';
				solutionanswer[line_n]=false;
			}
			line_n++;
		}
		myfile.close();
	}
	else 
	{
		cout << "Unable to open solution file\n"; 
		return ;
	}
	//2. save solution  ----- end -----


	//3.	Recognizing students answer and grading. -------Begin--------
	ofstream myoutputfile;
    myoutputfile.open ("input/report.txt");

	//input file name(students) setting 
	for(int j=1; j<=200;j++)
	{
		char stu[50];
		char stu1[30] = "input/student00";
		char stu2[30] = "input/student0";
		char last_word[30] = ".jpg";
		char string_temp[30];
		sprintf(string_temp,"%d",j);
		
			strcat(stu1,string_temp);
			strcat(stu1,last_word);				
	
			strcat(stu2,string_temp);
			strcat(stu2,last_word);		
		if(j<=9)
			strcpy(stu,stu1);
		else
			strcpy(stu,stu2);	
		
		
		

	bool the_answer[100];
	for(int i=0;i<100;i++)
		the_answer[i] =false;
	double centerX=500;

	CvPoint temp_X[40];
	for(int i=0;i<40;i++)
	{
		temp_X[i].x = 0; 
		temp_X[i].y = 99999;
	}

	
	IplImage *student = cvLoadImage(stu, CV_LOAD_IMAGE_GRAYSCALE); 	
	if(student == NULL) 
		break;
	IplImage *stduent_templete = cvLoadImage("templete/templete3.png", CV_LOAD_IMAGE_GRAYSCALE); 
	IplImage *D = cvCreateImage( cvSize( student->width - stduent_templete->width+1, student->height - stduent_templete->height+1 ), IPL_DEPTH_32F, 1 ); 

	bool temp=true;

	//input the answer into temp_X
	for(int i=0;i<question_number;i++)
	{
		cvMatchTemplate(student, stduent_templete, D, CV_TM_CCOEFF_NORMED); 
		cvMinMaxLoc(D, &min, &max, NULL, &left_top); 
		cvRectangle(student, left_top, cvPoint(left_top.x + stduent_templete->width, left_top.y + stduent_templete->height), CV_RGB(255,255,255),CV_FILLED);
		printf("%d : %f x: %d, y: %d \n",i,max,left_top.x,left_top.y);

		temp_X[i] = left_top;	

		//finding cetner of true x-axis and false x-axis
		if(temp==true)
			if(left_top.x > temp_X[0].x + 60 || left_top.x < temp_X[0].x -60)
			{
				centerX = (left_top.x + temp_X[0].x)/2;
				temp=false;
				printf("Cneter X =%f\n",centerX); 
			}	
	}

	
	//sort
	std::sort(temp_X, temp_X+39, my_cmp);

	//output

	char student_number_temp[30];
	char student_number1[30] = "student00";
	char student_number2[30] = "student0";
	char string_temp2[30];
	sprintf(string_temp2,"%d",j);
	strcat(student_number1,string_temp2);
	strcat(student_number2,string_temp2);	
	if(j<=9)
		strcpy(student_number_temp,student_number1);
	else
		strcpy(student_number_temp,student_number2);

	 myoutputfile << student_number_temp <<":\n";
	 int the_correct_number=0;
	 bool the_correct_answer_temp=true;
	
	 for(int i=0;i<question_number;i++)
	{
		 myoutputfile << "Q";
		 myoutputfile << i+1;
		 myoutputfile << ":";
		 //If student's answer is true
		 if(temp_X[i].x < centerX)
		 {
			myoutputfile << "T";
			if(solutionanswer[i] == true)
				the_correct_answer_temp = true;
			else
				the_correct_answer_temp = false;
		 }
		 else //if student's answer is false
		 {
				myoutputfile << "F";
				if(solutionanswer[i] == true)
				the_correct_answer_temp = false;
				else
				the_correct_answer_temp = true;
		 }

		 if(the_correct_answer_temp == true)
		 {
			 myoutputfile << " correct";
			 the_correct_number++;

		 }
		 else
			  myoutputfile << " incorrect";	 		

		 myoutputfile <<"\n";
	 }
	  myoutputfile <<"Score : " << the_correct_number << " / " << question_number << "\n";
	  myoutputfile << "#" << "\n";

	}
	//2.	Recognizing students answer and grading. -------End----------

	
    myoutputfile.close();
	cvNamedWindow("result",CV_WINDOW_AUTOSIZE);



	cvWaitKey(0);

	
	cvReleaseImage(&A);
	cvReleaseImage(&B);
	cvReleaseImage(&C);

	
	cvDestroyAllWindows();



}

