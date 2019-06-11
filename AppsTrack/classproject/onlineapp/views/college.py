from django.views import View
from onlineapp import models
from django.shortcuts import render, get_object_or_404
from django.contrib.auth.mixins import LoginRequiredMixin


class CollegeView(LoginRequiredMixin, View):
    login_url = 'login'

    def get(self, request, **kwargs):
        if kwargs:
            college = get_object_or_404(models.College, **kwargs)
            students = college.student_set.order_by("-mocktest1__total")
            per=request.user.get_all_permissions()
            return render(
                request,
                template_name='onlineapp/students.html',
                context={'students': students,
                         'clg_name': college,
                         },
            )
        colleges = models.College.objects.values('id', 'name', 'acronym')
        return render(
            request,
            template_name='onlineapp/colleges.html',
            context={'colleges': colleges}
        )
