# -*- coding: utf-8 -*-
# Author: thansky


from .base import db
from flask_login import UserMixin
import hashlib
from uuid import uuid1


class UserModel(db.Model):
    __tablename__ = 'users'

    openid = db.Column(db.Integer, primary_key=True,
                       autoincrement=True, unique=True)
    username = db.Column(db.String(128), index=True, unique=True)
    password = db.Column(db.String(128), index=True)
    token = None

    def __init__(self, username, password):
        self.username = username
        self.password = password

    def __repr__(self):
        return '<User %r>' % self.token

    def update(self, username, password):
        self.username = username
        self.password = password


class User(UserMixin):
    token = None
    model = None

    def __init__(self, user_model):
        self.model = user_model
        self.token = str(uuid1())

    def get_id(self):
        return str(self.token)


def __md5__(original_str):
    pwdMd5 = hashlib.md5()
    pwdMd5.update(original_str.encode('utf-8'))
    return pwdMd5.hexdigest()


class UserStorage(object):
    user_list = list()

    @staticmethod
    def regist(username, password):
        user_model = UserModel.query.filter_by(username=username).first()
        if not user_model:
            new_user_model = UserModel(username, password)
            db.session.add(new_user)
            db.commit()
            user = User(new_user_model)
            UserStorage.user_list.append(user)
            return user
        return None

    @staticmethod
    def authenticate(username, password):
        user_model = UserModel.query.filter_by(username=username).first()
        if user_model and password == __md5__(user_model.password):
            user = User(user_model)
            UserStorage.user_list.append(user)
            return user
        return None

    @staticmethod
    def check_on_line(token):
        for user in UserStorage.user_list:
            if user.token == token:
                return user
        return None

    @staticmethod
    def off_line(token):
        UserStorage.user_list = [user for user in UserStorage.user_list if
                                 user.token != token]

    @staticmethod
    def destory_user(username):
        user = UserModel.query.filter_by(username=username).first()
        if user:
            db.session.delete(user)
            db.session.commit()
        UserStorage.user_list = [user for user in UserStorage.user_list
                                 if user.model.username != username]
