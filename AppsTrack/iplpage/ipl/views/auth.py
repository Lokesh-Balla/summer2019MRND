from django.views import View
from ipl.forms import *
from django.shortcuts import render, redirect
from django.contrib.auth import authenticate, login, logout
from django.contrib import messages
from django.db.utils import *
from django.contrib.auth.models import User


class LogIn(View):
    def get(self, request, **kwargs):
        if request.user.is_authenticated:
            return redirect('seasonsview', 2019)
        form = Log_in()
        return render(
            request,
            template_name='ipl/login.html',
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
            return redirect('seasonsview', 2019)
        else:
            messages.error(request, "Invalid login credentials")
            return render(
                request,
                template_name='ipl/login.html',
                context={
                    'form': form,
                }
            )


class SignUp(View):
    def get(self, request):
        form = Sign_Up()
        return render(
            request,
            template_name='ipl/signup.html',
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
                    template_name='ipl/signup.html',
                    context={
                        'form': form,
                    }
                )

        if user is not None:
            login(request, user)
            return redirect('seasonsview', 2019)


class Logout(View):
    def get(self, request):
        logout(request)
        return redirect('login')
