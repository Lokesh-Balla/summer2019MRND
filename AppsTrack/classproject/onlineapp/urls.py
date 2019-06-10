from django.urls import include, path
from onlineapp import views

urlpatterns = [
    path('hello/', views.hello),
    path('foo/', views.foo),
    path('clg/', views.clg),
    path('get_all_clgs/', views.get_all_clgs),
    path('college_student/<int:id>/', views.college_student, name='college_student'),
]
