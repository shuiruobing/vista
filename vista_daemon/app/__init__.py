# coding=utf-8

from flask import Flask
from config import config
from datetime import timedelta
from os import path

basedir = path.dirname(__file__)
sqlDir = 'sqlite:///' + path.join(basedir, 'test.db')


def create_app():
    from . import views, models
    app = Flask(__name__)
    app.config["SECRET_KEY"] = 'thansky'
    app.config["PERMANENT_SESSION_LIFETIME"] = timedelta(seconds=30)
    app.config['SQLALCHEMY_DATABASE_URI'] = sqlDir
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    # app.config.from_object(config[config_name])
    # config[config_name].init_app(app)
    views.init_app(app)
    models.init_app(app)
    return app
