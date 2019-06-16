from django import forms
from todo.models import *


class TodoListForm(forms.ModelForm):
    class Meta:
        model = TodoList
        exclude = ['username']
        widgets = {
            'name': forms.TextInput(attrs={'class': 'input', 'placeholder': 'Name of the list'}),
            'create': forms.DateTimeInput(attrs={'class': 'input', 'placeholder': 'enter date and time'}),
        }


class TodoItemForm(forms.ModelForm):
    class Meta:
        model = TodoItem
        exclude = ['list_name']
        widgets = {
            'description': forms.TextInput(attrs={'class': 'input', 'placeholder': 'description for the item'}),
            'due_date': forms.DateTimeInput(attrs={'class': 'date', 'placeholder': 'due date'}),
            'completed': forms.CheckboxInput(attrs={'class': 'checkbox', 'placeholder': 'check the box if complete'}),
        }
