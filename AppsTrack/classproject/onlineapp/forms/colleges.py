from onlineapp.models import College
from django import forms


class AddCollege(forms.ModelForm):
    class Meta:
        model = College
        exclude = ['id']
        widgets = {
            'name': forms.TextInput(attrs={'class': 'input', 'palceholder': 'enter the clg name'}),
            'location': forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the location'}),
            'acronym': forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the acronym'}),
            'contact': forms.EmailInput(attrs={'class': 'input', 'placeholder': 'enter the email'}),
        }
