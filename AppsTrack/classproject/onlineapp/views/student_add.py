from django.views import View
from django.shortcuts import render, redirect
from onlineapp.forms import *
from onlineapp.models import *
from django.urls import resolve


class StudentAdd(View):
    def get(self, request, **kwargs):
        form_student = AddStudent()
        form_marks = AddMarks()
        if kwargs:
            if resolve(request.path_info).url_name == 'students_delete':
                Student.objects.get(**kwargs).delete()
                return redirect('colleges')
            elif resolve(request.path_info).url_name == 'students_edit':
                student = Student.objects.get(**kwargs)
                marks = MockTest1.objects.get(student=student.id)
                form_student = AddStudent(instance=student)
                form_marks = AddMarks(instance=marks)
        return render(
            request,
            template_name='onlineapp/student_form.html',
            context={
                'form_student': form_student,
                'form_marks': form_marks
            }
        )

    def post(self, request, **kwargs):
        if resolve(request.path_info).url_name == 'students_edit':
            student = Student.objects.get(**kwargs)
            marks = MockTest1.objects.get(student=student.id)
            form_student = AddStudent(request.POST, instance=student)
            form_marks = AddMarks(request.POST, instance=marks)
            form_student = form_student.save(commit=False)
            form_marks = form_marks.save(commit=False)
            form_marks.total = form_marks.problem1 + form_marks.problem2 + form_marks.problem3 + form_marks.problem4
            form_student.save()
            form_marks.save()
            return redirect("colleges")
        else:
            form_student = AddStudent(request.POST)
            form_marks = AddMarks(request.POST)

        if form_student.is_valid() and form_marks.is_valid():
            form_student = form_student.save(commit=False)
            form_marks = form_marks.save(commit=False)
            form_student.college = College.objects.get(**kwargs)
            form_student.save()
            form_marks.total = form_marks.problem1 + form_marks.problem2 + form_marks.problem3 + form_marks.problem4
            form_marks.student = form_student
            form_student.save()
            form_marks.save()
        return redirect("colleges")
