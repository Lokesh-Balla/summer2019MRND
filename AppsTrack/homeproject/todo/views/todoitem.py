from django.views import View
from todo.models import *
from django.shortcuts import render, redirect
from django.urls import resolve
from django.contrib.auth.models import User
from todo.forms import *


class TodoItemView(View):
    def get(self, request, **kwargs):
        form = TodoItem.objects.values('id', 'description', 'completed', 'due_date', 'list_name').filter(
            list_name=kwargs['epk'])
        pk = TodoList.objects.values_list('username').get(id=kwargs['epk'])
        return render(
            request,
            template_name='todo/items.html',
            context={
                'pk': pk[0],
                'epk': kwargs['epk'],
                'form': form,
            }
        )


class TodoItemAdd(View):
    def get(self, request, **kwargs):
        if resolve(request.path_info).url_name == 'todoitemdelete':
            TodoItem.objects.get(id=kwargs['ipk']).delete()
            return redirect('todoitemview', kwargs['epk'])
        if kwargs.get('ipk'):
            todolist = TodoItem.objects.get(id=kwargs['ipk'])
            form = TodoItemForm(instance=todolist)
        else:
            form = TodoItemForm()
        return render(
            request,
            template_name='todo/additem.html',
            context={
                'form': form,
            },
        )

    def post(self, request, **kwargs):
        if kwargs.get('ipk'):
            todoitem = TodoItem.objects.get(id=kwargs['ipk'])
            form = TodoItemForm(request.POST, instance=todoitem)
        else:
            form = TodoItemForm(request.POST)
        if form.is_valid():
            form = form.save(commit=False)
            form.list_name = TodoList.objects.get(id=kwargs['epk'])
            form.completed = False
            form.save()
        else:
            return render(request, template_name='todo/additem.html', context={'form': form})
        return redirect('todoitemview', kwargs['epk'])


class TodoItemCheck(View):
    def post(self, request, **kwargs):
        # for checkbox value change
        entry = TodoItem.objects.get(id=kwargs['ipk'])
        entry.completed = not entry.completed
        entry.save()
        return redirect('todoitemview', kwargs['epk'])
