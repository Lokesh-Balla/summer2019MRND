from django.views import View
from django.shortcuts import render, redirect
from onlineapp.forms import *
from django.urls import resolve


class CollegeAdd(View):
    def get(self, request, **kwargs):
        form = AddCollege()
        if kwargs:
            if resolve(request.path_info).url_name == 'colleges_delete':
                College.objects.get(**kwargs).delete()
                return redirect('colleges')
            college = College.objects.get(**kwargs)
            form = AddCollege(instance=college)

        return render(request, template_name='onlineapp/college_form.html', context={'form': form})

    def post(self, request, **kwargs):
        if kwargs:
            college = College.objects.get(**kwargs)
            form = AddCollege(request.POST, instance=college)
        else:
            form = AddCollege(request.POST)
        if form.is_valid():
            form.save()
        return redirect('colleges')
