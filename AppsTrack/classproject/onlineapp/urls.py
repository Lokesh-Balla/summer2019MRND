from django.urls import include, path
from onlineapp.views import *

urlpatterns = [
    path('colleges/', CollegeView.as_view(), name="colleges"),
    path('colleges/<int:id>', CollegeView.as_view(), name="students"),
    path('colleges/<str:acronym>', CollegeView.as_view(), name="students_acronym"),

    path('clg_add/', CollegeAdd.as_view(), name="colleges_add"),
    path('clg_add/<int:pk>/edit', CollegeAdd.as_view(), name="colleges_edit"),
    path('clg_add/<int:pk>/delete', CollegeAdd.as_view(), name="colleges_delete"),

    path('std_add/<int:pk>', StudentAdd.as_view(), name="students_add"),
    path('std_add/<int:pk>/edit', StudentAdd.as_view(), name="students_edit"),
    path('std_add/<int:pk>/delete', StudentAdd.as_view(), name="students_delete"),
    # path('hello/', views.hello),
    # path('foo/', views.foo),
    # path('clg/', views.clg),
    # path('get_all_clgs/', views.get_all_clgs),
    # path('college_student/<int:id>/', views.college_student, name='college_student'),
]
