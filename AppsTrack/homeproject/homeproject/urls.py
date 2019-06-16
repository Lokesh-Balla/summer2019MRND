"""homeproject URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.conf import settings
from django.contrib import admin
from django.urls import include, path
from todo.views import *

urlpatterns = [
    path('admin/', admin.site.urls),

    path('login/', Login.as_view(), name='login'),
    path('signup/', Signup.as_view(), name='signup'),
    path('logout/', Logout.as_view(), name='logout'),

    path('todolistview/<int:pk>', TodoListView.as_view(), name='todolistview'),
    path('todolistview/<int:pk>/add', TodoListAdd.as_view(), name='todolistadd'),
    path('todolistview/<int:pk>/edit/<int:epk>', TodoListAdd.as_view(), name='todolistedit'),
    path('todolistview/<int:pk>/delete/<int:epk>', TodoListAdd.as_view(), name='todolistdelete'),

    path('todoitemview/<int:epk>', TodoItemView.as_view(), name='todoitemview'),
    path('todoitemview/<int:epk>/<int:ipk>', TodoItemCheck.as_view(), name='todoitemcheck'),
    path('todoitemview/<int:epk>/add', TodoItemAdd.as_view(), name='todoitemadd'),
    path('todoitemview/<int:epk>/edit/<int:ipk>', TodoItemAdd.as_view(), name='todoitemedit'),
    path('todoitemview/<int:epk>/delete/<int:ipk>', TodoItemAdd.as_view(), name='todoitemdelete'),
]

if settings.DEBUG:
    import debug_toolbar

    urlpatterns = [
                      path('__debug__/', include(debug_toolbar.urls)),
                  ] + urlpatterns
