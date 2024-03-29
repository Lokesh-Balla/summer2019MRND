from django.urls import include, path
from onlineapp.views import *
from rest_framework.authtoken.views import obtain_auth_token
from rest_framework_simplejwt import views as jwt_views

urlpatterns = [

    path('api-token-auth/', obtain_auth_token, name='api_token_auth'),
    path('api/token/', jwt_views.TokenObtainPairView.as_view(), name='token_obtain_pair'),
    path('api/token/refresh/', jwt_views.TokenRefreshView.as_view(), name='token_refresh'),

    path('colleges/', CollegeView.as_view(), name="colleges"),
    path('colleges/<int:id>', CollegeView.as_view(), name="students"),
    path('colleges/<str:acronym>', CollegeView.as_view(), name="students_acronym"),

    path('clg_add/', CollegeAdd.as_view(), name="colleges_add"),
    path('clg_add/<int:pk>/edit', CollegeAdd.as_view(), name="colleges_edit"),
    path('clg_add/<int:pk>/delete', CollegeAdd.as_view(), name="colleges_delete"),

    path('std_add/<int:pk>', StudentAdd.as_view(), name="students_add"),
    path('std_add/<int:pk>/edit', StudentAdd.as_view(), name="students_edit"),
    path('std_add/<int:pk>/delete', StudentAdd.as_view(), name="students_delete"),

    path('login/', LogOn.as_view(), name='login'),
    path('signup/', Signon.as_view(), name='signup'),
    path('logout/', Logout.as_view(), name='logout'),

    path('college/api', GetColleges.as_view(), name='api_get_college'),
    path('college/api/<int:pk>', GetColleges.as_view(), name='api_get_pk_college'),

    path('college/<int:pk>', GetStudents.as_view(), name='api_get_student'),  # for get all,delete(give student pk)
    path('college/<int:pk>/student/<int:spk>', GetStudents.as_view(), name='api_post_student'),  # for get one,put

]
