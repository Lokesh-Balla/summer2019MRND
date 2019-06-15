from django.views import View
from django.shortcuts import render, redirect
from onlineapp.forms import *
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.models import User
from django.contrib import messages
from django.db.utils import *
from django.http import HttpResponse
from django.urls import resolve


class LogOn(View):
    def get(self, request):
        if request.user.is_authenticated:
            return redirect('colleges')
        form = Log_in()
        return render(
            request,
            template_name='onlineapp/login.html',
            context={
                'form': form,
            }
        )

    def post(self, request):
        form = Log_in(request.POST)
        if form.is_valid():
            user = authenticate(request, username=form.cleaned_data['username'], password=form.cleaned_data['password'])
        if user is not None:
            login(request, user)
            return redirect('colleges')
        else:
            messages.error(request, "Invalid login credentials")
            return render(
                request,
                template_name='onlineapp/login.html',
                context={
                    'form': form,
                }
            )


class Signon(View):
    def get(self, request):
        form = Sign_Up()
        return render(
            request,
            template_name='onlineapp/signup.html',
            context={
                'form': form,
            }
        )

    def post(self, request):
        form = Sign_Up(request.POST)
        if form.is_valid():
            try:
                user = User.objects.create_user(**form.cleaned_data)
                user.save()
            except IntegrityError as ie:
                messages.error(request, ie)
                return render(
                    request,
                    template_name='onlineapp/signup.html',
                    context={
                        'form': form,
                    }
                )

        if user is not None:
            login(request, user)
            return redirect('colleges')


class Logout(View):
    def get(self, request):
        logout(request)
        return redirect('login')

