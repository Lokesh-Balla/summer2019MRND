from django.shortcuts import render
from django.http import HttpResponse
from onlineapp import models


# Create your views here.
def hello(request):
    with open('hello.html', 'r') as file:
        html = file.read()
    return HttpResponse(html)


def foo(request):
    req = request.headers['foo']
    print(req)
    return HttpResponse(req)


def clg(request):
    val = models.College.objects.values_list('name').get(acronym="cvr")
    return HttpResponse(val)


# def get_all_clgs(request):
#     val = "<html><body><html><table><tr><th>College Name</th><th>Acronym</th></tr>"
#     qs = models.College.objects.values_list('name', 'acronym')
#     for x in qs:
#         val += '<tr><td>' + x[0] + '</td><td>' + x[1] + '</td></tr>'
#     val += "</table></body></html>"
#     return HttpResponse(val)

# def get_all_clgs(request):
#     colleges = models.College.objects.values('name', 'acronym')
#     return render(request, 'onlineapp/table.html', {'colleges': colleges})

def get_all_clgs(request):
    colleges = models.College.objects.values('id', 'name', 'acronym')
    return render(request, 'onlineapp/table.html', {'colleges': colleges})


def college_student(request, id):
    students = models.Student.objects.values('name', 'email', 'mocktest1__total').filter(college=id)
    return render(request, 'onlineapp/students_table.html', {'students': students})
