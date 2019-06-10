from onlineapp.models import Student, MockTest1
from django import forms


class AddStudent(forms.ModelForm):
    class Meta:
        model = Student
        exclude = ['id', 'college']
        widgets = {
            'name': forms.TextInput(attrs={'class': 'input', 'palceholder': 'enter the name'}),
            'dob': forms.DateInput(),
            'email': forms.EmailInput(attrs={'class': 'input', 'placeholder': 'enter the email'}),
            'db_folder': forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the db folder'}),
            'dropped_out': forms.CheckboxInput(),
        }


class AddMarks(forms.ModelForm):
    class Meta:
        model = MockTest1
        exclude = ['id', 'student', 'total']
        widgets = {
            'problem1': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'enter score1'}),
            'problem2': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'enter score2'}),
            'problem3': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'enter score3'}),
            'problem4': forms.NumberInput(attrs={'class': 'input', 'placeholder': 'enter score4'}),
        }


