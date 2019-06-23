from django.views import View
from ipl import models
from django.shortcuts import render, redirect
from django.urls import resolve
from django.contrib.auth.mixins import LoginRequiredMixin
from django.db.models import Count, Sum
from functools import reduce


class SesonsView(View):
    def get(self, request, **kwargs):
        year = models.Seasons.objects.get(year=kwargs['year'])
        seasons = models.Seasons.objects.all()
        tables = models.Matches.objects.values_list('team1', 'team2',
                                                    'city', 'tossWinner', 'tossDecision', 'winner', 'playerOfTheMatch',
                                                    'match_id', 'id').filter(year=year)
        return render(
            request,
            template_name='ipl/seasons.html',
            context={
                'year': kwargs['year'],
                'seasons': seasons,
                'tables': tables,
            },
        )

    def post(self, request, **kwargs):
        pass


class MatchView(LoginRequiredMixin, View):
    def get(self, request, **kwargs):
        tables = models.Matches.objects.values_list('tossWinner', 'tossDecision',
                                                    'winner', 'playerOfTheMatch').get(match_id=kwargs['mid'])
        match_id = models.Matches.objects.get(match_id=kwargs['mid'])
        balls = models.Deliveries.objects.values().filter(matchId=match_id)
        in1 = []
        in2 = []
        in3 = []
        in4 = []
        superover = 0
        for x in balls:
            if x['inning'] == 1:
                in1.append(x)
            elif x['inning'] == 2:
                in2.append(x)
            elif x['inning'] == 3:
                superover = 1
                in3.append(x)
            elif x['inning'] == 4:
                in4.append(x)

        # stats
        teams = list(models.Deliveries.objects.values_list('battingTeam').filter(matchId=match_id).distinct())
        teams = reduce(lambda x, y: x + y, teams)
        topScorersTeam1 = models.Deliveries.objects.values('batsman').order_by('-count').filter(
            matchId=match_id).filter(battingTeam=teams[0]).annotate(count=Sum('totalRuns'))[:3]
        topScorersTeam2 = models.Deliveries.objects.values('batsman').order_by('-count').filter(
            matchId=match_id).filter(battingTeam=teams[1]).annotate(count=Sum('totalRuns'))[:3]
        topWicketTakersTeam1 = models.Deliveries.objects.values('bowler').order_by('-count').filter(
            battingTeam=teams[0]).filter(matchId=match_id).exclude(playerDismissed='').annotate(
            count=Count('playerDismissed'))[:3]
        topWicketTakersTeam2 = models.Deliveries.objects.values('bowler').order_by('-count').filter(
            battingTeam=teams[1]).filter(matchId=match_id).exclude(playerDismissed='').annotate(
            count=Count('playerDismissed'))[:3]
        score = models.Deliveries.objects.values('battingTeam').filter(matchId=match_id).annotate(
            count=Count('totalRuns'), extra=Sum('extraRuns'))
        return render(
            request,
            template_name='ipl/matchs.html',
            context={
                'year': kwargs['year'],
                'info': tables,
                'in1': in1,
                'in2': in2,
                'in3': in3,
                'in4': in4,
                'superover': superover,
                'topScorersTeam1': topScorersTeam1,
                'topScorersTeam2': topScorersTeam2,
                'topWicketTakersTeam1': topWicketTakersTeam1,
                'topWicketTakersTeam2': topWicketTakersTeam2,
                'score': score,
            },
        )
