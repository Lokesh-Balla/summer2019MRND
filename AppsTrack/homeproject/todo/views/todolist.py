from django.views import View
from todo.models import *
from django.shortcuts import render, redirect
from django.urls import resolve
from django.contrib.auth.models import User
from todo.forms import *


class TodoListView(View):
    def get(self, request, **kwargs):
        todolists = TodoList.objects.values('id', 'name', 'created', 'username').filter(
            username=kwargs['pk'])
        return render(
            request,
            template_name='todo/lists.html',
            context={
                'pk': kwargs['pk'],
                'todolists': todolists,
            },
        )


class TodoListAdd(View):
    def get(self, request, **kwargs):
        if resolve(request.path_info).url_name == 'todolistdelete':
            TodoList.objects.get(id=kwargs['epk']).delete()
            return redirect('todolistview', kwargs['pk'])
        if kwargs.get('epk'):
            todolist = TodoList.objects.get(id=kwargs['epk'])
            form = TodoListForm(instance=todolist)
        else:
            form = TodoListForm()
        return render(
            request,
            template_name='todo/addlist.html',
            context={
                'form': form,
            },
        )

    def post(self, request, **kwargs):
        if kwargs.get('epk'):
            todolist = TodoList.objects.get(id=kwargs['epk'])
            form = TodoListForm(request.POST, instance=todolist)
        else:
            form = TodoListForm(request.POST)
        if form.is_valid():
            form = form.save(commit=False)
            form.username = User.objects.get(id=kwargs['pk'])
            form.save()
        else:
            return render(request, template_name='todo/addlist.html', context={'form': form})
        return redirect('todolistview', kwargs['pk'])
