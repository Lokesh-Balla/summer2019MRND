from django.db import models
from datetime import datetime


# Create your models here.

class Seasons(models.Model):
    year = models.IntegerField(unique=True)

    def __str__(self):
        return self.year


class Matches(models.Model):
    year = models.ForeignKey(Seasons, on_delete=models.CASCADE)
    match_id = models.IntegerField(default=-1)
    city = models.CharField(max_length=120)
    team1 = models.CharField(max_length=120)
    team2 = models.CharField(max_length=120)
    date = models.DateField(default=datetime.now)
    tossWinner = models.CharField(max_length=120)
    tossDecision = models.CharField(max_length=120)
    result = models.CharField(max_length=120)
    dlApplied = models.IntegerField()
    winner = models.CharField(max_length=120)
    winByRuns = models.IntegerField()
    winByWickets = models.IntegerField()
    playerOfTheMatch = models.CharField(max_length=120)
    venue = models.CharField(max_length=120)
    empire1 = models.CharField(max_length=120)
    empire2 = models.CharField(max_length=120)


class Deliveries(models.Model):
    matchId = models.ForeignKey(Matches, on_delete=models.CASCADE)
    inning = models.IntegerField()
    battingTeam = models.CharField(max_length=120)
    bowlingTeam = models.CharField(max_length=120)
    over = models.IntegerField()
    ball = models.IntegerField()
    batsman = models.CharField(max_length=120)
    nonStriker = models.CharField(max_length=120)
    bowler = models.CharField(max_length=120)
    isSuperOver = models.IntegerField()
    wideRuns = models.IntegerField()
    byeRuns = models.IntegerField()
    legbyeRuns = models.IntegerField()
    noballRuns = models.IntegerField()
    penaltyRuns = models.IntegerField()
    batsmanRuns = models.IntegerField()
    extraRuns = models.IntegerField()
    totalRuns = models.IntegerField()
    playerDismissed = models.CharField(max_length=120)
    DismissalKind = models.CharField(max_length=120)
    fielder = models.CharField(max_length=120)


class Teams(models.Model):
    name = models.CharField(max_length=120)

    def __str__(self):
        return self.name
