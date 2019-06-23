from django.views import View
from ipl import models
from functools import reduce
from django.shortcuts import render, redirect
from django.urls import resolve
from django.db.models import Count, Sum


class PointsView(View):
    def get(self, request, **kwargs):
        yid = models.Seasons.objects.get(year=kwargs['year'])
        teams = models.Matches.objects.values_list('team1', 'team2').filter(year=yid)
        teams = reduce(lambda x, y: x + y, teams)
        teams = list(set(teams))
        teams = {i: 0 for i in teams}
        details = models.Matches.objects.values().filter(year=yid)
        seasons = models.Seasons.objects.all()
        for x in details:
            if x['winner'] == '':
                teams[x['team1']] += 1
                teams[x['team2']] += 1
            else:
                teams[x['winner']] += 2

        teams = [{'name':i,'score':teams[i]} for i in teams]

        return render(
            request,
            template_name='ipl/points.html',
            context={
                'teams': teams,
                'seasons': seasons,
            },
        )