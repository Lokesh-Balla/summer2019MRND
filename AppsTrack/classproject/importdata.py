import click
import openpyxl
from bs4 import BeautifulSoup
import onlineapp
import classproject
import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'classproject.settings')
django.setup()


@click.group()
def main():
    pass


@main.command()
@click.argument('fname', nargs=1)
def excltodb(fname):
    exfile = openpyxl.load_workbook(fname)
    exsheet = exfile['Colleges']
    skip_first = 0
    for row in exsheet:
        if skip_first == 0:
            skip_first = 1
            continue
        val = onlineapp.models.College()
        val.name = row[0].value
        val.acronym = row[1].value
        val.location = row[2].value
        val.contact = row[3].value
        val.save()
    skip_first = 0
    exsheet = exfile['Current']
    for row in exsheet:
        if skip_first == 0:
            skip_first = 1
            continue
        val = onlineapp.models.Student()
        val.name = row[0].value
        val.email = row[2].value
        val.db_folder = row[3].value
        clg_names = onlineapp.models.College.objects.all()
        for x in clg_names:
            if x.acronym == row[1].value:
                val.college = onlineapp.models.College.objects.get(pk=x.id)
                break
        val.save()
    skip_first = 0
    exsheet = exfile['Deletions']
    for row in exsheet:
        if skip_first == 0:
            skip_first = 1
            continue
        val = onlineapp.models.Student()
        val.name = row[0].value
        val.email = row[2].value
        val.db_folder = row[3].value
        val.dropped_out=True
        clg_names = onlineapp.models.College.objects.all()
        for x in clg_names:
            if x.acronym == row[1].value:
                val.college = onlineapp.models.College.objects.get(pk=x.id)
                break
        val.save()


@main.command()
@click.argument('fname', nargs=1)
def htmltodb(fname):
    with open(fname, 'r') as fp:
        soup = BeautifulSoup(fp, "html.parser")
    rows = soup.findAll('tr')
    skip_first = 0
    for row in rows:
        if skip_first == 0:
            skip_first = 1
            continue
        val = onlineapp.models.MockTest1()
        row = [x.text for x in row]
        val.problem1 = row[2]
        val.problem2 = row[3]
        val.problem3 = row[4]
        val.problem4 = row[5]
        val.total = row[6]
        temp = row[1]
        temp = temp[temp.index('_') + 1:temp.rindex('_')]
        temp = temp[temp.index('_') + 1:]
        stu_names = onlineapp.models.Student.objects.all()
        for x in stu_names:
            if x.db_folder.lower() == temp.lower():
                try:
                    val.student = onlineapp.models.Student.objects.get(pk=x.id)
                    val.save()
                except ('IntegrityError', 'Error'):
                    continue
                break


if __name__ == '__main__':
    main()
