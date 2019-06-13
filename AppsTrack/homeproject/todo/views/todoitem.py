from django.views import View
from todo.models import *
from django.shortcuts import render, redirect
from django.urls import resolve
from django.contrib.auth.models import User
from todo.forms import *


class TodoItemView(View):
    pass
