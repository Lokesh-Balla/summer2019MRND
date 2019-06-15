from rest_framework.views import APIView
from rest_framework.response import Response
from rest_framework import status
from onlineapp.serializers import *
from rest_framework.authentication import SessionAuthentication, BasicAuthentication, TokenAuthentication
from rest_framework.permissions import IsAuthenticated
from rest_framework_simplejwt.authentication import JWTAuthentication


class GetColleges(APIView):
    authentication_classes = (JWTAuthentication, BearerAuthentication, BasicAuthentication,
                              SessionAuthentication, TokenAuthentication)
    permission_classes = (IsAuthenticated,)

    def get(self, request, **kwargs):
        if kwargs:
            colleges = College.objects.all().filter(**kwargs)
        else:
            colleges = College.objects.all()
        if colleges:
            serializer = CollegeSerializer(colleges, many=True)
            return Response({'colleges': serializer.data}, status=status.HTTP_200_OK)
        else:
            return Response(status=status.HTTP_404_NOT_FOUND)

    def post(self, request, **kwargs):
        college = CollegeSerializer(data=request.data)
        if college.is_valid():
            college.save()
            return Response(status=status.HTTP_201_CREATED)
        else:
            return Response(status=status.HTTP_400_BAD_REQUEST)

    def put(self, request, **kwargs):
        if kwargs:
            college = CollegeSerializer(data=request.data, instance=College.objects.get(**kwargs))
            if college.is_valid():
                college.save()
                return Response(status=status.HTTP_200_OK)
            else:
                return Response(status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request, **kwargs):
        if kwargs:
            College.objects.get(**kwargs).delete()
            return Response(status=status.HTTP_200_OK)
        else:
            return Response(status=status.HTTP_400_BAD_REQUEST)


class GetStudents(APIView):
    authentication_classes = (
        JWTAuthentication, BearerAuthentication, BasicAuthentication, SessionAuthentication, TokenAuthentication)
    permission_classes = (IsAuthenticated,)

    def get(self, request, **kwargs):
        if kwargs.get("spk"):
            students = Student.objects.filter(pk=kwargs['spk'])
        else:
            students = Student.objects.filter(college=kwargs['pk'])
        if students:
            serializer = StudentDetailsSerializer(students, many=True)
            return Response({'students': serializer.data}, status=status.HTTP_200_OK)
        else:
            return Response(status=status.HTTP_404_NOT_FOUND)

    def post(self, request, **kwargs):
        student = StudentDetailsSerializer(data=request.data)
        if student.is_valid():
            student.save()
            return Response(status=status.HTTP_201_CREATED)
        else:
            return Response(status=status.HTTP_400_BAD_REQUEST)

    def put(self, request, **kwargs):
        student = StudentDetailsSerializer(data=request.data, instance=Student.objects.get(id=kwargs['spk']))
        if student.is_valid():
            student.save()
            return Response(student.data, status=status.HTTP_201_CREATED)
        else:
            return Response(status=status.HTTP_400_BAD_REQUEST)

    def delete(self, request, **kwargs):
        if kwargs:
            Student.objects.get(**kwargs).delete()
            return Response(status=status.HTTP_200_OK)
        else:
            return Response(status=status.HTTP_400_BAD_REQUEST)
