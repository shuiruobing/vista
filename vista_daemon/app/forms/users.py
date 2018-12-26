

from flask_wtf import Form
from wtforms import StringField, PasswordField, BooleanField
from wtforms.validators import Required


class UserForm(Form):
    username = StringField('Username', validators=[Required()])
    password = PasswordField('Password', validators=[Required()])
