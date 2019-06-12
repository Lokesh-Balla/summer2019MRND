from django.contrib.auth.models import User, Group
from rest_framework import serializers
from onlineapp.models import *


class CollegeSerializer(serializers.ModelSerializer):
    class Meta:
        model = College
        fields = ('id', 'name', 'location', 'acronym', 'contact')
