from django.contrib.auth.models import User, Group
from rest_framework import serializers
from onlineapp.models import *


class CollegeSerializer(serializers.ModelSerializer):
    class Meta:
        model = College
        fields = ('id', 'name', 'location', 'acronym', 'contact')


class StudentSerializer(serializers.ModelSerializer):
    class Meta:
        model = Student
        fields = ('id', 'name', 'dob', 'email', 'db_folder', 'dropped_out', 'college')


class MockTest1Serializer(serializers.ModelSerializer):
    class Meta:
        model = MockTest1
        fields = ('problem1', 'problem2', 'problem3', 'problem4', 'total')


class StudentDetailsSerializer(serializers.ModelSerializer):
    mocktest1 = MockTest1Serializer()

    class Meta:
        model = Student
        fields = ('name', 'dob', 'email', 'db_folder', 'dropped_out', 'college', 'mocktest1')

    def create(self, validated_data):
        marks_data = validated_data.pop('mocktest1')
        student = Student.objects.create(**validated_data)
        MockTest1.objects.create(student=student, **marks_data)
        return student

    def update(self, instance, validated_data):
        student = validated_data.pop('mocktest1')
        mocktest1 = instance.mocktest1
        instance.name = validated_data.get('name', instance.name)
        instance.dob = validated_data.get('dob', instance.dob)
        instance.email = validated_data.get('email', instance.email)
        instance.db_folder = validated_data.get('db_folder', instance.db_folder)
        instance.dropped_out = validated_data.get('dropped_out', instance.dropped_out)
        instance.college = validated_data.get('college', instance.college)
        instance.save()

        mocktest1.problem1 = student.get('problem1', mocktest1.problem1)
        mocktest1.problem2 = student.get('problem2', mocktest1.problem2)
        mocktest1.problem3 = student.get('problem3', mocktest1.problem3)
        mocktest1.problem4 = student.get('problem4', mocktest1.problem4)
        mocktest1.total = student.get('total', mocktest1.total)
        mocktest1.save()
        return instance
