# Generated by Django 2.2.1 on 2019-06-11 01:39

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('onlineapp', '0003_auto_20190606_1538'),
    ]

    operations = [
        migrations.AlterField(
            model_name='college',
            name='name',
            field=models.CharField(max_length=128, unique=True),
        ),
    ]
