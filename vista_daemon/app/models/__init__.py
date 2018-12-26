# -*- coding: utf-8 -*-
# Author: thansky


from .base import db
from .users import UserStorage
from .tasks import TaskStorage


def init_app(app):
    db.init_app(app)
    db.app = app
    db.create_all()


# __all__ = ['users', 'tasks']
