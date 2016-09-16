# Automatic-grading-of-true-false-questions
This is a project for "Foundations of Artificial Intelligence" lecture. This is automatic grading application for true/false questions in exam papers.  
# Method
I use template matching in order to recognize the answer of exam papers. 

There are 3 steps.

1. Finding the number of questions from "Blank.jpg" : I cut T, F Box from “Blank.jpg” and it is used as template. By using the T, F box template, this program counts the number of T, F box. Because the number of T,F box equals the number of questions, we can get the number of questions. 
2. Saving solution from solution.txt to use next step.
3. Recognizing the answer of students and grading: In order to recognize the answer of students, the template matching is used. The template is made by sample answer of a student (you can see this template file in template folder).  By template matching, this program finds the location of answers and it is saved to Struct variable. X-location of the Struct is used for classifying true or false, and Y-location of the Struct is used for classifying the question number. Finally, this program output “output.txt”.

# Result
The accuracy is 94.5% ( 567 / 600). 
See [result] (https://github.com/chc2212/Automatic-grading-of-true-false-questions/blob/master/report.txt) for information.
# Conclusion
This program shows good performance (94.5% accuracy). Also, this program can improve the performance if it uses multiple templates.  But, it has bad accuracy when there is blank answer or “v” mark instead of filling the box. 
#Sample Blank Paper
<img src="https://github.com/chc2212/Automatic-grading-of-true-false-questions/blob/master/Blank.jpg" width="300">
