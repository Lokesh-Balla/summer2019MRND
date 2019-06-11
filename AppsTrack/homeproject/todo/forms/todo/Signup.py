from django import forms


class SignupForm(forms.Form):
    first_name = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the first name'})
    )
    last_name = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the last name'})
    )
    email = forms.EmailField(
        required=True,
        widget=forms.EmailInput(attrs={'class': 'input', 'placeholder': 'enter the mail id'})
    )
    username = forms.CharField(
        required=True,
        widget=forms.TextInput(attrs={'class': 'input', 'placeholder': 'enter the user name'})
    )
    password = forms.CharField(
        required=True,
        widget=forms.PasswordInput(attrs={'class': 'input', 'placeholder': 'enter the password'})
    )
