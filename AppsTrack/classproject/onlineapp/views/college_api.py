from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework import status
from onlineapp.serializers import *


@api_view(['GET', 'POST'])
def GetColleges(request, **kwargs):
    if request.method == 'GET':
        if kwargs:
            colleges = College.objects.all().filter(**kwargs)
        else:
            colleges = College.objects.all()
        if colleges:
            serializer = CollegeSerializer(colleges, many=True)
            return Response({'colleges': serializer.data}, status=status.HTTP_200_OK)
        else:
            return Response(status=status.HTTP_404_NOT_FOUND)
    elif request.method == 'POST':
        college = CollegeSerializer(data=request.data)
        if college.is_valid():
            college.save()
            return Response(status=status.HTTP_201_CREATED)
        else:
            return Response(status=status.HTTP_400_BAD_REQUEST)
