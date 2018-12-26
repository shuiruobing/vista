# -*- coding: utf-8 -*-
# Author: thansky

from .gennerals import genneral_view
from .users import user_view, login_mgr
from .tasks import task_view
# from flask_login import LoginManager


# login_manager = LoginManager()
# login_manager.session_protection = 'strong'
# login_manager.login_view = 'user_view.Login'

def init_app(app):
    login_mgr.init_app(app)
    app.register_blueprint(genneral_view)
    app.register_blueprint(user_view)
    app.register_blueprint(task_view)
