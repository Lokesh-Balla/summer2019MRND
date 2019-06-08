import homeproject
import todo
import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'homeproject.settings')
django.setup()


def main():
    for x in range(5):
        val = todo.models.TodoList()
        val.name = 'item' + str(x)
        val.save()
        for y in range(5):
            item = todo.models.TodoItem()
            item.description = 'todo item ' + str(y)
            item.list_name = val
            item.save()


if __name__ == '__main__':
    main()
