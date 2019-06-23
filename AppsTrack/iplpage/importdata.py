import click
import openpyxl
import ipl
import iplpage
import csv
import os
import django
from functools import reduce

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'iplpage.settings')
django.setup()


@click.group()
def main():
    pass


@main.command()
def ImportMatches():
    file1 = open('matches.csv', 'r')
    matches = csv.reader(file1)

    for i, row in enumerate(matches):
        if i == 0:
            continue
        try:
            season = ipl.models.Seasons.objects.get(year=int(row[1]))
            print('using previous')
        except Exception as a:
            print('new')
            season = ipl.models.Seasons()
            season.year = int(row[1])
            season.save()
        match = ipl.models.Matches()
        match.year = season
        match.match_id = row[0]
        match.city = row[2]
        if '/' in row[3]:
            date = row[3].split('/')
            date[2] = '20' + date[2]
            row[3] = '-'.join(date[::-1])
        date = row[3].split('-')
        date[0], date[2] = date[2], date[0]
        row[3] = '-'.join(date)
        match.date = row[3]
        match.team1 = row[4]
        match.team2 = row[5]
        match.tossWinner = row[6]
        match.tossDecision = row[7]
        match.result = row[8]
        match.dlApplied = int(row[9])
        match.winner = row[10]
        match.winByRuns = int(row[11])
        match.winByWickets = int(row[12])
        match.playerOfTheMatch = row[13]
        match.venue = row[14]
        match.empire1 = row[15]
        match.empire2 = row[16]
        match.save()


@main.command()
def importballs():
    file2 = open('deliveries.csv', 'r')
    deliveries = csv.reader(file2)
    matchObj = 0
    for i, row in enumerate(deliveries):
        if i == 0:
            continue
        if not matchObj == row[0]:
            matchObj = ipl.models.Matches.objects.get(match_id=row[0])

        score = ipl.models.Deliveries()
        score.matchId = matchObj
        score.inning = row[1]
        score.battingTeam = row[2]
        score.bowlingTeam = row[3]
        score.over = row[4]
        score.ball = row[5]
        score.batsman = row[6]
        score.nonStriker = row[7]
        score.bowler = row[8]
        score.isSuperOver = row[9]
        score.wideRuns = row[10]
        score.byeRuns = row[11]
        score.legbyeRuns = row[12]
        score.noballRuns = row[13]
        score.penaltyRuns = row[14]
        score.batsmanRuns = row[15]
        score.extraRuns = row[16]
        score.totalRuns = row[17]
        score.playerDismissed = row[18]
        score.DismissalKind = row[19]
        score.fielder = row[20]
        score.save()
        print(i)


@main.command()
def insertTeams():
    teams = ipl.models.Matches.objects.values_list('team1', 'team2')
    teams = reduce(lambda x, y: x + y, teams)
    teams = list(set(teams))
    #print(teams)
    for x in teams:
        team = ipl.models.Teams()
        team.name = x
        team.save()

if __name__ == '__main__':
    main()
