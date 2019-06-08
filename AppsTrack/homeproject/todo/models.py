from django.db import models
from datetime import datetime


# Create your models here.

class TodoList(models.Model):
    name = models.CharField(max_length=120)
    created = models.DateTimeField(default=datetime.now)

    def __str__(self):
        return self.name


class TodoItem(models.Model):
    description = models.CharField(max_length=700)
    due_date = models.DateTimeField(blank=True, null=True)
    completed = models.BooleanField(default=False)
    list_name = models.ForeignKey(TodoList, on_delete=models.CASCADE)

    def __str__(self):
        return self.description
