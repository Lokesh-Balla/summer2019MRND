from django import forms


class LoginForm(forms.Form):
    username = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the username'}),
    )
    password = forms.CharField(
        required=True,
        widget=forms.PasswordInput(attrs={'class': 'input', 'placeholder': 'enter the password'}),
    )
