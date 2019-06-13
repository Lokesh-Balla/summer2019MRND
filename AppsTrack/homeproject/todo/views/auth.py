from django.http import HttpResponse
from django.views import View
from django.contrib.auth.models import User
from django.contrib.auth import authenticate, login, logout
from django.shortcuts import render, redirect
from django.db import IntegrityError
from django.contrib import messages
from todo.forms import *


class Login(View):
    def get(self, request):
        form = LoginForm()
        return render(
            request,
            template_name='todo/login.html',
            context={
                'form': form,
            },
        )

    def post(self, request):
        form = LoginForm(request.POST)
        if form.is_valid():
            user = authenticate(
                request,
                username=form.cleaned_data['username'],
                password=form.cleaned_data['password'],
            )
            if user is not None:
                login(request, user)
                userdata = User.objects.get(username=form.cleaned_data['username'])
                return redirect('todolistview', userdata.id)
            else:
                messages.error(request, "Invalid Login Credentials")
                return render(
                    request,
                    template_name='todo/login.html',
                    context={
                        'form': form,
                    },
                )


class Signup(View):
    def get(self, request):
        form = SignupForm()
        return render(
            request,
            template_name='todo/signup.html',
            context={
                'form': form,
            },
        )

    def post(self, request):
        form = SignupForm(request.POST)
        if form.is_valid():
            try:
                user = User.objects.create_user(**form.cleaned_data)
                user.save()
                return redirect('login')
            except IntegrityError as ie:
                messages.error(request, ie)
        return render(
            request,
            template_name='todo/signup.html',
            context={
                'form': form,
            },
        )


class Logout(View):
    def get(self, request):
        logout(request)
        return redirect('login')
