from django.views import View
from ipl import models
from django.db.models import Q
from django.shortcuts import render, redirect
from django.urls import resolve
from django.contrib.auth.mixins import LoginRequiredMixin
from django.db.models import Count, Sum
from functools import reduce


class TeamView(View):
    def get(self, request, **kwargs):
        teamId = models.Teams.objects.values('name').get(**kwargs)
        details = models.Matches.objects.values().filter(Q(team1=teamId['name']) | Q(team2=teamId['name'])).order_by(
            'year')
        return render(
            request,
            template_name='ipl/team.html',
            context={
                'details': details,
            },
        )